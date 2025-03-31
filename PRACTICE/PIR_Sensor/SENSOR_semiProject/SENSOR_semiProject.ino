#define PIR_PIN 7 
#define LED_PIN 6 

void setup() {
  // put your setup code here, to run once:
  pinMode(PIR_PIN, INPUT); //7번 핀을 입력
  pinMode(LED_PIN, OUTPUT); //6번 핀을 출력 
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(PIR_PIN); //동작 감지 신호를 value 변수에 저장
  if(value == HIGH) //동작을 감지했을 경우
  {    
    digitalWrite(LED_PIN, HIGH); //LED 점등
    delay(9000); //9초 동안 지연
  }  
  else
  {
    digitalWrite(LED_PIN, LOW); //동작 감지 하지 않으면 LED 소등       
  }
}
