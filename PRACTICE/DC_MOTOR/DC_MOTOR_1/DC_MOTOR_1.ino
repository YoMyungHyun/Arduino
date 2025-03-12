void setup() {
  // put your setup code here, to run once:
  pinMode(6, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(6, HIGH);
  delay(3000); //3초간 모터 동작
  digitalWrite(6, LOW);
  delay(1000); //1초간 모터 멈춤
}
