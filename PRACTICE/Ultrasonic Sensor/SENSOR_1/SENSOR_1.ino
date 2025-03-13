#define ECHO 2 
#define TRIG 3 

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG, OUTPUT);  //출력용으로 설정
  pinMode(ECHO, INPUT);   //입력용으로 설정
  Serial.begin(9600); //초기화 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(TRIG, HIGH); //펄스파 발생
  delayMicroseconds(10); 
  digitalWrite(TRIG, LOW);  //펄스파 종료
  long duration = pulseIn(ECHO, HIGH); 

  if(duration == 0) 
  {
    return;
  }

  long distance = duration / 58; //총 걸리는 시간을 58로 나눠 거리를 측정
  Serial.print("Distance : "); //시리얼 모니터로 거리 데이터 출력
  Serial.print(distance);  
  Serial.println("cm");
  delay(2000); 
}
