// C++ code
//

#include <Adafruit_LiquidCrystal.h>
#define TempSensor A0 
#define WaterSensor A1 
#define SalinitySensor A2 
#define CompRelay 3
#define StatusLED1 12 
#define StatusLED2 13 

//LED Relay switching precode:
const int PushButton1 = 2;    // the number of the pushbutton pin
const int LEDRelay = 4;      // the number of the LED relay
int LEDRelayState = HIGH;         // the current state of the LED relay output pin
int PushButton1State;             // the current reading from the input pin
int lastPushButton1State = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

Adafruit_LiquidCrystal lcd_1(0); 

void setup()  {
  Serial.begin(9600);  //serial comms initialise
  pinMode(TempSensor, INPUT);  //temperature sensor input
  pinMode(WaterSensor, INPUT);  //water sensor input
  pinMode(SalinitySensor, INPUT);  //salinity sensor input
  pinMode(CompRelay, OUTPUT);  //compressor relay output
  pinMode(12, OUTPUT); //Status LED 1 output
  pinMode(13, OUTPUT); //Status LED 2 output
  
  lcd_1.begin(16, 2);   //constrain LCD display
  lcd_1.print("Temp:"); //display "Temp:"
  
  //LED Relay switching setup code:
  pinMode(PushButton1, INPUT); //push button 1 input
  pinMode(LEDRelay, OUTPUT);   //LED relay output
  digitalWrite(LEDRelay, LEDRelayState); // set initial LED state
 
 }

void loop()  {
  
  int tempreading = analogRead(TempSensor); //read from temperature sensor
  float tempvoltage = tempreading * (5000 / 1024.); //convert to voltage
  float temperature = (tempvoltage - 500) / 10; //convert to temperature
  
  int waterreading = analogRead(WaterSensor); //read from water sensor
  int salinityreading = analogRead(SalinitySensor); //read from salinity sensor
  
  
  lcd_1.setCursor(6, 0);						//set cursor to after "Temp:"
  lcd_1.print(temperature);
  lcd_1.println("\262C   "); //print temperature with degree symbol
  
  if (temperature > 9) {
    digitalWrite(CompRelay, HIGH);
  }								//turn on compressor above value H1 to cool water
  
  if (temperature < 7) {
    digitalWrite(CompRelay, LOW);
  }								//turn off compressor below value C1
  
  
  if (temperature > 15)  {
    lcd_1.setCursor(14, 0);
    lcd_1.print("HI");
    digitalWrite(12, HIGH);
    
     /** What this code doing? did you forget to copy over the contents?
     for (int thisNote = 0; thisNote < 11; thisNote++) {

    } 	*/						//turn on high temperature alarm above value H2
  
  }
  
  if (temperature < 4)  {
    lcd_1.setCursor(14, 0);
    lcd_1.print("LO");
    digitalWrite(13, HIGH);
  
  }								//turn on low temperature alarm below value C2
 

  if (temperature > 4 && temperature < 15)  {
    lcd_1.setCursor(14, 0);
    lcd_1.print("  ");
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
  }								//cancel high/low temperature alarms within values C1-H1

  if (waterreading < 1)  {
    lcd_1.setCursor(0, 1);
    lcd_1.print("WATER LOW");
   
  }								//turn on low water alarm below value W1
  
  if (waterreading > 1)  {
    lcd_1.setCursor(0, 1);
    lcd_1.print("         ");

  }								//cancel low water alarm above value W1
  
  if (salinityreading > 700)  {
    lcd_1.setCursor(10, 1);
    lcd_1.print("HISALT");

  }								//turn on high salinity alarm above value S1
  
  if (salinityreading < 400)  {
    lcd_1.setCursor(10, 1);
    lcd_1.print("LOSALT");

  }								//turn on low salinity alarm below value S2
  
  if (salinityreading > 400 && salinityreading < 700)  {
    lcd_1.setCursor(10, 1);
    lcd_1.print("      ");
    
  }								//cancel high/low salinity alarm within values S1-S2
   
  
  //LED relay switching code:
  int reading = digitalRead(PushButton1); // read the state of the switch into a local variable:
  if (reading != lastPushButton1State) {
    
    lastDebounceTime = millis();
  }  								// reset the debouncing timer

  if ((millis() - lastDebounceTime) > debounceDelay) {
    
    if (reading != PushButton1State) {
      PushButton1State = reading; 		// if the button state has changed
      
      if (PushButton1State == HIGH) {
        LEDRelayState = !LEDRelayState;
      }  							// only toggle the LED if the new button state is HIGH
    }
  }

  // set the LED:
  digitalWrite(LEDRelay, LEDRelayState); 
  lastPushButton1State = reading; // save the reading as lastButtonState:

  
   
  
} //final coda
