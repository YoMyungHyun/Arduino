#include "PCF8574.h"        // PCF8574 라이브러리
#include <Servo.h>          // Servo 라이브러리
#include <SoftwareSerial.h> // 소프트웨어 시리얼 라이브러리
#include <Wire.h>           // I2C 통신 라이브러리

/* 아두이노 보드 핀 설정 */
#define TXD 2                 // TXD 핀
#define RXD 3                 // RXD 핀
#define SPEED_L 6             // 좌측 모터 PWM 핀
#define DC_IN1_L 4            // 좌측 모터 IN1
#define DC_IN2_L 7            // 좌측 모터 IN2
#define SPEED_R 11            // 우측 모터 PWM 핀
#define DC_IN1_R 8            // 우측 모터 IN1
#define DC_IN2_R 12           // 우측 모터 IN2
#define PCF8574_ADDRESS 0x20  // PCF8574 I2C 주소

// 장애물 감지를 위한 임계값 (cm)
constexpr int OBSTACLE_THRESHOLD = 40;

/* PCF8574 확장 모듈 핀 설정 (초음파 센서) */
PCF8574 pcf(PCF8574_ADDRESS);
const int LEFTECHO  = 0;   // PCF8574 P0 (좌측 Echo)
const int LEFTTRIG  = 1;   // PCF8574 P1 (좌측 Trig)
const int RIGHTECHO = 2;   // PCF8574 P2 (우측 Echo)
const int RIGHTTRIG = 3;   // PCF8574 P3 (우측 Trig)
const int FRONTECHO = 4;   // PCF8574 P4 (전방 Echo)
const int FRONTTRIG = 5;   // PCF8574 P5 (전방 Trig)

/* ------------------------------------------------------------------
   1. 모터 제어 클래스 (MotorController)
------------------------------------------------------------------- */
class MotorController {
  int speedPinLeft, in1Left, in2Left;
  int speedPinRight, in1Right, in2Right;
public:
  MotorController(int spdL, int i1L, int i2L, int spdR, int i1R, int i2R)
    : speedPinLeft(spdL), in1Left(i1L), in2Left(i2L),
      speedPinRight(spdR), in1Right(i1R), in2Right(i2R) {}

  void begin() {
    pinMode(speedPinLeft, OUTPUT);
    pinMode(in1Left, OUTPUT);
    pinMode(in2Left, OUTPUT);
    pinMode(speedPinRight, OUTPUT);
    pinMode(in1Right, OUTPUT);
    pinMode(in2Right, OUTPUT);
  }
  
  // 모터 방향 설정 ('F': 전진, 'B': 후진, 'S': 정지)
  void setDirection(char dir) {
    switch (dir) {
      case 'F': // 전진
        digitalWrite(in1Left, HIGH);
        digitalWrite(in2Left, LOW);
        digitalWrite(in1Right, HIGH);
        digitalWrite(in2Right, LOW);
        break;
      case 'B': // 후진
        digitalWrite(in1Left, LOW);
        digitalWrite(in2Left, HIGH);
        digitalWrite(in1Right, LOW);
        digitalWrite(in2Right, HIGH);
        break;
      case 'S': // 정지
      default:
        digitalWrite(in1Left, LOW);
        digitalWrite(in2Left, LOW);
        digitalWrite(in1Right, LOW);
        digitalWrite(in2Right, LOW);
        break;
    }
  }
  
  // 모터 속도 설정 (0 ~ 255)
  void setSpeed(int speed) {
    analogWrite(speedPinLeft, speed);
    analogWrite(speedPinRight, speed);
  }
};

/* ------------------------------------------------------------------
   2. 서보 제어 클래스 (ServoController)
   - 이번에는 중앙을 90도로 설정, 좌측 회전은 70도, 우측 회전은 110도로 조향
------------------------------------------------------------------- */
class ServoController {
  Servo servo;
  int servoPin;
  int pos;  // 현재 서보 각도
  static const int centerPos = 22;
  static const int leftPos = 2;
  static const int rightPos = 42;
public:
  ServoController(int sPin) : servoPin(sPin), pos(centerPos) {}

  void begin() {
    servo.attach(servoPin);
    servo.write(pos);
  }
  
