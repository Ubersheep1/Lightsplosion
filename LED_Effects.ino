#include "definitions.h"

extern SETTINGS Settings;

uint8_t theaterChase_State = 0;
uint8_t theaterChase_Next = 0;
uint16_t theaterChase_Wait = 0;
//Theatre-style crawling lights.
void theaterChase(void)
{
  switch(theaterChase_State)
  {
    case 0:
    {
      for (uint16_t i=0; i < strip.numPixels(); i=i+5)
      {
        strip.setPixelColor(i, Settings.colouor1);
        strip.setPixelColor(i+4, 0);
      }
      strip.show();
      theaterChase_Wait = Settings.wait;
      theaterChase_Next = 1;
      theaterChase_State = 10;
      break;
    }
    case 1:
    {
      for (uint16_t i=0; i < strip.numPixels(); i=i+5)
      {
        strip.setPixelColor(i+1, Settings.colouor1);
        strip.setPixelColor(i, 0);
      }
      strip.show();
      theaterChase_Wait = Settings.wait;
      theaterChase_Next = 2;
      theaterChase_State = 10;
      break;
    }
    case 2:
    {
      for (uint16_t i=0; i < strip.numPixels(); i=i+5)
      {
        strip.setPixelColor(i+2, Settings.colouor1);
        strip.setPixelColor(i+1, 0);
      }
      strip.show();
      theaterChase_Wait = Settings.wait;
      theaterChase_Next = 3;
      theaterChase_State = 10;
      break;
    }
    case 3:
    {
      for (uint16_t i=0; i < strip.numPixels(); i=i+5)
      {
        strip.setPixelColor(i+3, Settings.colouor1);
        strip.setPixelColor(i+2, 0);
      }
      strip.show();
      theaterChase_Wait = Settings.wait;
      theaterChase_Next = 4;
      theaterChase_State = 10;
      break;
    }
    case 4:
    {
      for (uint16_t i=0; i < strip.numPixels(); i=i+5)
      {
        strip.setPixelColor(i+4, Settings.colouor1);
        strip.setPixelColor(i+3, 0);
      }
      strip.show();
      theaterChase_Wait = Settings.wait;
      theaterChase_Next = 0;
      theaterChase_State = 10;
      break;
    }
    case 10:
    {
      if(theaterChase_Wait == 0)
      {
        //only chase the specified number of times
        if(Settings.itterations == 0)
        {
          theaterChase_State = 0;
          lightingstate = STATE_RELEASE;
          break;
        }
        else
        {
          Settings.itterations --;
        }
        theaterChase_State = theaterChase_Next;
        break;
      }
      theaterChase_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

// Fill the dots one after the other with a color
uint8_t  colorWipe_State = 0;
uint16_t colorWipe_Wait = 0;
uint16_t colorWipe_Pos = 0;
void colorWipe(void)
{
  switch(colorWipe_State)
  {
    case 0:
    {
      strip.setPixelColor(colorWipe_Pos, Settings.colouor1);
      strip.show();
      colorWipe_Pos ++;
      if(colorWipe_Pos > strip.numPixels())
      {
        colorWipe_Pos = 0;
        
        Settings.itterations --;
        if(Settings.itterations == 0)
        {
          colorWipe_State = 0;
          lightingstate = STATE_RELEASE;
          break;
        }
      }
      colorWipe_Wait = Settings.wait;
      colorWipe_State = 1;
      break;
    }
    case 1:
    {
      if(colorWipe_Wait == 0)
      {
        colorWipe_State = 0;
        break;
      }
      colorWipe_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

//set the whole strip to a solid colour for a specified amount of time
uint8_t  solidColour_State = 0;
uint16_t solidColour_Wait = 0;
void solidColour(void)
{
  switch(solidColour_State)
  {
    case 0:
    {
      for(uint16_t i=0; i<strip.numPixels(); i++)
      {
         strip.setPixelColor(i, Settings.colouor1);
      }
      strip.show();
      solidColour_Wait = Settings.wait;
      solidColour_State = 1;
      break;
    }
    case 1:
    {
      if(solidColour_Wait == 0)
      {
        lightingstate = STATE_RELEASE;
        solidColour_State = 0;
        break;
      }
      solidColour_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

// Fill random dots with random colors
uint8_t  randcolours_State = 0;
uint16_t randcolours_Wait = 0;
void randcolours(void)
{
  switch(randcolours_State)
  {
    case 0:
    {
      randcolours_State = 1;
      
      for(uint16_t i=0; i<strip.numPixels(); i++)
      {
        strip.setPixelColor(i, Wheel(random(0,WHEELPOSMAX)));
      }
      strip.show();
      break;
    }
    case 1:
    {
      strip.setPixelColor(random(0,strip.numPixels()), Wheel(random(0,WHEELPOSMAX)));
      strip.show();
      
      //only chase a given number of times
      Settings.itterations --;
      if(Settings.itterations == 0)
      {
        randcolours_State = 0;
        lightingstate = STATE_RELEASE;
        break;
      }
      randcolours_Wait = Settings.wait;
      randcolours_State = 2;
      break;
    }
    case 2:
    {
      if(randcolours_Wait == 0)
      {
        randcolours_State = 1;
        break;
      }
      randcolours_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

//set the entire strip to a cycling position in the colour wheel
uint8_t  stripwheel_State = 0;
uint16_t stripwheel_Wait = 0;
uint16_t stripwheel_Pos = 0;
void stripwheel(void)
{
  switch(stripwheel_State)
  {
    case 0:
    {
      uint32_t c = Wheel(stripwheel_Pos);
      stripwheel_Pos++;
      for(uint16_t i=0; i<strip.numPixels(); i++)
      {
         strip.setPixelColor(i, c);
      }
      strip.show();
      
      if(stripwheel_Pos > WHEELPOSMAX)
      {
        stripwheel_Pos = 0;
        //only chase a given number of times
        Settings.itterations --;
        if(Settings.itterations == 0)
        {
          stripwheel_State = 0;
          lightingstate = STATE_RELEASE;
          break;
        }
      }
      stripwheel_Wait = Settings.wait;
      stripwheel_State = 1;
      break;
    }
    case 1:
    {
      if(stripwheel_Wait == 0)
      {
        stripwheel_State = 0;
        break;
      }
      stripwheel_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

//in this function, the colour wheel is unrolled along the strip and
//slowly moves along the strip (LED 1 is a different starting point
uint8_t  striprainbow_State = 0;
uint16_t striprainbow_Wait = 0;
uint16_t striprainbow_Pos = 0;
void striprainbow(void)
{
  switch(striprainbow_State)
  {
    case 0:
    {
      for(uint16_t i=0; i<strip.numPixels(); i++)
      {
         int temp = int((float)(striprainbow_Pos+i)*wheelPosIncrement);
         strip.setPixelColor(i, Wheel(temp>WHEELPOSMAX?temp%WHEELPOSMAX:temp));
      }
      striprainbow_Pos ++;
      strip.show();
      
      if(striprainbow_Pos > strip.numPixels())
      {
        striprainbow_Pos = 0;
        //only chase a given number of times
        Settings.itterations --;
        if(Settings.itterations == 0)
        {
          striprainbow_State = 0;
          lightingstate = STATE_RELEASE;
          break;
        }
      }
      striprainbow_Wait = Settings.wait;
      striprainbow_State = 1;
      break;
    }
    case 1:
    {
      if(striprainbow_Wait == 0)
      {
        striprainbow_State = 0;
        break;
      }
      striprainbow_Wait --;
      delayMicroseconds(1000);
      break;
    }
  }
}

// Input a value 0 to 768 to get a color value from the wheel.
// The wheel is in three sections:
// Section 1: position = 0-255, red=255-0, green=0-255
// Section 2: position = 256-511, green=255-0, blue = 0-255
// Section 3: position = 512-767, blue = 255-0, red = 0-255
uint32_t Wheel(uint16_t WheelPos)
{
  if(WheelPos < 256)
  {
    return strip.Color(255 - WheelPos, WheelPos, 0);
  }
  else if(WheelPos < 512)
  {
    WheelPos -= 256;
    return strip.Color(0, 255 - WheelPos, WheelPos);
  }
  else
  {
    WheelPos %= 256;
    return strip.Color(WheelPos, 0, 255 - WheelPos);
  }
}

