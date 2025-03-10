#define RED 9 // 9번 핀으로 빨간색 LED 제어
#define GREEN 8 // 8번 핀으로 초록색 LED 제어
#define TOUCH_STATE 7 // 7번 핀으로 터치 이벤트 처리

int count = 0; // 터치 횟수를 저장하는 변수

void setup() {
  pinMode(RED, OUTPUT); // 9번 핀을 출력용으로 설정
  pinMode(GREEN, OUTPUT); // 8번 핀을 출력용으로 설정
  pinMode(TOUCH_STATE, INPUT); // 7번 핀을 입력용으로 설정
}

void loop() {
  int touchValue = digitalRead(TOUCH_STATE);

  if (touchValue == HIGH) {
    count++;
  }

  if (count % 2 == 1) {
    digitalWrite(GREEN, LOW);
    delay(200);
    digitalWrite(RED, HIGH);
    delay(200);
    digitalWrite(RED, LOW);
    delay(200);
  } else {
    digitalWrite(RED, LOW);
    delay(200);
    digitalWrite(GREEN, HIGH);
  }
}
