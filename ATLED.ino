//Variable Initialization
//constant Variables
const int mq9=A0, mq135=A1, buzzer=3, mq9pow=4, mq135pow=7;

//Permanent Variables
float lat=0, lon=0;
int spd=0, hding=0;
int yr=0, mnth=0, dt=0, hr=0, mn=0, sc=0;
int fine=0;

char yr_str[4], mnth_str[2], dt_str[2], hr_str[2], mn_str[2], sc_str[2];
char spd_str[2], lat_str[4], lon_str[4], hding_str[3];
char mq9_str[4], mq135_str[4];

char *character;

//Temporary Variables
int i=0, j=0, b=0, c=0, d=0;

//Header
#include <VirtualWire.h>
#include <DFRobot_sim808.h>
#include <LiquidCrystal.h>

//Define
//TX RX Pin??
#define PIN_TX    10
#define PIN_RX    11

//SMS Send setup
//Mobile phone number
#define PHONE_NUMBER "9895220220"  
 
//The content of messages sent
/*#define MESSAGE1  "Overspeeding" 
#define MESSAGE2  "Vehicle is polluting"
#define MESSAGE3  "System Not Moving"*/
char msgtxt[124];




//SMS Read setup
int messageIndex=0;
char phone[14];
char datetime[1];


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

//RF Transmitter setup
  vw_set_ptt_inverted(true); //
  vw_set_tx_pin(12);
  vw_setup(4000);// speed of data transfer Kbps
  
  Serial.begin (9600);
  
   // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello. You are being monitored!");

delay(3000);
  
  //******** Initialize sim808 module *************
  while(!sim808.init()) {
      delay(1000);
      Serial.print("Sim808 init error\r\n");
  }


  //************* Turn on the GPS power************
  if( sim808.attachGPS())
  {
    lcd.print("GPS pwr Success");
    //Serial.println("GPS pwr success");
  }
  else
  { 
      lcd.print("GPS pwr failure");
      //Serial.println("GPS pwr failure");
  }
      delay(3000);


 
}


