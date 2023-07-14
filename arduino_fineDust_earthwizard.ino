#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int Vo = A0;
int V_LED = 12;
int red = 3;
int yellow = 4;
int green = 5;
int blue = 6;
int piezo = 2;
int notes[] = {523, 587};

float Vo_value = 0;
float Voltage = 0;
float dustDensity = 0;

void setup() {
  Serial.begin(9600); // 시리얼통신 설정
  lcd.init(); // LCD 초기화
  lcd.backlight();
  lcd.clear(); // 이전 출력값 삭제
  lcd.setCursor(0, 0);
  lcd.print("AIR POLLUTION");
  lcd.setCursor(0, 1);
  lcd.print(" - SMWU - ");
  delay(3000); // 3초간 지속

  pinMode(V_LED, OUTPUT); // 출력모드 설정
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(piezo, OUTPUT);
  pinMode(Vo, INPUT); // 입력모드 설정
}

void loop() {
  digitalWrite(V_LED, LOW); // 미세먼지센서의 적외선 LED 켜기. 커패시터의 전원 이용해 공기 중 미세먼지를 적외선 LED로 비춤
  delayMicroseconds(280); // 0.28ms만큼 지속
  Vo_value = analogRead(Vo); // 적외선 수신소자 Vo에서 인식한 미세먼지값을 A0핀에 읽어오기
  delayMicroseconds(40); // 0.04ms만큼 지속
  digitalWrite(V_LED, HIGH); // 적외선 LED 끄기 (커패시터 충전)
  delayMicroseconds(9680); // 9.68ms만큼 기다림.

  Voltage = Vo_value * 5.0 / 1024.0; 
  dustDensity = (Voltage - 0.225) / 0.005; //초기값 설정

  Serial.print("Voltage:");
  Serial.println(Voltage);
  Serial.print("dustDensity:");
  Serial.println(dustDensity);

  if (dustDensity <= 30) { 
    digitalWrite(blue, HIGH); 
    lcd.setCursor(0, 0);
    lcd.print("INFORM: GOOD"); 
    lcd.setCursor(0, 1); 
    lcd.print("DUST: ");
    lcd.print(dustDensity);
  } else {
    digitalWrite(blue, LOW);
  }

  delay(500); //05초간 지속

  if (dustDensity > 30 && dustDensity <= 80) { 
    digitalWrite(green, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("INFORM: Normal"); 
    lcd.setCursor(0, 1);
    lcd.print("DUST: ");
    lcd.print(dustDensity);
  } else {
    digitalWrite(green, LOW);
  }

  delay(500);

  if (dustDensity > 80 && dustDensity <= 150) { // 80 이상 150 이하
    digitalWrite(yellow, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("INFORM: BAD"); // Bad 텍스트 출력
    lcd.setCursor(0, 1);
    lcd.print("DUST: ");
    lcd.print(dustDensity);
  } else {
    digitalWrite(yellow, LOW);
  }

  delay(500);

  if (dustDensity > 150) { 
    digitalWrite(red, HIGH); 
    lcd.setCursor(0, 0);
    lcd.print("INFORM:WORST"); 
    lcd.setCursor(0, 1);
    lcd.print("DUST: ");
    lcd.print(dustDensity);
  } else {
    digitalWrite(red, LOW);
  }

  delay(500);

  if (dustDensity > 150) // 150 초과 시
    for (int i = 0; i < 2; i++) { // i
      tone(piezo, notes[i], 1000);
      delay(1000);
      noTone(piezo);
    }
    delay(1000);
  }