  // 부드러운 각도 이동 (delay 20ms)
  void setAngleSmoothly(int start, int end) {
    if (start < end) {
      for (int i = start; i <= end; i++) {
        servo.write(i);
        delay(20);
      }
    } else {
      for (int i = start; i >= end; i--) {
        servo.write(i);
        delay(20);
      }
    }
    pos = end;
  }

  // 중앙으로 복귀
  void center() {
    if (pos != centerPos) {
      setAngleSmoothly(pos, centerPos);
    }
  }

  // 좌측 회전: 70도로
  void turnLeft() {
    // if (pos != centerPos) {
    //   center();
    // }
    setAngleSmoothly(pos, leftPos);
  }
  
  // 우측 회전: 110도로
  void turnRight() {
    // if (pos != centerPos) {
    //   center();
    // }
    setAngleSmoothly(pos, rightPos);
  }
};

/* ------------------------------------------------------------------
   3. 초음파 센서 클래스 (UltrasonicSensor)
------------------------------------------------------------------- */
class UltrasonicSensor {
  PCF8574* pcfPtr;
  int trigPin;
  int echoPin;
public:
  UltrasonicSensor(PCF8574* pcf, int trig, int echo)
    : pcfPtr(pcf), trigPin(trig), echoPin(echo) {}

  // 거리 측정 (cm 단위)
  float getDistance() {
    pcfPtr->write(trigPin, HIGH);
    delayMicroseconds(10);
    pcfPtr->write(trigPin, LOW);

    unsigned long startTime = millis();
    // 타임아웃 1초
    while (pcfPtr->read(echoPin) == LOW) {
      if (millis() - startTime > 1000) return -1;
    }

    unsigned long pulseStart = micros();
    while (pcfPtr->read(echoPin) == HIGH) {
      if (millis() - startTime > 1000) return -1;
    }
    unsigned long pulseEnd = micros();

    long duration = pulseEnd - pulseStart;
    float distance = duration * 0.0343 / 2;
    return distance;
  }
};

/* ------------------------------------------------------------------
   4. RC카 제어 클래스 (RC_Car)
   - 블루투스 명령에 따라 수동 모드 또는 자동 모드를 실행
   - 자동 모드에서는 DC모터는 계속 전진하며, 초음파 센서 정보를 0.2초마다 받아 장애물에 따라 조향합니다.
------------------------------------------------------------------- */
class RC_Car {
  MotorController motor;
  ServoController servo;
  UltrasonicSensor sensorFront;
  UltrasonicSensor sensorLeft;
  UltrasonicSensor sensorRight;
  SoftwareSerial* btSerial;  // 블루투스 통신용 소프트웨어 시리얼
  int speed;                 // DC모터 속도
  int command;               // 수동 조종 명령 (1~9)
  bool autoMode;             // 자동 모드 여부 (false: RC카 모드, true: autoMode)
public:
  RC_Car(SoftwareSerial* serial)
    : motor(SPEED_L, DC_IN1_L, DC_IN2_L, SPEED_R, DC_IN1_R, DC_IN2_R),
      servo(9),
      sensorFront(&pcf, FRONTTRIG, FRONTECHO),
      sensorLeft(&pcf, LEFTTRIG, LEFTECHO),
      sensorRight(&pcf, RIGHTTRIG, RIGHTECHO),
      btSerial(serial),
      speed(150),
      command(5),
      autoMode(false)
  {}

  void begin() {
    motor.begin();
    servo.begin();
    // PCF8574 트리거 핀 LOW 설정
    pcf.write(LEFTTRIG, LOW);
    pcf.write(RIGHTTRIG, LOW);
    pcf.write(FRONTTRIG, LOW);
  }
  
  // 블루투스 명령 처리 및 모드 업데이트  
  // 앱인벤터에서 0번 버튼이 터치되면 모드가 토글됩니다.
  void update() {
    if (btSerial->available()) {
      String btStr = btSerial->readStringUntil('c'); // 'c'까지 읽기
      int btNum = btStr.toInt();
      
      // 10 이상의 값은 속도 업데이트로 처리
      if (btNum > 9) {
        speed = btNum;
      }
      // 0번 명령: 모드 토글
      else if (btNum == 0) {
        autoMode = !autoMode;
        if (autoMode)
          Serial.println("Auto Mode ON");
        else
          Serial.println("RC-Car Mode ON");
        delay(200);  // 모드 전환 debounce용 짧은 딜레이
      }
      // 수동 명령 (1~9)은 autoMode가 아닐 때만 반영
      else if (!autoMode) {
        command = btNum;
        manualDrive(command);
      }
    }
    
    // 모드에 따라 실행
    if (autoMode) {
      autoDrive();
      delay(1000);  // 1초 주기로 센서 읽기 및 판단
    }
    else {
      // 수동 모드인 경우 주기적으로 명령 실행 (필요 시 delay 조정)
      delay(1000);
    }
  }
  
