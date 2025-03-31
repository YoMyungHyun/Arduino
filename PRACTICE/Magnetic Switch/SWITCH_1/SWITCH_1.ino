#define MAGNETIC 7 //마그네틱 스위치의 신호를 7번 핀으로 출력
void setup() {
  // put your setup code here, to run once:
  pinMode(MAGNETIC, INPUT_PULLUP); //7번 핀을 INPUT_PULLUP
  Serial.begin(9600); //초기화
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(MAGNETIC); //마그네틱 스위치의 출력 신호를 읽어 value 변수에 저장
  Serial.println(value); //값 출력
  delay(1000); //1초 동안 지연
}
