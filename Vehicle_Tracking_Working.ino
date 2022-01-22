
//==============================================
// My libraries
//==============================================
#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>


//==============================================
// Define what pins to use for my softwareserial 
// and the length of my message
//==============================================
#define MESSAGE_LENGTH 160
#define PIN_TX 10
#define PIN_RX 11
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,


//==============================================
// variables that i will goin to use
//==============================================
int messageIndex = 0;
char message[MESSAGE_LENGTH];
char MESSAGE[300];
char lat[12];
char lon[12];
char wspeed[12];
char phone[16];
char datetime[24];


//==============================================
// functions
//==============================================
void sendSMS();
void getGPS();
void readSMS();



//==============================================
// void setup
//==============================================
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  //******** Initialize sim808 module *************
  sim808.init();
  if(!sim808.init()) { 
  Serial.print("Sim808 init error\r\n");
  delay(1000);
  }else{
  Serial.print("Sim808 Initialized\r\n");
  delay(1000);    
    }
  //************* Turn on the GPS power************
  sim808.attachGPS();
  if(!sim808.attachGPS())
      Serial.println("Open the GPS power Failure");
  else 
      Serial.println("Open the GPS power Success");      
}



//==============================================
// void loop
//==============================================
void loop()
{
   //*********** Detecting unread SMS ************************
   messageIndex = sim808.isSMSunread();
   //*********** At least, there s one UNREAD SMS ***********
  if (messageIndex < 0)
  { 
    Serial.println("Please send SMS message to me!");
  }else{
    readSMS();
    getGPS();
    sendSMS();
    }
   delay (5000);
}



//====================================
// Method for readSMS()
//====================================

void readSMS()
{
  sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);          
  Serial.print("From number: ");
  Serial.println(phone);  
  Serial.print("Datetime: ");
  Serial.println(datetime);        
  Serial.print("Recieved Message: ");
  Serial.println(message);
  //***********In order not to full SIM Memory, is better to delete it**********
  sim808.deleteSMS(messageIndex);
}


//====================================
// Method for getGPS()
//====================================
void getGPS()
{ 
  sim808.attachGPS();
  float la = sim808.GPSdata.lat;
  float lo = sim808.GPSdata.lon;
  float ws = sim808.GPSdata.speed_kph;
  dtostrf(la, 4, 6, lat); //put float value of la into char array of lat. 6 = number of digits before decimal sign. 2 = number of digits after the decimal sign.
  dtostrf(lo, 4, 6, lon); //put float value of lo into char array of lon
  dtostrf(ws, 6, 2, wspeed);  //put float value of ws into char array of wspeed
  sprintf(MESSAGE, "Latitude : \n%s\nLongitude : \n%s\nWind Speed : \n%s kph \nhttp://maps.google.com/maps?q=%s,\n%s\n", lat, lon, wspeed, lat, lon);
  sim808.detachGPS();
  
}



//====================================
// Method for sendSMS()
//====================================
void sendSMS()
{
  Serial.println("Start to send message ...");
  Serial.println(MESSAGE);
  Serial.println(phone);
  sim808.sendSMS(phone,MESSAGE);
  phone[16] = "639399150254";
}

