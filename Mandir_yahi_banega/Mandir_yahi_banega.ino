#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11);

int Relay = 9;
int RBUTT = 3;
int TIMER_BUTT = 13;
int RELAYSTATE = LOW;
int Rbuttonstate=LOW;
int buttonstate=LOW;
int lastButtonState = LOW;
int MlastButtonState = LOW;
int timer = 900000;
char BINPUT = 0;
char Incoming_value = 0;

unsigned long lastDebounceTime = 0;
unsigned long MlastDebounceTime = 0;
unsigned long debounceDelay = 50;
void setup() {
  Serial.begin(9600);
 pinMode(Relay, OUTPUT);
 pinMode(RBUTT, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(RBUTT), blink, HIGH);
 pinMode(TIMER_BUTT, INPUT);
 mySerial.begin(9600);
}

void loop() {

  

  int reading = digitalRead(TIMER_BUTT);
  if(reading != MlastButtonState)
  {
    lastDebounceTime = millis();
  }
  
  if((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != Rbuttonstate) 
    {Rbuttonstate = reading;

      // only toggle the LED if the new button state is HIGH
      if (Rbuttonstate == HIGH) {
        lastButtonState = !lastButtonState;
        long long test = millis();
        if(lastButtonState == HIGH)
        {
          
          while(millis() - test < timer)
          {
            digitalWrite(Relay,HIGH);
            if(lastButtonState == LOW)
            {
              break;
            }
          }
          digitalWrite(Relay,LOW);
          lastButtonState = !lastButtonState;
        }
        
      }
    } 
  }


  if(Serial.available()>0)
  {
    Incoming_value = mySerial.read();
    if(Incoming_value == '1')  
    {
      lastButtonState = !lastButtonState;
      digitalWrite(Relay,lastButtonState); 
    }
    if(Incoming_value == '0')  
    {//Checks whether value of Incoming_value is equal to 0
      lastButtonState = !lastButtonState;
      long long test = millis();
      while(millis() - test < timer)
      {
            digitalWrite(Relay,HIGH);
            if(lastButtonState == LOW)
            {
              break;
            }
       }
          digitalWrite(Relay,LOW);
          lastButtonState = !lastButtonState;
    }
    else
    {
      timer = int(Incoming_value);
      timer = timer*60000;
    }
  }
}

void blink() {
  int reading = digitalRead(RBUTT);
  if(reading != MlastButtonState)
  {
    MlastDebounceTime = millis();
  }

  
  if((millis() - MlastDebounceTime) > debounceDelay)
  {
    if (reading != buttonstate) 
    {buttonstate = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonstate == HIGH) {
        MlastButtonState = !MlastButtonState;
        digitalWrite(Relay, lastButtonState);
        Serial.println("RELAY CHANGED");
      }
    }
  }
}
