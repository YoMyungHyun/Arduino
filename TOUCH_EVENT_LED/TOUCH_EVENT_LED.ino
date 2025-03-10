#define RED 9         // 9번 핀: 빨간색 LED
#define GREEN 8       // 8번 핀: 초록색 LED
#define TOUCH_STATE 7 // 7번 핀: 터치 센서

int count = 0;        // 터치 횟수를 저장하는 변수
bool lastTouch = LOW; // 이전 터치 상태 기록 (버튼 디바운싱 용도)

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(TOUCH_STATE, INPUT);
  Serial.begin(9600); // 시리얼 통신 초기화
}

void loop() {
  int touchValue = digitalRead(TOUCH_STATE);
  
  // 터치가 감지되었을 때(상승 에지 검출)
  if (touchValue == HIGH && lastTouch == LOW) {
    count++;
    Serial.println("Touched");
    delay(200);  // 간단한 디바운스 처리
  }
  
  lastTouch = touchValue;
  
  // 터치 횟수가 홀수이면 빨간 LED 깜빡임, 짝수이면 초록 LED 켜기
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