void loop() {
  // put your main code here, to run repeatedly:
if (fine>0)
{
  character="1";
  vw_send((uint8_t *)character, strlen(2));
  vw_wait_tx(); // Wait until the whole message is gone
  delay(2000);
//Serial.println("Current fine= ");
lcd.print("Current fine= ");
//Serial.println(fine);
lcd.print(fine);
delay(5000);
}
else{
  character="0";
  vw_send((uint8_t *)character, strlen(2));
  vw_wait_tx(); // Wait until the whole message is gone
  delay(2000);
}

  //*********** Detecting unread SMS ************************
    messageIndex = sim808.isSMSunread();
    //Serial.print("messageIndex: ");
    lcd.print("messageIndex: ");
    //Serial.println(messageIndex);
    lcd.print(messageIndex);

  //*********** At least, there is one UNREAD SMS ***********
   if (messageIndex > 0) { 
      sim808.readSMS(messageIndex, msgtxt, 130, phone, datetime);
              
  //******* In order not to full SIM Memory, is better to delete it********
      sim808.deleteSMS(messageIndex);
      Serial.print("From number: ");
      //lcd.print("From: ");
      Serial.println(phone);
      //lcd.print(phone);  
      //Serial.print("Datetime: ");
      //lcd.print("D/T: ");
      //Serial.println(datetime);
      //lcd.print(datetime);        
      Serial.print("Recieved Message: ");
      lcd.print("Message Received");
      Serial.println(msgtxt);
   }

if (msgtxt[1] == "1" && msgtxt[2]=="3" && msgtxt[3]=="2" && msgtxt [4]=="4" && phone=="9895220220")
{
if (msgtxt[6] == "1")
{
  fine=fine+100;
}
else if (msgtxt[6] == "5")
{
  fine=fine+500;
}
else if (msgtxt[6] == "1" && msgtxt[7]=="0")
{
  fine=fine+1000;
}
else if (msgtxt[6] == "0")
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
    lcd.print("CO lvl: ");
    //Serial.println (i);
    lcd.print(i);
    if (i>500)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Carbon Monoxide Pollution.");
      lcd.print("Warning! High CO pollution");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.print("Switch off Vehicle");
      delay (5000);
      //Serial.println ("Time before police alert = 5s");
      lcd.print("Time before police alert = 5s");
      delay (1000);
      //Serial.println ("Time before police alert = 4s");
      lcd.print("Time before police alert = 4s");
      delay (1000);
      //Serial.println ("Time before police alert = 3s");
      lcd.print("Time before police alert = 3s");
      delay (1000);
      //Serial.println ("Time before police alert = 2s");
      lcd.print("Time before police alert = 2s");
      delay (1000);
      //Serial.println ("Time before police alert = 1s");
      lcd.print("Time before police alert = 1s");
      delay (1000);
      //Serial.println ("Time before police alert = 0s");
      lcd.print("Time before police alert = 0s");
      delay (1000);
      i=analogRead (mq9);
      if (i>500)
      {
        //Serial.println ("Sending SMS to Police");
        lcd.print("Sending SMS to Police");
        msgpol();
        //Serial.println("Sim808 init success");
        lcd.print("Sim808 init success");
        //Serial.println("Start to send message ...");
        lcd.print("Start to send SMS");
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
        
      }
      else
      {
        //Serial.println ("Tow your Vehicle. Tow Helpline : 9895220220");
        lcd.print("Tow your vehicle. Call 9895220220");
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
    lcd.print("Air part polution is: ");
    //Serial.println (j);
    lcd.print("j");
    if (j>500)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! High Particulant Air Pollution.");
      lcd.print("Warning! High air pollution");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Switch off vehicle");
      lcd.print("Switch off Vehicle");
      delay (5000);
      //Serial.println ("Time before police alert = 5s");
      lcd.print("Time before police alert = 5s");
      delay (1000);
      //Serial.println ("Time before police alert = 4s");
      lcd.print("Time before police alert = 4s");
      delay (1000);
      //Serial.println ("Time before police alert = 3s");
      lcd.print("Time before police alert = 3s");
      delay (1000);
      //Serial.println ("Time before police alert = 2s");
      lcd.print("Time before police alert = 2s");
      delay (1000);
      //Serial.println ("Time before police alert = 1s");
      lcd.print("Time before police alert = 1s");
      delay (1000);
      //Serial.println ("Time before police alert = 0s");
      lcd.print("Time before police alert = 0s");
      delay (1000);
      j=analogRead (spd);
      if (j>500)
      {
        //Serial.println ("Sending SMS to Police");
        lcd.print("Sending SMS to Police");
        msgpol();
        Serial.println("Sim808 init success");
        Serial.println("Start to send message ...");
        sim808.sendSMS(PHONE_NUMBER,msgtxt);
        
      }
      else
      {
        //Serial.println ("Tow your Vehicle. Tow Helpline : 9895220220");
        lcd.print("Tow your vehicle. Call 9895220220");
      }
    }
  }

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
    lat=sim808.GPSdata.lat;
    Serial.print("longitude :");
    Serial.println(sim808.GPSdata.lon);
    lon=sim808.GPSdata.lon;
    Serial.print("speed_kph :");
    Serial.println(sim808.GPSdata.speed_kph);
    spd=sim808.GPSdata.speed_kph;
    Serial.print("heading :");
    Serial.println(sim808.GPSdata.heading);
    hding=sim808.GPSdata.heading;
    Serial.println();
   }

    //************* Turn off the GPS power ************
    //sim808.detachGPS();


    //Speed Check


  if (d!=spd)
  {
    d=spd;
    //Serial.print ("Current Speed is : ");
    lcd.print("Current speed is: ");
    //Serial.println (j);
    lcd.print(j);
    if (spd>65)
    {
      digitalWrite (buzzer, HIGH);
      //Serial.println ("Warning! Overspeed");
      lcd.print("Warning! Overspeed");
      delay(5000);
      digitalWrite (buzzer, LOW);
      delay (500);
      //Serial.println ("Slow Down.");
      lcd.print("Slow Down");
      delay (5000);
      //Serial.println ("Time before police alert = 5s");
      lcd.print("Time before police alert = 5s");
      delay (1000);
      //Serial.println ("Time before police alert = 4s");
      lcd.print("Time before police alert = 4s");
      delay (1000);
      //Serial.println ("Time before police alert = 3s");
      lcd.print("Time before police alert = 3s");
      delay (1000);
      //Serial.println ("Time before police alert = 2s");
      lcd.print("Time before police alert = 2s");
      delay (1000);
      //Serial.println ("Time before police alert = 1s");
      lcd.print("Time before police alert = 1s");
      delay (1000);
      //Serial.println ("Time before police alert = 0s");
      lcd.print("Time before police alert = 0s");
      delay (1000);
      spd=sim808.GPSdata.speed_kph;
      if (spd>65)
      {
        //Serial.println ("Sending SMS to Police");
        lcd.print("Sending SMS to Police");
        msgspeed();
        Serial.println("Sim808 init success");
        Serial.println("Start to send message ...");
        sim808.sendSMS(PHONE_NUMBER,msgtxt);     
      }
      
      else
      {
        //Serial.println ("Thank you for co-operating. Please check speed governor");
        lcd.print("Thank you. Please Check Speed Governor");
      }
    }
  }

  
}


//Message string creation functions


//Speed message string function
void msgspeed(){
  char msgtxt[130] = {"\0"};
 strcat(msgtxt,"Ovrspd KL-45-E-6988\n");
 itoa(spd,spd_str,2);
 itoa(lat,lat_str,4);
 itoa(lon,lon_str,4);
 itoa(hding,hding_str,3);
 itoa(dt,dt_str,2);
 itoa(mnth,mnth_str,2);
 itoa(yr,yr_str,4);
 itoa(hr,hr_str,2);
 itoa(mn,mn_str,2);
 itoa(sc,sc_str,2);
 strcat(msgtxt,"spd= ");
 strcat(msgtxt,spd_str);
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


//Pollution message string function
 void msgpol(){ 
  char msgtxt[130] = {"\0"};
 strcat(msgtxt,"Pltn KL-45-E-6988\n");
 itoa(mq9,mq9_str,4);
 itoa(mq135,mq135_str,4);
 itoa(lat,lat_str,4);
 itoa(lon,lon_str,4);
 itoa(hding,hding_str,3);
 itoa(dt,dt_str,2);
 itoa(mnth,mnth_str,2);
 itoa(yr,yr_str,4);
 itoa(hr,hr_str,2);
 itoa(mn,mn_str,2);
 itoa(sc,sc_str,2);
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

