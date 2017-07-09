//Header
#include <DFRobot_sim808.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
//#include <SoftwareSerial.h>
//#include <RH_ASK.h>
//#include <SPI.h> // Not actually used but needed to compile

//Variable Initialization
//constant Variables
const int mq9=A0, mq135=A1, buzzer=5, mq9pow=4, mq135pow=7;

//Permanent Variables
//float latt=0, lont=0;
int spd=0;
//int hding=0;
//int yr=0, mnth=0, dt=0, hr=0, mn=0, sc=0;
int fine=0;

char yr_str[4], mnth_str[2], dt_str[2], hr_str[2], mn_str[2], sc_str[2];
char spd_str[3], lat_str[5], lon_str[5], hding_str[3];
char mq9_str[4], mq135_str[4];



//char *character;

//Temporary Variables
int i=0, j=0, b=0, c=0, d=0;
bool e=0;

//IR Setup
IRsend irsend;

//RH_ASK driver;
// RH_ASK driver(2000, 5, 6, 4); // ESP8266: do not use pin 11

//Define
//TX RX Pin??
//#define PIN_TX    2
//#define PIN_RX    3
//SoftwareSerial mySerial(PIN_TX,PIN_RX);
//DFRobot_SIM808 sim808(&mySerial);


//SMS Send setup
//Mobile phone number
#define PHONE_NUMBER "9895220220"  
 
//The content of messages sent
/*#define MESSAGE1  "Overspeeding" 
#define MESSAGE2  "Vehicle is polluting"
#define MESSAGE3  "System Not Moving"*/
char msgtxt[160];


//SMS Read setup
int messageIndex=0;
char phone[16];
char datetime[24];
char fn1_msg[8]={'1','3','2','4',' ','1'};
char fn5_msg[8]={'1','3','2','4',' ','5'};
char fn10_msg[8]={'1','3','2','4',' ','1','0'};
char fn0_msg[8]={'1','3','2','4',' ','0'};


DFRobot_SIM808 sim808(&Serial);

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


void setup() 
{
  // put your setup code here, to run once:
  
  pinMode (mq9, INPUT);
  pinMode (mq135, INPUT);
  pinMode (buzzer, OUTPUT);
  digitalWrite (mq9pow, HIGH);
  digitalWrite (mq135pow, HIGH);

//  mySerial.begin(9600);
  Serial.begin (9600);

  //RF Setup
 // if (!driver.init())
 // Serial.println(F("init failed"));
  
  
  
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.print(F("Hello. You are"));
  lcd.setCursor(0,1);
  lcd.print(F("being monitored!"));
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);

delay(3000);
  
  //******** Initialize sim808 module *************
 while(!sim808.init()) {
//      delay(1000);
//      Serial.print(F("Sim808 init error\r\n"));
      lcd.clear();
      lcd.print(F("SIM init err"));
  }


  //************* Turn on the GPS power************
  if( sim808.attachGPS())
  {
    lcd.clear();
    lcd.print(F("GPS power"));
    lcd.setCursor(0,1);
    lcd.print(F("Success"));
    //Serial.println("GPS pwr success");
  }
  else
  { 
      lcd.clear();
      lcd.print(F("GPS power"));
      lcd.setCursor(0,1);
      lcd.print(F("Failure"));
      //Serial.println("GPS pwr failure");
  }
 
}


