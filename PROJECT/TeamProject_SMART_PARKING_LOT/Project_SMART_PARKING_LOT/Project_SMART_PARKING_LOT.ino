#include <Servo.h>

// 핀 정의
#define TRIG_PIN 2
#define ECHO_PIN 3
#define PIEZO_PIN 12

// RGB LED 핀 정의 (공통 캐소드 기준; analogWrite 사용)
#define RGB_RED_PIN 11
#define RGB_GREEN_PIN 10
#define RGB_BLUE_PIN 9

// 차단기(서보모터) 핀 정의
#define BARRIER_SERVO_PIN 8

// 설정 값
#define DIST_THRESHOLD 30       // 임계 거리 30cm
#define DELAY_BEFORE_LOWER 10000  // 차량이 30cm 이상일 때 차단기를 내리기 전에 10초 대기

// 상태 변수
bool barrierRaised = false;      // 차단기(서보)가 올려졌는지 여부
unsigned long carLeftTime = 0;     // 차량이 주차장에서 빠져나간(30cm 이상) 시점을 기록
bool alarmTriggered = false;     // 경고 알림(피에조+LED)이 이미 발생했는지 여부

// 객체 생성
Servo barrierServo;  // 차단기 제어용 서보 객체

// RGB LED 색상 설정 함수: 각 채널에 대해 0 ~ 255 값 사용
void setLEDColor(int red, int green, int blue) {
  analogWrite(RGB_RED_PIN, red);
  analogWrite(RGB_GREEN_PIN, green);
  analogWrite(RGB_BLUE_PIN, blue);
}

// 초음파 센서를 이용한 거리 측정 함수 (cm 단위)
// 초음파의 속도: 약 0.034 cm/µs, 왕복이므로 2로 나눔
long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distanceCM = duration * 0.034 / 2;
  return distanceCM;
}

void setup() {
  // 핀 모드 설정
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  
  // 차단기 서보 모터 초기화
  pinMode(BARRIER_SERVO_PIN, OUTPUT);
  barrierServo.attach(BARRIER_SERVO_PIN);
  
  Serial.begin(9600);
  
  // 초기 상태 설정:
  // - RGB LED: 초록색 (안전 상태)
  // - 차단기(서보): 내려진 상태 (0도)
  setLEDColor(0, 255, 0);
  barrierServo.write(0);
}

void loop() {
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // 차량(또는 물체)가 30cm 이내로 들어온 경우
  if(distance > 0 && distance < DIST_THRESHOLD) {
    // 차단기가 아직 올라가지 않았다면 올림
    if(!barrierRaised) {
      barrierServo.write(90);  // 차단기를 90도로 올림 (열림)
      barrierRaised = true;
      carLeftTime = 0;         // 타이머 초기화
      Serial.println("Barrier raised!");
    }
    
    // 경고 알림이 아직 발생하지 않았다면 한 번 알림
    if(!alarmTriggered) {
      // LED를 빨간색으로 변경
      setLEDColor(255, 0, 0);
      // 피에조 스피커에서 1000Hz 음을 3초 동안 출력
      tone(PIEZO_PIN, 1000);
      delay(3000);
      noTone(PIEZO_PIN);
      // 알림 후 LED를 다시 초록색으로 복원
      setLEDColor(0, 255, 0);
      alarmTriggered = true;
    }
  }
  // 차량이 차고에서 빠져나와(30cm 이상) 있는 경우
  else {
    if(barrierRaised) {
      // 차량이 빠져난 첫 시점을 기록 (타이머 시작)
      if(carLeftTime == 0) {
        carLeftTime = millis();
        Serial.println("Car left, starting timer...");
      } else {
        // 10초 경과 후 차단기를 내림
        if(millis() - carLeftTime >= DELAY_BEFORE_LOWER) {
          barrierServo.write(0);   // 차단기를 내림 (닫힘)
          barrierRaised = false;
          alarmTriggered = false;  // 다음 차량 입장을 위해 알림 플래그 리셋
          carLeftTime = 0;
          Serial.println("Barrier lowered!");
        }
      }
    }
  }
  
  delay(100); // 센서값 안정화를 위한 짧은 대기
}
