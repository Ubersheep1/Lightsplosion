#ifndef DEFINITIONS
#define DEFINITIONS

#define NO_LEDS 200
#define PIN 5
#define WHEELPOSMAX 767
#define COMSRXBUFFLEN  250

#define COLOUR_BLACK   strip.Color(0,0,0)
#define COLOUR_RED     strip.Color(255,0,0)
#define COLOUR_GREEN   strip.Color(0,255,0)
#define COLOUR_BLUE    strip.Color(0,0,255)
#define COLOUR_YELLOW  strip.Color(255,255,0)
#define COLOUR_CYAN    strip.Color(0,255,255)
#define COLOUR_MAGENTA strip.Color(255,0,255)
#define COLOUR_WHITE   strip.Color(255,255,255)

enum
{//used to work out where to reply when a CMD is recieved
  OUTPUTTO_NETWORK,
  OUTPUTTO_SERIAL,
};

enum
{
  OPP_SUCCESS,
  OPP_FAIL,
};

enum
{//the main LED state machine states, also used for comms CMD codes because of reasons
  STATE_WAIT,
  STATE_RELEASE,
  STATE_THCHASE,
  STATE_COLOURWIPE,
  STATE_WHEEL,
  STATE_RAINBOW,
  STATE_RAND,
  STATE_SOLID,
  STATE_SEQUENCEEND,
  STATE_HELP,
  STATE_MACRO,
  STATE_UNKNOWN,
  STATE_END,
};

/*
enum
{//the CMD codes for each command which can be recieved
  CMDCODE_HELP,
  CMDCODE_MACRO,
  CMDCODE_THCHASE,
  CMDCODE_COLOURWIPE,
  CMDCODE_WHEEL,
  CMDCODE_RAINBOW,
  CMDCODE_RAND,
  CMDCODE_SOLID,
  CMDCODE_UNKNOWN,
};
*/

enum
{
  OPTCODE_N,
  OPTCODE_C1,
  OPTCODE_C2,
  OPTCODE_W,
  OPTCODE_I,
  OPTCODE_IMM,
  OPTCODE_UNKNOWN,
  OPTCODE_END,
};

typedef struct
{//the data structure for the settings of a macro
  uint32_t  colouor1;
  uint32_t  colouor2;
  uint16_t  wait;
  uint32_t  itterations;
}SETTINGS;

typedef struct
{//the data structure for an element in the sequence array
  uint8_t State;
  SETTINGS Settings;
}SEQUENCE;

#endif
