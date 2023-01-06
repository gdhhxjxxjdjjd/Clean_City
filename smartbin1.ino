#define BLYNK_PRINT Serial
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#define TRIGGERPIN 4
#define ECHOPIN    5
#define TRIGGERPIN1 0
#define ECHOPIN1    2
#define co2pin A0
#define co2zero 55
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "iafcudb7nCWzI8_fjuHvpkPczWtMnYbj";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ALBtelecom-Pellumb";
char pass[] = "19711971";

WidgetLCD lcd(V1);
WidgetLCD lcd1(V0);
WidgetLED led(V2);
WidgetLED led2(V3);
WidgetLED led3(V4);
WidgetLCD lcd2(V5);

Servo myservo;

void setup()
{
  // Debug console
  myservo.attach(16);
  Serial.begin(9600);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(TRIGGERPIN1, OUTPUT);
  pinMode(ECHOPIN1, INPUT);
  pinMode(co2pin, INPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
  
  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(0, 0, "Distance in cm"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  // Please use timed events when LCD printintg in void loop to avoid sending too many commands
  // It will cause a FLOOD Error, and connection will be dropped
  lcd1.clear();
  lcd1.print(0, 0, "Smart Bin 1");

  led.off();
  led2.off();
  led3.off();

}

void loop()
{
  lcd.clear();
  lcd.print(0, 0, "Distance in cm"); 
  
  lcd1.clear();
  lcd1.print(0, 0, "Smart Bin 1");

  lcd2.clear();
  lcd2.print(0, 0, "C02 in PPM");
  
  long duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print(distance);
  Serial.println("Cm");

  long duration1, distance1;
  digitalWrite(TRIGGERPIN1, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN1, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN1, LOW);
  duration1 = pulseIn(ECHOPIN1, HIGH);
  distance1 = (duration1/2) / 29.1;
  Serial.println("Distanca2:");
  Serial.println(distance1); 
int co2now[10];
int co2raw = 0;
int co2ppm = 0;
int zzz = 0;
for (int x=0; x<10; x++){
co2now[x]= analogRead(co2pin);
delay(100);
}
for(int x=0; x<10; x++){
zzz = zzz + co2now[x];
}
co2raw = zzz/10;
co2ppm = co2raw - co2zero;
Serial.print("Air quality=");
Serial.print(co2ppm);

lcd.print(7, 1, distance);
lcd2.print(7, 1, co2ppm);
if(distance > 30){
  led.on();
  led2.off();
  led3.off();
}
  
  if(distance < 30 && distance >= 10){
    led.off();
    led2.on();
    led3.off();
  }
  if(distance < 10){
    led.off();
    led2.off();
    led3.on();
  }

if(distance < 30 && distance >= 10 && distance1 < 10){
myservo.write(130);
delay(8000);
myservo.write(0);
  }   
if(distance > 30 && distance1 < 10){
myservo.write(130);
delay(8000);
myservo.write(0);
  }   
Blynk.run();
delay(10);
}
