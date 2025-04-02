#include <SoftwareSerial.h>

#define TXD 2   //TXD를 2번 핀으로 설정
#define RXD 3   //RXD를 3번 핀으로 설정

SoftwareSerial mySerial(TXD, RXD);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);   //소프트웨어 시리얼 동기화
}

void loop() {
  // put your main code here, to run repeatedly:

}
