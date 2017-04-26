#include <VirtualWire.h>

int  led_red=3, led_yel=4, led_green=5, buz=6;

void setup()
{
  pinMode(led_red,OUTPUT);
  pinMode(led_yel,OUTPUT);
  pinMode(led_green,OUTPUT);
  vw_set_ptt_inverted(true); // Required for DR3100
    vw_set_rx_pin(13);
    vw_setup(4000);  // Bits per sec
    pinMode (11,OUTPUT);
    digitalWrite (11, HIGH);

    vw_rx_start();       // Start the receiver PLL running
    Serial.begin (9600);
}

 void loop()
{
  digitalWrite(led_red,LOW);
  digitalWrite(led_yel,LOW);
  digitalWrite(led_green,LOW);
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;

    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      if(buf[0]=='1')
      {
      digitalWrite(led_red,HIGH);
      digitalWrite(buz,HIGH);
      }
      else if(buf[0]=='0')
      digitalWrite(led_green,HIGH);
      else
      digitalWrite(led_yel,LOW);
     }
}
