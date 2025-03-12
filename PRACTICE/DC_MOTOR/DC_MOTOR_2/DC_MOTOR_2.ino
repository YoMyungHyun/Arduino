#include <DHT.h>
#include <DHT_U.h>

#define MOTOR 6
#define TOUCH 2

int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR, OUTPUT);
  pinMode(TOUCH, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(TOUCH);
  delay(200);

  if(value == HIGH){ //터치 센서 눌렀을때
    count++;
    switch(count){
      case 1:
      analogWrite(MOTOR, 85); // 최대 속도의 1/3 속도 출력
      break;
      case 2:
      analogWrite(MOTOR, 170); // 최대 속도의 2/3 속도 출력
      break;
      case 3:
      analogWrite(MOTOR, 255); // 최대 속도 출력
      break;
      case 4:
      analogWrite(MOTOR, 0); // 모터 멈춤
      count = 0;
      break;
    }
  }
}
