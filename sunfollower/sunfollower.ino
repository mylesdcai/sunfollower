
/*
 * Sunfollower 
 * Myles Cai
 * 6/13/18
 * 
 * Tracking light with one LDR and two servos. 
 * 
 */

 
#include <Servo.h>
Servo base;
Servo arm;

int basepos = 0;
int armpos = 0;
int darkness;
int peak = 1023;
int basepeak = 0;
int armpeak = 0;
boolean rev = false;

void setup() {
  Serial.begin(9600);

  base.attach(9);
  arm.attach(10);
  DDRD |= 1 << 3;
  PORTD |= 1 << 3;

}

void loop() {
  // two for loops to hit all points on a hemisphere as coverable by the device 
  for (basepos = 0; basepos <= 180; basepos += 20){

    base.write(basepos);
    delay(50);
    
    for (armpos = 30; armpos <= 150; armpos+=2){

      if((basepos/20) % 2 == 1){
        arm.write(180 - armpos);
        rev = true;
      } else {
        arm.write(armpos);
        rev = false;
      }
       
      delay(20);
      
      int darkness = analogRead(A0);
      // Serial.println(darkness); // range is from 10 (bright) to 950 (dark) or so
      delay(20);

      if(darkness < peak){ 
        peak = darkness;
        
        if (rev) {
          armpeak = 180 - armpos;
        } else { armpeak = armpos;}
        
        basepeak = basepos;

        /*
        Serial.print("new brightest: ");
        Serial.print(darkness);
        Serial.print(" at: ");
        Serial.print(basepeak);
        Serial.print(" , ");
        Serial.println(armpeak);
        */
        }
    }
  }

//  Serial.println(basepeak);
//  Serial.println(armpeak);
  delay(500);

  base.write(basepeak);
  arm.write(armpeak);
  delay(200);
  
  PORTD &= ~(1 << 3);
  for (int i = 0; i < 15; i++) {
     delay(65535);
  }

  // reset variables
  PORTD |= 1 << 3;
  peak = 1023;

  // reset position
  while (basepos > 0 || armpos > 30){
    for (basepos = basepeak; basepos > 0; basepos-=5) { 
      // in steps of 1 degree
      base.write(basepos);             
      delay(50);                       
    }
    for (armpos = armpeak; armpos > 30; armpos-=5) { 
      arm.write(armpos);              
      delay(50);                       
    }
  }

  delay(500);
  
}