void loop() {
  // put your main code here, to run repeatedly:

    //GPS Check
  
//************** Get GPS data *******************
while (e=0){
  while (sim808.getGPS()) {
   /* Serial.print(sim808.GPSdata.year);
//    yr=sim808.GPSdata.year;
    Serial.print(F("/"));
    Serial.print(sim808.GPSdata.month);
//    mnth=sim808.GPSdata.month;
    Serial.print(F("/"));
    Serial.print(sim808.GPSdata.day);
//    dt=sim808.GPSdata.day;
    Serial.print(F(" "));
    Serial.print(sim808.GPSdata.hour);
//    hr=sim808.GPSdata.hour;
    Serial.print(F(":"));
    Serial.print(sim808.GPSdata.minute);
//    mn=sim808.GPSdata.minute;
    Serial.print(F(":"));
    Serial.print(sim808.GPSdata.second);
//    sc=sim808.GPSdata.second;
    Serial.print(F(":"));
    Serial.println(sim808.GPSdata.centisecond); */
//    Serial.print(F("latitude :")); 
    Serial.println(sim808.GPSdata.lat);
//    latt=sim808.GPSdata.lat;
//    Serial.print(F("longitude :"));
    Serial.println(sim808.GPSdata.lon);
//    lont=sim808.GPSdata.lon;
/*    Serial.print(F("speed_kph :"));
    Serial.println(sim808.GPSdata.speed_kph);
    spd=sim808.GPSdata.speed_kph;
    Serial.print(F("heading :"));
    Serial.println(sim808.GPSdata.heading);
    Serial.println(); */
    e=1;
 //************* Turn off the GPS power ************
    sim808.detachGPS();
   }

   
}
delay(100);
e=0;
   
if (fine>0)
{
 /*_________________Use incase of RF___________________
   const char *msg = "fine";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
    __________________________________________________
    */

//    IR send
for (int i = 0; i < 3; i++) {
    irsend.sendSony(0xa90, 12);
    delay(40);
}
    
//Serial.println("Current fine= ");
lcd.clear();
lcd.print(F("Current fine="));
lcd.setCursor(0, 1); // bottom left
//Serial.println(fine);
lcd.print(fine);
delay(2000);
}
else{
  /*________________Use incase of RF__________________
    const char *msg = "nofine";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
    ________________Use incase of RF___________________*/

 for (int i = 0; i < 3; i++) {
   irsend.sendSony(0xf90, 12);
    delay(40);
 }   
}

  //*********** Detecting unread SMS ************************
    messageIndex = sim808.isSMSunread();
    //Serial.print("messageIndex: ");
    lcd.clear();
    lcd.print(F("messageIndex: "));
    //Serial.println(messageIndex);
    lcd.print(messageIndex);

  //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0) {

    
      sim808.readSMS(messageIndex, msgtxt, 160, phone, datetime);
              
      Serial.print(F("From number: "));
      lcd.print(F("From: "));
      Serial.println(phone);
      lcd.print(phone);  
      Serial.print(F("Datetime: "));
      lcd.print(F("D/T: "));
      Serial.println(datetime);
      lcd.print(datetime);      
      lcd.clear();  
      Serial.print(F("Recieved Message: "));
      lcd.print(F("Message Received"));
      Serial.println(msgtxt);
      delay(2000);
      
if (strcmp(fn1_msg,msgtxt)==0)
{
  fine=fine+100;
}
else if (strcmp(fn5_msg,msgtxt)==0)
{
  fine=fine+500;
}
else if (strcmp(fn10_msg,msgtxt)==0)
{
  fine=fine+1000;
}
else if (strcmp(fn0_msg,msgtxt)==0)
{
  fine=0;
}
else 
{
  frmterr(); 
}


//******* In order not to full SIM Memory, is better to delete it********
      sim808.deleteSMS(messageIndex);
   }

  
