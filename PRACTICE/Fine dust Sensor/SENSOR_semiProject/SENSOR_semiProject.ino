#include <U8glib.h>   //라이브러리 추가
#include <DHT.h> 

#define INPUT_PULSE 2 //2번 핀으로 입력 펄스 인가
#define OUTPUT_VOLTAGE A0 //A0 핀으로 출력 전압 입력 
#define RED_PIN 11 //11번 핀으로 RED 핀 제어 
#define GREEN_PIN 10 //10번 핀으로 GREEN 핀 제어 
#define BLUE_PIN 9 //9번 핀으로 BLUE 핀 제어 
#define DHTPIN 3 //Signal 핀 설정
#define DHTTYPE DHT11 //온습도 센서 타입 설정
 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  //0.96인치 128x64 OLED; SSD1306 128X64 I2C 규격 선택 
DHT dht(DHTPIN, DHTTYPE); //온습도 센서 초기화 

float preVoltage = 0; //0~1023 범위의 출력 전압
float voltage = 0; //0~5 범위의 출력 전압
float dustDensity = 0; //미세먼지 농도 수치 
float sumDustDensity = 0; //미세먼지 농도 수치 합
float avgDustDensity = 0; //미세먼지 농도 수치 평균
float humidity, temperature; //온도, 습도 변수 선언

void colorPrint(int red, int green, int blue); //3색 LED 색깔 출력 함수 선언

void setup()   {                
  pinMode(INPUT_PULSE, OUTPUT); //2번 핀 출력으로 설정
  pinMode(OUTPUT_VOLTAGE, INPUT); //A0 핀 입력으로 설정
  pinMode(RED_PIN, OUTPUT); //11번 핀 출력으로 설정
  pinMode(GREEN_PIN, OUTPUT); //10번 핀 출력으로 설정
  pinMode(BLUE_PIN, OUTPUT); //9번 핀 출력으로 설정 
  dht.begin(); 
} 

void loop() {  
  humidity = dht.readHumidity(); //습도 데이터 읽기
  temperature = dht.readTemperature(); //온도 데이터 읽기      
  sumDustDensity = 0; //미세먼지 농도 수치 합을 초기화 
  for(int i=0;i<30;i++) //미세먼지 농도 수치 30회 측정
  {
    digitalWrite(INPUT_PULSE, LOW); //입력 펄스 인가
    delayMicroseconds(280); //0.28ms 대기
    preVoltage = analogRead(OUTPUT_VOLTAGE); //A0 핀으로부터 데이터를 읽어 preVoltage에 저장
    delayMicroseconds(40); //0.04ms 대기
    digitalWrite(INPUT_PULSE, HIGH); //입력 펄스 종료  
    delayMicroseconds(9680); //9.68ms 대기
    voltage = preVoltage * 5.0 / 1024.0; //0~5 범위 전압 값으로 변환 후 voltage에 저장
    dustDensity = (voltage-0.3)/0.005; //미세먼지 농도 수치 dustDensity에 저장
    sumDustDensity += dustDensity; //미세먼지 농도 수치 합계
    delay(10); //데이터 계산 간 10ms 대기
  }
  avgDustDensity = sumDustDensity / 30.0; //미세먼지 농도 수치의 평균 값을 avgDustDensity에 저장
  
  if(avgDustDensity >= 101) //매우 나쁨 단계
  {
    colorPrint(255,0,0); //빨간색 점등
  }
  else if(avgDustDensity >= 51) //나쁨 단계
  {
    colorPrint(255,10,0); //주황색 점등
  } 
  else if(avgDustDensity >= 31) //보통 단계
  {
    colorPrint(0,255,0); //초록색 점등
  }
  else //좋음 단계
  {
    colorPrint(0,0,255); //파란색 점등
  }
  u8g.firstPage(); //picture loop의 시작
  do {
    u8g.setFont(u8g_font_fub14); //온도, 습도 폰트지정
    u8g.setPrintPos(5, 20); //온도 데이터 출력 커서 설정
    u8g.print(temperature,1); //온도 데이터(소수점 첫째 자리) 출력
    u8g.print("\xb0""C"); //온도 기호(°C) 출력 
    u8g.setPrintPos(70, 20); //습도 데이터 출력 커서 설정
    u8g.print(humidity,1); //습도 데이터(소수점 첫째 자리) 출력
    u8g.print("%"); //습도 기호(%) 출력
    u8g.setFont(u8g_font_fub20); //미세먼지 농도 수치 폰트지정
    u8g.setPrintPos(40, 55); //미세먼지 농도 수치 출력 커서 설정
    u8g.print(avgDustDensity,1); //미세먼지 농도 수치(소수점 첫째 자리) 출력
  } while(u8g.nextPage()); //picture loop의 끝
  delay(1000);  //1초 대기
}
void colorPrint(int red, int green, int blue) //3색 LED 색깔 출력 함수 정의
{
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}
