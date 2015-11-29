#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Ethernet.h>
#include "definitions.h"

//globals (some of them.. sorry)
float wheelPosIncrement = 1.0;
char cmdBuffer[COMSRXBUFFLEN] = {0};
uint8_t cmdBufferptr = 0;
char sercmdBuffer[COMSRXBUFFLEN] = {0};
uint8_t sercmdBufferptr = 0;
static byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetServer server(160);  //pick an un-used port
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NO_LEDS, PIN, NEO_RGB + NEO_KHZ800);

void setup()
{
  //set up the serial port
  Serial.begin(9600);
  Serial.println("Starting up");
  
  //set up the ethernet
  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  // Initialize all pixels to 'off'
  strip.begin();
  strip.show();
  
  //calculate the wheel position increment for the number of pixels
  if(strip.numPixels()<WHEELPOSMAX)
  {
    wheelPosIncrement = (float)WHEELPOSMAX / (float)strip.numPixels();
  }
  else
  {
    wheelPosIncrement = (float)strip.numPixels() / (float)WHEELPOSMAX;
  }
  
  //set up the default sequence for the LEDs
  initDefaultSequence();
  
  randomSeed(50);
  
  //setup complete
  Serial.println("Startup complete");
}

void loop()
{
  //in the loop, call the tick functions for all the stateful functionality
  ethernettick();  //this handles any socket data
  serialtick();    //this handles any serial port data
  lightingtick();  //this handles the LEDs
}

void ethernettick(void)
{
  //check if there are any chars in the RX buffer for the ethernet, if there are
  //process them. Look for line terminators and if one pops up, process the
  //chars in the RX buffer through the CMD parser
  EthernetClient client = server.available();
  if (client)
  {
    //get the number of bytes in the RX buffer and loop that number of times
    uint8_t count = client.available();
    for(uint8_t i=0; i<count; i++)
    {
      //read one char off the RX buffer
      char temp = client.read();
      //if it is a line terminator (mac, windows or linux), do a thing
      if((temp == '\r') || (temp == '\n'))
      {
        //if its the first char RXed after a CMD execution, it means that the
        //client connected is a windows box (CRLF line terminator), so discard
        // the extra character and move on
        if(cmdBufferptr == 0)
          continue;
        cmdBuffer[cmdBufferptr] = 0;
        cmdBufferptr = 0;
        //after resetting the intput butter system, parse the command
        cmdParser(cmdBuffer, OUTPUTTO_NETWORK);
        break;
      }
      else
      {
        //the char was not a line terminator, so buffer it
        cmdBuffer[cmdBufferptr] = temp;
        cmdBufferptr++;
      }
      //this prevents buffer overflows
      if(cmdBufferptr >= COMSRXBUFFLEN)
        cmdBufferptr=0;
    }
  }
}

void serialtick(void)
{
  //this function works in exactly the same was as the ethernet tick
  uint8_t count = Serial.available();
  if(count)
  {
    for(uint8_t i=0; i<count; i++)
    {
      char temp = Serial.read();
      //this is the only difference, mirror the RX chars back to the sender
      Serial.write(temp);
      if((temp == '\r') || (temp == '\n'))
      {
        if(sercmdBufferptr == 0)
          continue;
        Serial.write("\r\n");
        sercmdBuffer[sercmdBufferptr] = 0;
        sercmdBufferptr = 0;
        cmdParser(sercmdBuffer, OUTPUTTO_SERIAL);
        break;
      }
      else
      {
        sercmdBuffer[sercmdBufferptr] = temp;
        sercmdBufferptr++;
      }
      if(sercmdBufferptr >= COMSRXBUFFLEN)
        sercmdBufferptr = 0;
    }
  }
}

uint8_t LEDwaitcount = 0;
uint8_t lightingstate = STATE_RELEASE;
void lightingtick(void)
{
  //the lighting process is stateful
  switch(lightingstate)
  {
    case STATE_WAIT:
    {
      //allows a delay with no activity on the LEDs
      if(LEDwaitcount == 0)
      {
        lightingstate = STATE_RELEASE;
        break;
      }
      LEDwaitcount --;
      delayMicroseconds(1000);
      break;
    }
    case STATE_RELEASE:
    {
      //when the state machine is released, it goes and gets the next opperation
      //to perform fro mthe sequence buffer
      sequenceUpdate();
      break;
    }
    case STATE_THCHASE:
    {
      //each effect is also a stateful state machine
      //(nested state machines everywhere!!!!)
      theaterChase();
      break;
    }
    case STATE_COLOURWIPE:
    {
      colorWipe();
      break;
    }
    case STATE_WHEEL:
    {
      stripwheel();
      break;
    }
    case STATE_RAINBOW:
    {
      striprainbow();
      break;
    }
    case STATE_RAND:
    {
      randcolours();
      break;
    }
    case STATE_SOLID:
    {
      solidColour();
      break;
    }
    default:
    {
      //something went wrong, self correct
      lightingstate = STATE_RELEASE;
      break;
    }
  }
}

