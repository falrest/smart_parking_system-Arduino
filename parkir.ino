#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gate;

//================ PIN =================
#define IR_MASUK   2
#define IR_KELUAR  3
#define SERVO_PIN  4

//================ DATA =================
const byte totalSlot = 5;
byte slotTersisa = totalSlot;

// Servo
const byte POS_TUTUP = 0;
const byte POS_BUKA  = 90;

byte servoPos = POS_TUTUP;

// Timer
unsigned long servoTimer = 0;
unsigned long sensorTimer = 0;

const unsigned long gateOpenTime = 3000;
const unsigned long debounceTime = 500;

// Status
bool gateOpen = false;
bool lcdNeedUpdate = true;

void setup() {

  pinMode(IR_MASUK, INPUT_PULLUP);
  pinMode(IR_KELUAR, INPUT_PULLUP);

  gate.attach(SERVO_PIN);
  gate.write(POS_TUTUP);

  lcd.init();
  lcd.backlight();

  tampilStatus();
}

void loop() {

  bacaSensor();

  kontrolServo();

  if(lcdNeedUpdate){
      tampilStatus();
      lcdNeedUpdate=false;
  }

}

//================ SENSOR =================

void bacaSensor(){

  if(millis()-sensorTimer<debounceTime) return;

  if(digitalRead(IR_MASUK)==LOW){

      sensorTimer=millis();

      if(slotTersisa>0){

          slotTersisa--;

          tampilPesan("Silahkan","Masuk");

          bukaGate();

      }else{

          tampilPesan("PARKIR","PENUH");

      }

      lcdNeedUpdate=true;
  }

  if(digitalRead(IR_KELUAR)==LOW){

      sensorTimer=millis();

      if(slotTersisa<totalSlot){

          slotTersisa++;

          tampilPesan("Terima","Kasih");

          bukaGate();

      }

      lcdNeedUpdate=true;
  }

}

//================ SERVO =================

void bukaGate(){

    gate.write(POS_BUKA);

    gateOpen=true;

    servoTimer=millis();

}

void kontrolServo(){

    if(gateOpen){

        if(millis()-servoTimer>=gateOpenTime){

            gate.write(POS_TUTUP);

            gateOpen=false;

        }

    }

}

//================ LCD =================

void tampilStatus(){

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("Slot:");

    lcd.print(slotTersisa);

    lcd.print("/");

    lcd.print(totalSlot);

    lcd.setCursor(0,1);

    if(slotTersisa==0)
        lcd.print("PARKIR PENUH ");
    else
        lcd.print("Silakan Masuk");

}

void tampilPesan(String a,String b){

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(a);

    lcd.setCursor(0,1);
    lcd.print(b);

}
