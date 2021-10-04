#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial gsm(2, 3);
String msg;
char call;
char val;
char alert;
float gpslat, gpslon;
TinyGPS gps;
short rec = -1;
int buttonState = 0;
const int pin = 12;
int led = 13;


void setup()
{
  gsm.begin(9600);
  Serial.begin(9600);
  gsm.println("AT");
  gsm.println("AT+CMGF=1");
  gsm.println("AT+CNMI=2,2,0,0,0");
}

void loop()
{
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (5.0 / 1023.0);
  buttonState = digitalRead(pin);


  while (Serial.available())
  {
    int c = Serial.read();
    if (gps.encode(c))
    {
      gps.f_get_position(&gpslat, &gpslon);
    }
  }

  String msg;
  while (gsm.available() > 0)
  {
    val = gsm.read();
    msg.concat(val);
    rec = msg.indexOf("Get location");

  }
  if (rec != -1)
  {
    SendMessage();
    rec = -1;
  }
  if (voltage > 4)
  {
    MakeCall();
    SendMessage();
  }
  else
  {
    digitalWrite(led, LOW);
  }
}

void SendMessage()
{
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+91************\"\r");
  delay(1000);
  gsm.print("https://www.google.com/maps/?q=");
  gsm.print(gpslat, 6);
  gsm.print(",");
  gsm.print(gpslon, 6);
  delay(100);
  gsm.println((char)26);
  delay(1000);
}

void MakeCall()
{
  gsm.println("ATD+91***********");
  delay(1000);
}
