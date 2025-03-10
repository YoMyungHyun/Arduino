#define TOUCH 7
#define LED 9

unsigned long startTime;
unsigned long touchTime;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(TOUCH, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(TOUCH) == HIGH){
    count++;
    startTime = millis();
    while(digitalRead(TOUCH) == HIGH);
    touchTime = millis() - startTime;
    if(touchTime >= 2000) {
      analogWrite(LED,0);
      count = 0;
    }
  }

  switch(count){
    case 1:
    analogWrite(LED, 85);
    break;
    case 2:
    analogWrite(LED, 170);
    break;
    case 3:
    analogWrite(LED, 255);
    break;
    case 4:
    analogWrite(LED, 0);
    count = 0;
    break;
  }
}
