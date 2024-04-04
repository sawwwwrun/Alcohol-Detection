#define alc A0
#define motor 6
#include <LCD_I2C.h>
LCD_I2C lcd(0x27, 16, 2);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
SoftwareSerial GPS_SoftSerial(4, 5);
#include <TinyGPS++.h>
TinyGPSPlus gps;
int alcvalue;
int motorvalue;
volatile float minutes, seconds;
volatile int degree, secs, mins;
float latn, longn;
double lat_val, lng_val, alt_m_val;
uint8_t hr_val, min_val, sec_val;
bool loc_valid, alt_valid, time_valid;
String s = "www.google.com/maps/dir/";
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
mySerial.begin(9600);
GPS_SoftSerial.begin(9600);
lcd.begin();
lcd.backlight();
pinMode(alc, INPUT);
pinMode( motor, OUTPUT);
}
void loop() {
gpsread();
alcvalue = digitalRead(alc);
if (alcvalue == 1)
{
digitalWrite(motor, HIGH);
SendMessage();
}
else {
digitalWrite(motor, LOW);
}
delay(300);
lcd.setCursor(0, 0);
lcd.print("ALC: " );
if (alcvalue == 1) {
lcd.print("DET");
}
else {
lcd.print("NOT");
}
lcd.setCursor(13, 0);
delay(1000);
}
void SendMessage()
{
Serial.println("msg sending...");
mySerial.println("AT+CMGF=1"); 
//Sets the GSM Module in Text Mode
delay(2000); // Delay of 1000 milli 
seconds or 1 second
mySerial.println("AT+CMGS=\"+9193617
75665\"\r"); // Replace x with mobile 
number
delay(2000);
mySerial.println("ALCOHOL 
DETECTED ");
mySerial.println("USER SARAN");
mySerial.println("TN37DL5743");
mySerial.print("LOCATION :");
mySerial.println(s);
delay(100);
mySerial.println((char)26);// ASCII code 
of CTRL+Z
delay(4000);
}
void gpsread()
{
smartDelay(200); /* Generate precise 
delay of 1ms */
unsigned long start;
lat_val = gps.location.lat(); /* Get latitude 
data */
loc_valid = gps.location.isValid(); /* 
Check if valid location data is available */
lng_val = gps.location.lng(); /* Get 
longtitude data */
alt_m_val = gps.altitude.meters(); /* Get 
altitude data in meters */
alt_valid = gps.altitude.isValid(); /* 
Check if valid altitude data is available */
hr_val = gps.time.hour(); /* Get hour */
min_val = gps.time.minute(); /* Get 
minutes */
sec_val = gps.time.second(); /* Get 
seconds */
time_valid = gps.time.isValid(); /* Check 
if valid time data is available */
if (!loc_valid)
{
Serial.print("Latitude : ");
Serial.println("*");
Serial.print("Longitude : ");
Serial.println("*");
}
else
{
DegMinSec(lat_val);
DegMinSec(lng_val); /* Convert the 
decimal degree value into degrees minutes 
qseconds form */
longn = (lng_val, 6);
}
if (!alt_valid)
{
}
else
{
}
if (!time_valid)
{
}
else
{
char time_string[32];
sprintf(time_string, "Time : 
%02d/%02d/%02d \n", hr_val, min_val, 
sec_val);
Serial.print(time_string);
}
if (!alt_valid) {
lat_val = 11.060979;
lng_val = 77.0339173;
}
s = "www.google.com/maps/dir/";
s += String(lat_val, 6);
s += ",";
s += String(lng_val, 6);
s += "/";
}
static void smartDelay(unsigned long ms)
{
unsigned long start = millis();
do
{
while (GPS_SoftSerial.available()) /* 
Encode data read from GPS while data is 
available on serial port */
gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the 
string received by the GPS and to store it in 
a buffer so that information can be 
extracted from it */
} while (millis() - start < ms);
}
void DegMinSec( double tot_val) /* 
Convert data in decimal degrees into 
degrees minutes seconds form */
{
degree = (int)tot_val;
minutes = tot_val - degree;
seconds = 60 * minutes;
minutes = (int)seconds;
mins = (int)minutes;
seconds = seconds - minutes;
seconds = 60 * seconds;
secs = (int)seconds;
}