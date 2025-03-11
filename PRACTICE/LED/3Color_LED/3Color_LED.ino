#define RED_PIN 11
#define GREEN_PIN 10
#define BLUE_PIN 9
#define TOUCH_STATE 7
void colorPrint(int redValue, int greenValue, int blueValue);

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(TOUCH_STATE, INPUT);
}

void loop() {

  int touchValue = digitalRead(TOUCH_STATE);
  // 빨간색 출력
  analogWrite(RED_PIN, 255);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
  delay(1000);
  
  // 초록색 출력
  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 255);
  analogWrite(BLUE_PIN, 0);
  delay(1000);
}
