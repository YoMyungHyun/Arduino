#define ECHO 2 
#define TRIG 3 
#define RED 11
#define GREEN 10
#define BLUE 9

void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
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

  long distance = duration / 58;
  if(distance <= 15){ //주차 공간에 자동차가 있을 때 (빨간색 점등)
    analogWrite(RED, 255);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 0);
  }
  else{ //주차 공간에 자동차가 없을 때(초록색 점등)
    analogWrite(RED, 0);
    analogWrite(GREEN, 255);
    analogWrite(BLUE, 0);
  }
  delay(2000);
}
