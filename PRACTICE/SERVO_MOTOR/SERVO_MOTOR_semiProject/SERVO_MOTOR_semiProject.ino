#include <Servo.h> 

#define TOUCH 7 
#define DC_MOTOR 5

Servo myservo;  // 서보 모터 제어를 위한 객체 생성

int motorStep = 0;
int rotateState = 0;  //1~3초 사이 터치 이벤트 횟수 저장
int pos = 0;    // 서보 모터 위치 값 변수 선언
unsigned long startTime; 
unsigned long touchTime; 

void motor_Control(int dcStep);

void setup() {
  myservo.attach(6);  
  pinMode(TOUCH, INPUT);
  Serial.begin(9600); 
}

void loop() {
  int touchValue = digitalRead(TOUCH); 
  delay(200);

  if(touchValue == HIGH) //터치 이벤트가 발생했을 때
  {
    motorStep++;
    Serial.print("touch count : ");
    Serial.print(motorStep); // 시리얼 모니터로 현재 DC 모터 단계 확인
    motor_Control(motorStep); 
    startTime = millis(); //터치 이벤트가 지속적으로 발행하기 전까지의 시간을 startTime 변수에 저장
    while(digitalRead(TOUCH) == HIGH); //손가락을 계속 누르고 있는 시간을 측정하기 위한 루프
    touchTime = millis() - startTime; //터치 이벤트가 지속적으로 발생한 시간을 touchTime에 저장
    Serial.print("\tcontinuous time : ");
    Serial.println(touchTime);

    if(touchTime >= 1000 && touchTime <= 3000) //지속적으로 터치 이벤트가 발생한 시간이 1~3초 사이 일 때
    {
      rotateState++; 
    }
  }

  if(rotateState%2 == 1) //rotateState 변수가 홀수 일 때 서보 모터 회전
  {
    for (pos = 0; pos <= 120; pos += 1) { // 0 ~ 120까지 1씩 증가시켜 루프를 돌림 
    myservo.write(pos);              
    delay(15);                      
    }
    for (pos = 120; pos >= 0; pos -= 1) { // 120 ~ 0까지 1씩 감소시켜 루프를 돌림
    myservo.write(pos);              
    delay(15);                       
    }    
  }  
}
void motor_Control(int dcStep){
  switch(dcStep){
    case 1: // count가 1이면
    analogWrite(DC_MOTOR, 85); 
    break;
    case 2:
    analogWrite(DC_MOTOR, 170);
    break;
    case 3:
    analogWrite(DC_MOTOR, 255);
    break;
    case 4:
    analogWrite(DC_MOTOR, 0); // count가 4이면 멈춤
    motorStep = 0;
    break;
  }
}
