//Written By Nikodem Bartnik - nikodembartnik.pl
//Code for NodeMcu and 3d printed Turntable
//Blynk IOT to add

#define STEPPER_PIN_1 D1  //9
#define STEPPER_PIN_2 D2  //10
#define STEPPER_PIN_3 D3  //11
#define STEPPER_PIN_4 D4  //12

#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL5A5B40FL7"
#define BLYNK_TEMPLATE_NAME "display base"

#define BLYNK_AUTH_TOKEN "HKWC-qwjkFajxVro6BUKteD62TvApwOJ"

#include <ESP8266WiFi.h>  // Change for different WiFi module
#include <BlynkSimpleEsp8266.h>  // Use appropriate library for your board

#include <BleKeyboard.h>

BleKeyboard bleKeyboard("ESP BLE", "lilly", 100);  // Device name, manufacturer, battery level

#include <Stepper.h>

// 28BYJ-48 has 64 steps per revolution * 64 gear reduction = 4096 total steps per 360°
const int stepsPerRevolution = 4096;

// 3.6 degrees corresponds to: (3.6 / 360) * 4096 = 41 steps
const int stepSize = 41;

Stepper stepper(stepsPerRevolution, 8, 10, 9, 11); // IN1, IN3, IN2, IN4

char ssid[] = "EE-53WRTK";
char pass[] = "3xkMhteKpKrLDPCf";

BlynkTimer timer;

int step_number = 0;
int Blynk_Start = 0;

void setup() {

Serial.begin(115200);
 // while the serial stream is not open, do nothin
 while (!Serial) ;
 

Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
//timer.setInterval(1000L, myTimerEvent);

pinMode(STEPPER_PIN_1, OUTPUT);
pinMode(STEPPER_PIN_2, OUTPUT);
pinMode(STEPPER_PIN_3, OUTPUT);
pinMode(STEPPER_PIN_4, OUTPUT);

//Start Bluetooth
bleKeyboard.begin();

}

BLYNK_WRITE(V0) {
  
  int buttonState = param.asInt();  // Read button state (1 = ON, 0 = OFF);

  Blynk_Start = buttonState;

  Serial.println("Inside Blynk write");
  Serial.println(buttonState);
  

  }

void loop() {
  
  //if  statement here for button press
  
   for (int i = 0; i < 360 / 3.6; i++) {
    stepper.step(stepSize);
    // two pauses allows movement and photo to be taken
    delay (1000)
        if (bleKeyboard.isConnected()) {
          Serial.println("Taking Photo");  //Up button needs to go here
          bleKeyboard.print("Hello World!");
          delay(5000);  // Wait 5 seconds before sending again
      } else {
    
    Serial.println("Waiting for connection...");
      }
    
      delay(1000); // Wait 1 second
  
  //end if  button press
  /* old blynk code
  Blynk.run();

              //  OneStep(false,1);


 OneStep(false,Blynk_Start);
 
 //OneStep(false,0);


  delay(2);
*/
}


void OneStep(bool dir,int StrtStop){  //strtstop  starts or stops the motor
   

Serial.println(StrtStop);
Serial.println("");
//Serial.println(dir);
  
  
   if(StrtStop == 0){

  //  Serial.println("motor stopped");

      digitalWrite(STEPPER_PIN_1, LOW);
      digitalWrite(STEPPER_PIN_2, LOW);
      digitalWrite(STEPPER_PIN_3, LOW);
      digitalWrite(STEPPER_PIN_4, LOW);

   }
   else{
             
        //      Serial.println("motor started");
  
          switch(step_number){
            case 0:
            digitalWrite(STEPPER_PIN_1, HIGH);
            digitalWrite(STEPPER_PIN_2, LOW);
            digitalWrite(STEPPER_PIN_3, LOW);
            digitalWrite(STEPPER_PIN_4, LOW);
            break;
            case 1:
            digitalWrite(STEPPER_PIN_1, LOW);
            digitalWrite(STEPPER_PIN_2, HIGH);
            digitalWrite(STEPPER_PIN_3, LOW);
            digitalWrite(STEPPER_PIN_4, LOW);
            break;
            case 2:
            digitalWrite(STEPPER_PIN_1, LOW);
            digitalWrite(STEPPER_PIN_2, LOW);
            digitalWrite(STEPPER_PIN_3, HIGH);
            digitalWrite(STEPPER_PIN_4, LOW);
            break;
            case 3:
            digitalWrite(STEPPER_PIN_1, LOW);
            digitalWrite(STEPPER_PIN_2, LOW);
            digitalWrite(STEPPER_PIN_3, LOW);
            digitalWrite(STEPPER_PIN_4, HIGH);
            break;
          } 
        }
       
           

   

step_number++;
  if(step_number > 3){
    step_number = 0;
  }
}


// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}