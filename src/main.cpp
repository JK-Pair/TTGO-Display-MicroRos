#include <Arduino.h>
#include <TFT_eSPI.h>

#define MC_DATUM 4

TFT_eSPI tft = TFT_eSPI();

String force_str = "F:";
String force_int = "50 N";

String duty_str = "D:";
String duty_int = "50 %";

String state_str = "S:";
String state_int = "0";

String mode_str = "Manual";


struct Button
{
  const uint8_t PIN;
  bool pressed;
};


Button top_button = {35, false};
Button below_button = {0, false};


int state_machine = 0;


void IRAM_ATTR IO_INT_ISR()
{
  // Toggle The state machine
  below_button.pressed = false;
  top_button.pressed = true;
}

void IRAM_ATTR IO_INT_ISR2()
{
  // Toggle The state machine
  top_button.pressed = false;
  below_button.pressed = true;
}

void setup()
{
  pinMode(below_button.PIN, INPUT_PULLUP);
  pinMode(top_button.PIN, INPUT_PULLUP);
  attachInterrupt(top_button.PIN, IO_INT_ISR, RISING);
  attachInterrupt(below_button.PIN, IO_INT_ISR2, FALLING);


  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_RED);
  delay(50);
  tft.fillScreen(TFT_GREEN);
  delay(50);
  tft.fillScreen(TFT_BLUE);
  delay(50);

  tft.setTextSize(6);
  
  // tft.drawString(force_str+force_int, 5, 10);
  // tft.drawString(duty_str+duty_int, 5, 50);
  // tft.drawString(state_str+state_int, 5, 95);


  // tft.setTextDatum(MC_DATUM);

}

void loop()
{
  if (top_button.pressed)
  {
    force_int = "15 N";
    duty_int = "75 %";
    state_int = "1";
    mode_str = "Manual";
    
  }

  else if (below_button.pressed)
  {
    force_int = "50 N";
    duty_int = "26 %";
    state_int = "2";
    mode_str = "Auto  ";
  }

  tft.setTextSize(6);
  tft.drawString(state_str+state_int, 5, 0);
  tft.setTextSize(3);
  tft.drawString(mode_str, 125, 15);
  tft.setTextSize(6);
  tft.drawString(force_str+force_int, 5, 48);
  tft.drawString(duty_str+duty_int, 5, 95);




}