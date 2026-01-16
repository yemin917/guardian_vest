#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID   "TMPL6C6B1yk2y"
#define BLYNK_TEMPLATE_NAME "Guardian Vest"
#define BLYNK_AUTH_TOKEN    "vVIu2O-EdcwzPa_ZNxQGhoFbn0mAkKEU"

#include <SPI.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>

// WiFi 접속 정보
char ssid[] = "yemin";                      
char pass[] = "123212321";                        

// Blynk 이벤트 코드
const char* EVENT_CODE = "warning_notice"; 

// 알림 중복 전송 방지를 위한 플래그
bool alertSent = false; 
// ============== Blynk & WiFi 설정 끝 ==============


// ============== 기존 성공 코드 시작 ==============
#include <Adafruit_NeoPixel.h>
#include<Wire.h>
#include <MPU6050.h>

#define TRIG_PIN_L 13
#define ECHO_PIN_L 12
#define TRIG_PIN_R 4
#define ECHO_PIN_R 7
#define TRIG_PIN_C 10
#define ECHO_PIN_C 11

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  long distance = duration * 0.0343 / 2;

  return distance;
}

#define LED_PIN_L 3
#define LED_PIN_R 9
#define NUM_LEDS 1
#define VIB_MOTOR_PIN_L 6
#define VIB_MOTOR_PIN_R 5
#define BUZZER_PIN 8
#define RELAY_PIN 2

Adafruit_NeoPixel strip1(NUM_LEDS, LED_PIN_L, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(NUM_LEDS, LED_PIN_R, NEO_GRB + NEO_KHZ800);

MPU6050 mpu(0x68);

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() {
  Serial.begin(9600);

  // ===================== Blynk 연결 로직 =====================
  Serial.println("Blynk 연결 시도...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // =============================================================

  pinMode(TRIG_PIN_L, OUTPUT);
  pinMode(ECHO_PIN_L, INPUT);
  pinMode(TRIG_PIN_R, OUTPUT);
  pinMode(ECHO_PIN_R, INPUT);
  pinMode(TRIG_PIN_C, OUTPUT);
  pinMode(ECHO_PIN_C, INPUT);
  pinMode(VIB_MOTOR_PIN_L, OUTPUT);
  pinMode(VIB_MOTOR_PIN_R, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW); // 릴레이 초기화

  analogWrite(VIB_MOTOR_PIN_L, 0);
  analogWrite(VIB_MOTOR_PIN_R, 0);
  
  strip1.begin();
  strip2.begin();
  strip1.show(); 
  strip2.show(); // LED 모두 끄기

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
  Blynk.run(); // Blynk 연결 유지 및 통신 처리

  long d1 = readDistance(TRIG_PIN_L, ECHO_PIN_L);
  long d2 = readDistance(TRIG_PIN_R, ECHO_PIN_R);
  long d3 = readDistance(TRIG_PIN_C, ECHO_PIN_C);

  Serial.print("Sensor 1: "); Serial.print(d1); Serial.println(" cm");
  Serial.print("Sensor 2: "); Serial.print(d2); Serial.println(" cm");
  Serial.print("Sensor 3: "); Serial.print(d3); Serial.println(" cm");

  //100cm 미만이면 LED on, 진동모터 on, 부저 on
  if (d1 > 0 && d1 <= 100) {
    strip1.setPixelColor(0, strip1.Color(255, 0, 0));
    analogWrite(VIB_MOTOR_PIN_L, 255);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    strip1.setPixelColor(0, 0, 0, 0);
    analogWrite(VIB_MOTOR_PIN_L, 0);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (d2 > 0 && d2 <= 100) {
    strip2.setPixelColor(0, strip2.Color(255, 0, 0));
    analogWrite(VIB_MOTOR_PIN_R, 255);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    strip2.setPixelColor(0, 0, 0, 0);
    analogWrite(VIB_MOTOR_PIN_R, 0);
    digitalWrite(BUZZER_PIN, LOW);
  }

  if (d3 > 0 && d3 <= 100) {
    strip1.setPixelColor(0, strip1.Color(255, 0, 0));
    strip2.setPixelColor(0, strip2.Color(255, 0, 0));
    analogWrite(VIB_MOTOR_PIN_L, 255);
    analogWrite(VIB_MOTOR_PIN_R, 255);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  
  strip1.show();
  strip2.show();


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

  
  // ===================== 센서 데이터 전송 로직 (V0, V1) =====================
  if (Blynk.connected()) {
      // V0: Accel Z 값 전송 (History Graph용)
      Blynk.virtualWrite(V0, AcZ); 
      // V1: Distance 값 전송
  }
  // ========================================================================

 //낙상 감지 (IMU)
  int absAcX = abs(AcX);
  int absAcY = abs(AcY);
  int absAcZ = abs(AcZ);

  if ((AcZ < -8000) && (AcY > -8000)) {  
    Serial.println("⚠️ 낙상 감지!");
    strip1.setPixelColor(0, strip1.Color(255, 255, 0));
    strip2.setPixelColor(0, strip2.Color(255, 255, 0)); // 노란색 LED 경고
    strip1.show();
    strip2.show();
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(RELAY_PIN, HIGH);

   // ===================== 릴레이 작동 시 Blynk 알림 전송 로직 =====================
    if (Blynk.connected()) {
        // V2: Fall Status = 1 (감지)
        Blynk.virtualWrite(V2, 1);
        // V3: Relay Status = 1 (ON)
        Blynk.virtualWrite(V3, 1); 

        if (!alertSent) {
          Blynk.logEvent(EVENT_CODE, "긴급경고! 사용자의 에어백이 작동되었습니다");
          Serial.println(">>> Blynk 긴급 알림 전송 완료.");
          alertSent = true; 
        }
    }
    // =========================================================================

  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(RELAY_PIN, LOW);

  // 릴레이가 꺼지면 알림 플래그 초기화
    alertSent = false;
  }

// ===================== 정상 상태 및 릴레이 OFF 전송 (V2, V3) =====================
    if (Blynk.connected()) {
        Blynk.virtualWrite(V2, 0); // V2: Fall Status = 0 (정상)
        Blynk.virtualWrite(V3, 0); // V3: Relay Status = 0 (OFF)
    }
    // ==============================================================================

  delay(700);
}
