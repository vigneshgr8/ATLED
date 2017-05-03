//Header
#include <DFRobot_sim808.h>
#include <LiquidCrystal.h>
//#include <SoftwareSerial.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

//Variable Initialization
//constant Variables
const int mq9=A0, mq135=A1, buzzer=5, mq9pow=4, mq135pow=7;

//Permanent Variables
float latt=0, lont=0;
int spd=0, hding=0;
int yr=0, mnth=0, dt=0, hr=0, mn=0, sc=0;
int fine=0;

char yr_str[4], mnth_str[2], dt_str[2], hr_str[2], mn_str[2], sc_str[2];
char spd_str[2], lat_str[4], lon_str[4], hding_str[3];
char mq9_str[4], mq135_str[4];

char *character;

//Temporary Variables
int i=0, j=0, b=0, c=0, d=0;


//RH_ASK driver;
 RH_ASK driver(2000, 5, 6, 4); // ESP8266: do not use pin 11

//Define
//TX RX Pin??
//#define PIN_TX    10
//#define PIN_RX    11

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
  
  Serial.begin (9600);

  //RF Setup
  if (!driver.init())
  Serial.println("init failed");
  
  
  
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.print("Hello. You are");
  lcd.setCursor(0,1);
  lcd.print("being monitored!");
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);

delay(3000);
  
  //******** Initialize sim808 module *************
 while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
      lcd.clear();
      lcd.print("SIM init err");
  }


  //************* Turn on the GPS power************
  if( sim808.attachGPS())
  {
    lcd.clear();
    lcd.print("GPS power");
    lcd.setCursor(0,1);
    lcd.print("Success");
    //Serial.println("GPS pwr success");
  }
  else
  { 
      lcd.clear();
      lcd.print("GPS power");
      lcd.setCursor(0,1);
      lcd.print("Failure");
      //Serial.println("GPS pwr failure");
  }
 
}


void loop() {
  // put your main code here, to run repeatedly:

    //GPS Check
  
//************** Get GPS data *******************
    if (sim808.getGPS()) {
    Serial.print(sim808.GPSdata.year);
    yr=sim808.GPSdata.year;
    Serial.print("/");
    Serial.print(sim808.GPSdata.month);
    mnth=sim808.GPSdata.month;
    Serial.print("/");
    Serial.print(sim808.GPSdata.day);
    dt=sim808.GPSdata.day;
    Serial.print(" ");
    Serial.print(sim808.GPSdata.hour);
    hr=sim808.GPSdata.hour;
    Serial.print(":");
    Serial.print(sim808.GPSdata.minute);
    mn=sim808.GPSdata.minute;
    Serial.print(":");
    Serial.print(sim808.GPSdata.second);
    sc=sim808.GPSdata.second;
    Serial.print(":");
    Serial.println(sim808.GPSdata.centisecond);
    Serial.print("latitude :");
    Serial.println(sim808.GPSdata.lat);
    latt=sim808.GPSdata.lat;
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon);
    lont=sim808.GPSdata.lon;
    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    spd=sim808.GPSdata.speed_kph;
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);
    hding=sim808.GPSdata.heading;
    Serial.println();
 //************* Turn off the GPS power ************
    sim808.detachGPS();
   }
   delay(3000);

   
if (fine>0)
{
const char *msg = "fine";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
    
//Serial.println("Current fine= ");
lcd.clear();
lcd.print("Current fine=");
lcd.setCursor(0, 1); // bottom left
//Serial.println(fine);
lcd.print(fine);
delay(2000);
}
else{
  const char *msg = "nofine";

    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(200);
}

  //*********** Detecting unread SMS ************************
    messageIndex = sim808.isSMSunread();
    //Serial.print("messageIndex: ");
    lcd.clear();
    lcd.print("messageIndex: ");
    //Serial.println(messageIndex);
    lcd.print(messageIndex);

  //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0) { 
      sim808.readSMS(messageIndex, msgtxt, 160, phone, datetime);
              
  //******* In order not to full SIM Memory, is better to delete it********
      sim808.deleteSMS(messageIndex);
     
      Serial.print("From number: ");
      //lcd.print("From: ");
      Serial.println(phone);
      //lcd.print(phone);  
      Serial.print("Datetime: ");
      //lcd.print("D/T: ");
      Serial.println(datetime);
      //lcd.print(datetime);      
      lcd.clear();  
      Serial.print("Recieved Message: ");
      lcd.print("Message Received");
      Serial.println(msgtxt);
      delay(2000);
   }

