#include <Adafruit_NeoPixel.h>
#include<Wire.h>
#include <MPU6050.h>

#define TRIG_PIN 13
#define ECHO_PIN 12
#define LED_PIN 3
#define NUM_LEDS 1

int v;      
int vmax;  
int Sigpin = A0 ; 

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

MPU6050 mpu(0x68);

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode (Sigpin , INPUT);
  strip.begin();
  strip.show(); // LED 모두 끄기
  Wire.begin();
  mpu.initialize();
  if(mpu.testConnection()){
    Serial.println("MPU6050 연결 성공");
  }else{
    Serial.println("MPU6050 연결 실패");
  }
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

void loop() {
  초음파 거리 측정
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  float distance = 0.017 * duration; 

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 0 && distance <= 100) {
    strip.setPixelColor(0, strip.Color(255, 0, 0));
  } else {
    strip.setPixelColor(0, 0, 0, 0);
  }
  strip.show();

  
  //TRM-121A
  unsigned  long T;          // 주기
  double f;                 // 주파수 
  char s [ 20 ];     
  vmax = 0 ;
  while (digitalRead (Sigpin)); 
  while ( ! digitalRead (Sigpin));
 
  T = pulseIn (Sigpin , HIGH) + pulseIn (Sigpin , LOW); 
  f = 1 / ( double ) T;         
  v = int ((f * 1e6 ) /900.0 );     
  vmax = max (v, vmax); 
  if(vmax >= 1000) 
    vmax = 0;
  else
    sprintf (s, "% 3d km / h" , vmax);
  Serial.println (s); 


  //IMU MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read(); 
  Tmp=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);

  delay(700);
}