#include "DHT.h" 
#define MOTOR 6 
#define DHTPIN 2 
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE); //DHT 센서 초기화 

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR, OUTPUT); 
  Serial.begin(9600); 
  dht.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000); 
  float humidity = dht.readHumidity(); //습도 데이터를 읽어와 변수에 저장
  float temperature = dht.readTemperature(); //온도 데이터를 읽어와 변수에 저장
  float di = (float)9/5*temperature-0.55*((float)1-humidity/100)*((float)9/5*temperature-26)+32; //불쾌지수를 계산하여 변수에 저장

  if (isnan(humidity) || isnan(temperature)) { //데이터를 읽어오지 못했을 경우 아래 메시지 출력
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if(di >= 70) //불쾌지수 데이터가 70 이상일 때(매우 높음)
  {
    analogWrite(MOTOR, 255); //최대 속도 출력
  }
  else if(di >= 65) //불쾌지수 데이터가 65 ~ 70 미만일 때(높음)
  {
    analogWrite(MOTOR, 170); //최대 속도의 2/3 속도 출력
  }
  else
  {
    analogWrite(MOTOR, 0); //모터 멈춤
  }
  Serial.print("Discomfort Index:");
  Serial.println(di,1); //소수점 첫째자리까지 불쾌지수 출력  
}
