#include <Arduino.h>
#include "max6675.h"
#include "ModbusRtu.h"

// data array for modbus network sharing
uint16_t au16data[16] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, -1
};

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  u8serno : serial port (use 0 for Serial)
 *  u8txenpin : 0 for RS-232 and USB-FTDI 
 *               or any pin number > 1 for RS-485
 */
Modbus slave(1, 0, 0);  // this is slave @1 and RS-232 or USB-FTDI

int thermoDO = 19;
int thermoCLK = 18;
int thermoCS = 23;
int thermoCS2 = 5;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MAX6675 thermocouple2(thermoCLK, thermoCS2, thermoDO);

#define LED 2

void setup() {
  slave.begin(19200);  //, SERIAL_8E1 ); // 19200 baud, 8-bits, even, 1-bit stop
  //Serial.begin(115200);

  Serial.println("MAX6675 test");

  pinMode(LED, OUTPUT);

  // Start Program Check LED
  for (int i = 10; i <= 0; i--) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
  }

  // wait for MAX chip to stabilize
  delay(500);
}

/*unsigned long lastTempCheck = 0;
bool checkA = true;*/

void loop() {
  /*Serial.print("0.00,");
  Serial.print(thermocouple.readCelsius());
  Serial.print(",");
  Serial.print(thermocouple2.readCelsius());
  Serial.println(",0.00,0.00");*/

  // basic readout test, just print the current temp
  /* if (lastTempCheck + 500 < millis()) {
      //time to check your temp
      if (checkA) {
        au16data[2] = ((uint16_t) thermocouple.readCelsius()*100);
      } else {
        au16data[4] = ((uint16_t) thermocouple2.readCelsius()*100);
      }

       Serial.print("C = ");  
       Serial.print(au16data[2]);
       Serial.print(" ");
       Serial.print(au16data[4]);
       Serial.print(" ");
       Serial.println(au16data[6]);

      lastTempCheck = millis();
      checkA = !checkA;
    }

   
   slave.poll( au16data, 16 );

   for(int i=1; i<=99; i++) {
    if(i<=au16data[4]){
      digitalWrite(LED, HIGH);
    }else{
      digitalWrite(LED, LOW);
    }
    
    delay(5);
   }*/

  //write current thermocouple value
  au16data[2] = (uint16_t)(thermocouple.readCelsius() * 100);
  au16data[3] = (uint16_t)(thermocouple2.readCelsius() * 100);

  //poll modbus registers
  slave.poll(au16data, 16);

  /*for (int i = 1; i <= 99; i++) {
    if (i <= au16data[4])
      digitalWrite(LED, HIGH);
    else
      digitalWrite(LED, LOW);

    delay(5);
  }*/

  Serial.print("C = ");
  Serial.print(thermocouple.readCelsius());
  Serial.print(" : ");
  Serial.print(au16data[2]);
  Serial.print(" // C2 = ");
  Serial.print(thermocouple2.readCelsius());
  Serial.print(" : ");
  Serial.println(au16data[3]);


  // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
  //delay(1000);
  digitalWrite(LED, HIGH);
  delay(150);
  digitalWrite(LED, LOW);
  delay(100);
}