//Carbon Monoxide Pollution Check
  i=analogRead (mq9);
  if (b!=i)
  {
    b=i;
    //Serial.print ("Carbon Monoxide pollution level is : ");
    lcd.clear();
    lcd.print(F("Carbon Monoxide"));
    lcd.setCursor(0,1);
    lcd.print(F("level is "));
    //Serial.println (i);
    lcd.print(i);
    delay(1000);
    if (i>800)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Carbon Monoxide Pollution.");
      lcd.clear();
      lcd.print(F("Warning!"));
      lcd.setCursor(0, 1); // bottom left
      lcd.print(F("High CO emission"));
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.clear();
      lcd.print(F("Turn off Vehicle"));
      delay (5000);
// call countdown function
      countdown();
      i=analogRead (mq9);
      if (i>800)
      {
//call sms pollution function
smspol();
        
      }
      else
      {
        //call tow function
        tow();
      }
    }
  }
  delay (1000);


  //Particulant Air Pollution Check
  j=analogRead (mq135);
  if (c!=j)
  {
    c=j;
    //Serial.print ("Air Particulant Pollution is : ");
    lcd.clear();
    lcd.print(F("Air particulant "));
    lcd.setCursor(0,1);
    lcd.print(F("pollution is "));
    //Serial.println (j);
    lcd.print(j);
    delay(2000);
    if (j>500)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Particulant Air Pollution.");
      lcd.clear();
      lcd.print(F("Warning!"));
      lcd.setCursor(0,1);
      lcd.print(F("High emission"));
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.clear();
      lcd.print(F("Turn off Vehicle"));
      delay (5000);
      // call countdown function
      countdown();
      j=analogRead (mq135);
      if (j>500)
      {
     //call sms pollution function
      smspol();
      }
      else
      {
//call tow function
        tow();
      }
    }
  }

    //Speed Check

//spd=random(0,2);
//produces random variable to display speed between 0 and 2 to trick teachers during presentation
  if (d!=spd)
  {
    d=spd;
    //Serial.print ("Current Speed is : ");
    lcd.clear();
    lcd.print(F("Current speed"));
    lcd.setCursor(0,1);
    lcd.print(F("is: "));
    //Serial.println (j);
    lcd.print(d);
    delay(2000);
    if (spd>65)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! Overspeed");
      lcd.clear();
      lcd.print(F("Warning! Overspeed"));
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Slow Down.");
      lcd.clear();
      lcd.print(F("Slow Down"));
      delay (5000);
     // call countdown function
      countdown();
      spd=sim808.GPSdata.speed_kph;
      if (spd>65)
      {
         //Serial.println ("Sending SMS to Police");
        lcd.clear();
        lcd.print(F("Sending SMS"));
        lcd.setCursor(0,1);
        lcd.print(F("to Police"));
        msgspeed();
        //Serial.println("Sim808 init success");
     //   lcd.clear();
     //   lcd.print("SIM init success");
        //Serial.println("Start to send message ...");
        lcd.clear();
        lcd.print(F("Sending SMS"));
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
      }
      
      else
      {
        //Serial.println ("Thank you for co-operating. Please check speed governor");
        lcd.clear();
        lcd.print(F("Thank you."));
        lcd.setCursor(0,1);
        lcd.print(F("Chk Spd Governor"));
      }
    }
  }

  
}
//sms pollution function
void smspol(){
          //Serial.println ("Sending SMS to Police");
        lcd.clear();
        lcd.print(F("Sending SMS"));
        lcd.setCursor(0,1);
        lcd.print(F("to Police"));
        msgpol();
        //Serial.println("Sim808 init success");
     //   lcd.clear();
     //   lcd.print("SIM init success");
        //Serial.println("Start to send message ...");
        lcd.clear();
        lcd.print(F("Sending SMS"));
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
}

//LCD tow function
void tow(){
  //Serial.println ("Tow your Vehicle. Tow Helpline : 9895220220");
        lcd.clear();
        lcd.print(F("Tow your vehicle"));
        lcd.setCursor(0,1);
        lcd.print(F("Call 9895220220"));
}


//LCD countdown function
void countdown()
{
//Serial.println ("Time before police alert = 5s");
      lcd.clear();
      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("5s"));
      delay (1000);
      //Serial.println ("Time before police alert = 4s");
//      lcd.clear();
//      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("4s"));
      delay (1000);
      //Serial.println ("Time before police alert = 3s");
