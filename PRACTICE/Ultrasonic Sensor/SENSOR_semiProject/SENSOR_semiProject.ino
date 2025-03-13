#include <DHT.h>
#include <DHT_U.h>

#define ECHO 2
#define TRIG 3
#define DHTPIN 4
#define MOTOR 6
#define DHTTYPE DHT11 //온습도 센서 타입 선택

DHT dht(DHTPIN, DHTTYPE); //DHT 센서 초기화

 void setup() {
  // put your setup code here, to run once:
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(MOTOR, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(TRIG, HIGH); //펄스파 발생
  delayMicroseconds(10); 
  digitalWrite(TRIG, LOW);  //펄스파 종료
  float humidity = dht.readHumidity(); //습도 데이터를 읽어와 변수에 저장
  float temperature = dht.readTemperature(); //온도 데이터를 읽어와 변수에 저장
  float di = (float)9/5*temperature-0.55*((float)1-humidity/100)*((float)9/5*temperature-26)+32; //불쾌지수를 계산하여 변수에 저장

  long duration = pulseIn(ECHO, HIGH); 

  if(duration == 0) // 시간이 0 이면 종료
  {
    return;
  }

  long distance = duration / 58; //총 걸리는 시간을 58로 나눠 cm 단위로 거리 측정

  if (isnan(humidity) || isnan(temperature)) { //데이터를 읽어오지 못했을 경우 loop() 함수 출력
  Serial.println("Faild to read from DHT sensor!");
    return;
  }
  if(di >= 75 && distance <= 50){ //불쾌지수 75이상이고 초음파 센서와 사람 간 거리가 50cm이하 일 때 최대 속력
  analogWrite(MOTOR, 255);
}
  else if(di >= 70 && distance <= 50){ //불쾌지수 70이상이고 초음파 센서와 사람 간 거리가 50cm이하 일 때 1/2  속력
  analogWrite(MOTOR, 127);
}
  else{
  analogWrite(MOTOR, 0);
}
  Serial.print("Discomfort Index : ");
  Serial.print(di,1); //소수 첫째 자리까지 불쾌지수 출력
  Serial.print("\tDistance : ");
  Serial.print(distance,1); //소수 첫째 자리까지 거리 출력
  Serial.println("cm");
  delay(2000);
}
