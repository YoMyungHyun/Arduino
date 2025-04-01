#include <SoftwareSerial.h>

#define TXD 2   //TXD를 2번 핀으로 설정
#define RXD 3   //RXD를 3번 핀으로 설정

SoftwareSerial mySerial(TXD, RXD);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   //시리얼 통신 동기화
  mySerial.begin(9600);   //소프트웨어 시리얼 동기화
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mySerial.available()){   //스마트폰 앱에서 데이터를 입력했을 때
    Serial.write(mySerial.read());    //입력한 데이터를 읽어 시리얼 모니터로 출력
  }if(Serial.available()){    //시리얼  모니터에서 데이털르 입력했을 때
    mySerial.write(Serial.read());    //입력한 데이터를 읽어 블루투스 모듈로 앱에 데이터 전송
  }
}