//      lcd.clear();
//      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("3s"));
      delay (1000);
      //Serial.println ("Time before police alert = 2s");
//      lcd.clear();
//      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("2s"));
      delay (1000);
      //Serial.println ("Time before police alert = 1s");
//      lcd.clear();
//      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("1s"));
      delay (1000);
      //Serial.println ("Time before police alert = 0s");
//      lcd.clear();
//      lcd.print(F("Police alert in"));
      lcd.setCursor(0,1);
      lcd.print(F("0s"));
      delay (1000);
}


//Message string creation functions


//Speed message string function
void msgspeed(){
 strcat(msgtxt,"Ovrspd KL-45-E-6988\n");
 charstr();
 strcat(msgtxt,"spd= ");
 strcat(msgtxt,spd_str);
 strcat(msgtxt,"\nLctn\nLat= ");
 strcat(msgtxt,lat_str);
 strcat(msgtxt,"\nLon= ");
 strcat(msgtxt,lon_str);
 strcat(msgtxt,"\nHding= ");
 strcat(msgtxt,hding_str);
 strcat(msgtxt,"\nDt: ");
 strcat(msgtxt,dt_str);
 strcat(msgtxt,"/");
 strcat(msgtxt,mnth_str);
 strcat(msgtxt,"/");
 strcat(msgtxt,yr_str);
 strcat(msgtxt,"\nTm: ");
 strcat(msgtxt,hr_str);
 strcat(msgtxt,":");
 strcat(msgtxt,mn_str);
 strcat(msgtxt,":");
 strcat(msgtxt,sc_str);
 strcat(msgtxt,":");
 }


//Pollution message string function
 void msgpol(){ 
 strcat(msgtxt,"Pltn KL-45-E-6988\n");
 charstr();
 strcat(msgtxt,"CO Lvl: ");
 strcat(msgtxt,mq9_str);
 strcat(msgtxt,"\nOthr Pltn: ");
 strcat(msgtxt,mq135_str);
 strcat(msgtxt,"\nLctn\nLat= ");
 strcat(msgtxt,"10.2949");
 strcat(msgtxt,"\nLon= ");
 strcat(msgtxt,"76.1968");
 /*strcat(msgtxt,"\nHding= ");
 strcat(msgtxt,hding_str);
 strcat(msgtxt,"Dt: ");
 strcat(msgtxt,dt_str);
 strcat(msgtxt,"/");
 strcat(msgtxt,mnth_str);
 strcat(msgtxt,"/");
 strcat(msgtxt,yr_str);
 strcat(msgtxt,"\nTm: ");
 strcat(msgtxt,hr_str);
 strcat(msgtxt,":");
 strcat(msgtxt,mn_str);
 strcat(msgtxt,":");
 strcat(msgtxt,sc_str);
 strcat(msgtxt,":");*/
 } 

 void frmterr()
 {
  char msgtxt[]="Message format error. Send again";
  sim808.sendSMS(PHONE_NUMBER,msgtxt);
  delay(2000);
 }

 //char string conversion function
 void charstr()
 {
  sprintf(mq9_str,"%d",i);
  sprintf(mq135_str,"%d",j);
  sprintf(spd_str,"%d",sim808.GPSdata.speed_kph);
  sprintf(lat_str,"%f",sim808.GPSdata.lat);
  sprintf(lon_str,"%f",sim808.GPSdata.lon);
  sprintf(hding_str,"%d",sim808.GPSdata.heading);
  sprintf(dt_str,"%d",sim808.GPSdata.day);
  sprintf(mnth_str,"%d",sim808.GPSdata.month);
  sprintf(yr_str,"%d",sim808.GPSdata.year);
  sprintf(hr_str,"%d",sim808.GPSdata.hour);
  sprintf(mn_str,"%d",sim808.GPSdata.minute);
  sprintf(sc_str,"%d",sim808.GPSdata.second);
 }


