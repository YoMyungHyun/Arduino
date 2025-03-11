#define RED_PIN 11 
#define GREEN_PIN 10 
#define BLUE_PIN 9 

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT); 
  pinMode(GREEN_PIN, OUTPUT); 
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int di = 60; di<90; di+=2){
    if(di >= 80){
      colorPrint(255,0,0);
    }
    else if(di >= 75){
      colorPrint(255,128,0);
    }
     else if(di >= 68){
      colorPrint(0,255,0);
    }
    else {
      colorPrint(0,0,255);
    }
    delay(500);
}
}
void colorPrint(int redValue, int greenValue, int blueValue) // 색깔을 출력하는 함수 정의
{
  analogWrite(RED_PIN, redValue); 
  analogWrite(GREEN_PIN, greenValue);
  analogWrite(BLUE_PIN, blueValue);    
}