if (msgtxt[0] == "1" && msgtxt[1]=="3" && msgtxt[2]=="2" && msgtxt [3]=="4")
{
if (msgtxt[5] == "1")
{
  fine=fine+100;
}
else if (msgtxt[5] == "5")
{
  fine=fine+500;
}
else if (msgtxt[5] == "1" && msgtxt[6]=="0")
{
  fine=fine+1000;
}
else if (msgtxt[5] == "0")
{
  fine=0;
}
else 
{
  frmterr();
  sim808.sendSMS(PHONE_NUMBER,msgtxt);  
}
}

  
//Carbon Monoxide Pollution Check
  i=analogRead (mq9);
  if (b!=i)
  {
    b=i;
    //Serial.print ("Carbon Monoxide pollution level is : ");
    lcd.clear();
    lcd.print("Carbon Monoxide");
    lcd.setCursor(0,1);
    lcd.print("level is ");
    //Serial.println (i);
    lcd.print(i);
    delay(2000);
    if (i>800)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Carbon Monoxide Pollution.");
      lcd.clear();
      lcd.print("Warning!");
      lcd.setCursor(0, 1); // bottom left
      lcd.print("High CO emission");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.clear();
      lcd.print("Turn off Vehicle");
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
    lcd.print("Air particulant ");
    lcd.setCursor(0,1);
    lcd.print("pollution is ");
    //Serial.println (j);
    lcd.print(j);
    delay(2000);
    if (j>500)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Particulant Air Pollution.");
      lcd.clear();
      lcd.print("Warning!");
      lcd.setCursor(0,1);
      lcd.print("High emission");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.clear();
      lcd.print("Turn off Vehicle");
      delay (5000);
      // call countdown function
      countdown();
      j=analogRead (spd);
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


  if (d!=spd)
  {
    d=spd;
    //Serial.print ("Current Speed is : ");
    lcd.clear();
    lcd.print("Current speed");
    lcd.setCursor(0,1);
    lcd.print("is: ");
    //Serial.println (j);
    lcd.print(d);
    delay(2000);
    if (spd>65)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! Overspeed");
      lcd.clear();
      lcd.print("Warning! Overspeed");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Slow Down.");
      lcd.clear();
      lcd.print("Slow Down");
      delay (5000);
     // call countdown function
      countdown();
      spd=sim808.GPSdata.speed_kph;
      if (spd>65)
      {
         //Serial.println ("Sending SMS to Police");
        lcd.clear();
        lcd.print("Sending SMS");
        lcd.setCursor(0,1);
        lcd.print("to Police");
        msgspeed();
        //Serial.println("Sim808 init success");
     //   lcd.clear();
     //   lcd.print("SIM init success");
        //Serial.println("Start to send message ...");
        lcd.clear();
        lcd.print("Sending SMS");
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
      }
      
      else
      {
        //Serial.println ("Thank you for co-operating. Please check speed governor");
        lcd.clear();
        lcd.print("Thank you.");
        lcd.setCursor(0,1);
        lcd.print("Chk Spd Governor");
      }
    }
  }

  
}
//sms pollution function
void smspol(){
          //Serial.println ("Sending SMS to Police");
        lcd.clear();
        lcd.print("Sending SMS");
        lcd.setCursor(0,1);
        lcd.print("to Police");
        msgpol();
        //Serial.println("Sim808 init success");
     //   lcd.clear();
     //   lcd.print("SIM init success");
        //Serial.println("Start to send message ...");
        lcd.clear();
        lcd.print("Sending SMS");
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
}

//LCD tow function
void tow(){
  //Serial.println ("Tow your Vehicle. Tow Helpline : 9895220220");
        lcd.clear();
        lcd.print("Tow your vehicle");
        lcd.setCursor(0,1);
        lcd.print("Call 9895220220");
}


//LCD countdown function
void countdown()
{
//Serial.println ("Time before police alert = 5s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("5s");
      delay (1000);
      //Serial.println ("Time before police alert = 4s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("4s");
      delay (1000);
      //Serial.println ("Time before police alert = 3s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("3s");
      delay (1000);
      //Serial.println ("Time before police alert = 2s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("2s");
      delay (1000);
      //Serial.println ("Time before police alert = 1s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("1s");
      delay (1000);
      //Serial.println ("Time before police alert = 0s");
      lcd.clear();
      lcd.print("Police alert in");
      lcd.setCursor(0,1);
      lcd.print("0s");
      delay (1000);  
}


//Message string creation functions


//Speed message string function
void msgspeed(){
 strcat(msgtxt,"Ovrspd KL-45-E-6988\n");
 sprintf(spd_str,"%d",spd);
 sprintf(lat_str,"%f",latt);
 sprintf(lon_str,"%f",lont);
 sprintf(hding_str,"%d",hding);
 sprintf(dt_str,"%d",dt);
 sprintf(mnth_str,"%d",mnth);
 sprintf(yr_str,"%d",yr);
 sprintf(hr_str,"%d",hr);
 sprintf(mn_str,"%d",mn);
 sprintf(sc_str,"%d",sc);
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
 sprintf(spd_str,"%d",spd);
 sprintf(lat_str,"%f",latt);
 sprintf(lon_str,"%f",lont);
 sprintf(hding_str,"%d",hding);
 sprintf(dt_str,"%d",dt);
 sprintf(mnth_str,"%d",mnth);
 sprintf(yr_str,"%d",yr);
 sprintf(hr_str,"%d",hr);
 sprintf(mn_str,"%d",mn);
 sprintf(sc_str,"%d",sc);
 strcat(msgtxt,"CO Lvl: ");
 strcat(msgtxt,mq9_str);
 strcat(msgtxt,"\nOthr Pltn: ");
 strcat(msgtxt,mq135_str);
 strcat(msgtxt,"\nLctn\nLat= ");
 strcat(msgtxt,lat_str);
 strcat(msgtxt,"\nLon= ");
 strcat(msgtxt,lon_str);
 strcat(msgtxt,"\nHding= ");
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
 strcat(msgtxt,":");
 } 

 void frmterr()
 {
  char msgtxt[]="Message format error. Send again";
 }