  // 수동 조종 함수 (이전 코드 유지)
  void manualDrive(int cmd) {
    switch (cmd) {
      case 1: leftForward();  break;
      case 2: forward();      break;
      case 3: rightForward(); break;
      case 4: turnLeft();     break;
      case 5: stop();         break;
      case 6: turnRight();    break;
      case 7: leftBack();     break;
      case 8: back();         break;
      case 9: rightBack();    break;
      default: stop();        break;
    }
  }
  
  // 동작 함수들 (수동 모드 예제)
  void leftForward() {
    motor.setDirection('F');
    servo.turnLeft();
    motor.setSpeed(speed);
    delay(2100);
    servo.center();
  }
  
  void forward() {
    motor.setDirection('F');
    motor.setSpeed(speed);
  }
  
  void rightForward() {
    motor.setDirection('F');
    servo.turnRight();
    motor.setSpeed(speed);
    delay(2100);
    servo.center();
  }
  
  void turnLeft() {
    servo.turnLeft();
  }
  
  void stop() {
    motor.setSpeed(0);
    motor.setDirection('S');
    servo.center();
  }
  
  void turnRight() {
    servo.turnRight();
  }
  
  void leftBack() {
    motor.setDirection('B');
    servo.turnLeft();
    motor.setSpeed(speed);
    delay(2100);
    servo.center();
  }
  
  void back() {
    motor.setDirection('B');
    motor.setSpeed(speed);
  }
  
  void rightBack() {
    motor.setDirection('B');
    servo.turnRight();
    motor.setSpeed(speed);
    delay(2100);
    servo.center();
  }
  
  // 자동 주행 함수  
  // DC모터는 계속 전진하면서 1초마다 센서 값을 읽어 아래 규칙에 따라 조향합니다.
  void autoDrive() {
    float front = sensorFront.getDistance();
    float left = sensorLeft.getDistance();
    float right = sensorRight.getDistance();

    // DC모터는 항상 전진 상태로 설정
    motor.setDirection('F');
    motor.setSpeed(speed);
    
    // 조건 판단 (우선순위: 세 센서 모두 장애물 > front+right > front+left > front 단독)
    if (front < OBSTACLE_THRESHOLD && left < OBSTACLE_THRESHOLD && right < OBSTACLE_THRESHOLD) {
      // 세 센서 모두 40cm 이내: 후진 동작 (500ms)
      Serial.println("All obstacles! Reverse");
      motor.setDirection('B');
      delay(500);
      motor.setDirection('F');
      servo.center();
    }
    else if (front < OBSTACLE_THRESHOLD && right < OBSTACLE_THRESHOLD) {
      // 전방 및 오른쪽 장애물: 좌측 회전
      Serial.println("Front & Right blocked. Turn Left");
      servo.turnLeft();
      delay(100);
      servo.center();
    }
    else if (front < OBSTACLE_THRESHOLD && left < OBSTACLE_THRESHOLD) {
      // 전방 및 왼쪽 장애물: 우측 회전
      Serial.println("Front & Left blocked. Turn Right");
      servo.turnRight();
      delay(100);
      servo.center();
    }
    else if (front < OBSTACLE_THRESHOLD) {
      // 전방 장애물만 존재할 때: 기본적으로 우측으로 회전
      Serial.println("Front blocked. Turn Right");
      servo.turnRight();
      delay(100);
      servo.center();
    }
    else {
      // 장애물이 없으면 중앙 유지
      servo.center();
    }
  }
};

/* ------------------------------------------------------------------
   전역 객체 생성 및 초기화
------------------------------------------------------------------- */
SoftwareSerial btSerial(TXD, RXD);  // 블루투스 통신용 SoftwareSerial
RC_Car car(&btSerial);              // RC_Car 객체 생성

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);
  Wire.begin();
  pcf.begin();
  
  car.begin();
}

void loop() {
  car.update();
}
