#include <Servo.h>

Servo myservo;  //서보 모터 제어를 위한 객체 생성

int pos = 0;    //변수 선언

void setup() {
  myservo.attach(6);  //서보 모터 제어를 위한 핀 설정
}

void loop() {
  for (pos = 0; pos <= 120; pos += 1) { //0~120 까지 1씩 증가시켜 루프를 돌림
    // in steps of 1 degree
    myservo.write(pos);    
    delay(15);   
  }
  for (pos = 120; pos >= 0; pos -= 1) { //120~0까지 1씩 감소시켜 루프를 돌림
    myservo.write(pos);             
    delay(15);                      
  }
}
