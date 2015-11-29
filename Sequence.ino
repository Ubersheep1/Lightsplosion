#include "definitions.h"

SEQUENCE Sequence[50] = {0};
SETTINGS Settings = {0};
uint8_t SequencLen = 0;

void initDefaultSequence(void)
{
  //itterator makes it easier to insert sequences
  SequencLen = 0;
  
  //set up the colours
  Sequence[SequencLen].Settings.colouor1   = COLOUR_RED;
  Sequence[SequencLen+1].Settings.colouor1 = COLOUR_GREEN;
  Sequence[SequencLen+2].Settings.colouor1 = COLOUR_BLUE;
  Sequence[SequencLen+3].Settings.colouor1 = COLOUR_YELLOW;
  Sequence[SequencLen+4].Settings.colouor1 = COLOUR_CYAN;
  Sequence[SequencLen+5].Settings.colouor1 = COLOUR_MAGENTA;
  Sequence[SequencLen+6].Settings.colouor1 = COLOUR_WHITE;
  //The first 7 states are chases with mostly the same settings
  for(uint8_t j=0; j<7; j++, SequencLen++)
  {
    Sequence[SequencLen].State = STATE_THCHASE;
    Sequence[SequencLen].Settings.wait = 75;
    Sequence[SequencLen].Settings.itterations = 20;
  }
  SequencLen++;
  
  //set the strip to solid black so there isn't a weird transition
  //to the next effect.
  Sequence[SequencLen].State = STATE_SOLID;
  Sequence[SequencLen].Settings.colouor1 = COLOUR_BLACK;
  Sequence[SequencLen].Settings.wait = 1;
  SequencLen++;
  
  //set up the colours
  Sequence[SequencLen].Settings.colouor1 = COLOUR_RED;
  Sequence[SequencLen+1].Settings.colouor1 = COLOUR_GREEN;
  Sequence[SequencLen+2].Settings.colouor1 = COLOUR_BLUE;
  Sequence[SequencLen+3].Settings.colouor1 = COLOUR_YELLOW;
  Sequence[SequencLen+4].Settings.colouor1 = COLOUR_CYAN;
  Sequence[SequencLen+5].Settings.colouor1 = COLOUR_MAGENTA;
  Sequence[SequencLen+6].Settings.colouor1 = COLOUR_WHITE;
  //The next 7 states are wipe with mostly the same settings
  for(uint8_t j=0; j<7; j++, SequencLen++)
  {
    Sequence[SequencLen].State = STATE_COLOURWIPE;
    Sequence[SequencLen].Settings.wait = 10;
    Sequence[SequencLen].Settings.itterations = 1;
  }
  SequencLen++;
  
  //set the strip to solid black so there isn't a weird transition
  //to the next effect.
  Sequence[SequencLen].State = STATE_SOLID;
  Sequence[SequencLen].Settings.colouor1 = COLOUR_BLACK;
  Sequence[SequencLen].Settings.wait = 1;
  SequencLen++;
  
  //rand
  Sequence[SequencLen].State = STATE_RAND;
  Sequence[SequencLen].Settings.itterations = 1000;
  Sequence[SequencLen].Settings.wait = 20;
  SequencLen++;
  
  //set the strip to solid black so there isn't a weird transition
  //to the next effect.
  Sequence[SequencLen].State = STATE_SOLID;
  Sequence[SequencLen].Settings.colouor1 = COLOUR_BLACK;
  Sequence[SequencLen].Settings.wait = 1;
  SequencLen++;
  
  //wheel
  Sequence[SequencLen].State = STATE_WHEEL;
  Sequence[SequencLen].Settings.itterations = 2;
  Sequence[SequencLen].Settings.wait = 50;
  SequencLen++;
  
  //set the strip to solid black so there isn't a weird transition
  //to the next effect.
  Sequence[SequencLen].State = STATE_SOLID;
  Sequence[SequencLen].Settings.colouor1 = COLOUR_BLACK;
  Sequence[SequencLen].Settings.wait = 1;
  SequencLen++;
  
  //rainbow
  Sequence[SequencLen].State = STATE_RAINBOW;
  Sequence[SequencLen].Settings.itterations = 2;
  Sequence[SequencLen].Settings.wait = 50;
  SequencLen++;
  
  Serial.print("Loading default sequence, length = ");
  Serial.println(SequencLen);
}

uint8_t sequenceno = 0;
void sequenceUpdate(void)
{
  //if the end of the sequence has been reached, restart it at the begginging
  if(sequenceno == SequencLen)
  {
    sequenceno = SequencLen - 6;
  }
  
  //if the comms interrupted the sequence with a new macro to run, load it and
  //run it now
  if(cmdMacroFlag == 1)
  {
    cmdMacroFlag = 0;
    lightingstate = cmdSettings.State;
    memcpy(&Settings, &cmdSettings.Settings, sizeof(SETTINGS));
    return;
  }
  
  //execute the next item in the sequence
  lightingstate = Sequence[sequenceno].State;
  memcpy(&Settings, &Sequence[sequenceno].Settings, sizeof(SETTINGS));
  //Settings = &Sequence[sequenceno].Settings;
  sequenceno++;
}

