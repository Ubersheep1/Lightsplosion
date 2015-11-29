#include "definitions.h"

SEQUENCE cmdSettings = {0};
uint8_t  cmdMacroFlag = 0;
//this is used to indicate if a macro should be run immediately, or if the
//current item in the sequence should be allowed to complete first.
uint8_t  cmdImmediateFlag = 0;

void cmdParser(char* cmd, uint8_t Location)
{
  char* Local = cmd;
  uint8_t cmdCode = cmdSelector(&Local);
  
  switch(cmdCode)
  {
    case STATE_HELP:
    {
      printCMDhelp(cmdSelector(&Local), Location);
      break;
    }
    case STATE_MACRO:
    {
      for(uint8_t i=0; i<5; i++)
      {
        //get the op code of the 5 arguments
        uint8_t optCode = optcode_Selector(&Local);
        if(optionParser(optCode, &Local, Location) == OPP_FAIL)
        {
          outputPrint("Option not recognised: ", Location);
          outputPrint(Local, Location);
          printCMDhelp(STATE_MACRO, Location);
          return;
        }
      }
      //tell the sequencer that there is a cmd macro to run
      cmdMacroFlag = 1;
      
      //if the immediade flag is set, abort the current macro
      if(cmdImmediateFlag)
        lightingstate = STATE_RELEASE;
      break;
    }
    case STATE_THCHASE:
    case STATE_COLOURWIPE:
    case STATE_WHEEL:
    case STATE_RAINBOW:
    case STATE_RAND:
    case STATE_SOLID:
    {
      outputPrintLn("This is a macro name, try help + the macro name for more info", Location);
      break;
    }
    case STATE_UNKNOWN:
    default:
    {
      outputPrint("Command not found: ", Location);
      outputPrint(cmd, Location);
      outputPrintLn(" try \"help\" instead.", Location);
      break;
    }
  }
}

uint8_t cmdSelector(char** cmd)
{
  uint8_t retValue = STATE_UNKNOWN;
  
  //if the end of the buffer is reached, return
  if((**cmd) == 0)
    return(STATE_END);
  
  if(memcmp((*cmd), "help", 4) == 0)
  {
    (*cmd)+=4;
    retValue = STATE_HELP;
  }
  else if(memcmp((*cmd), "macro", 5) == 0)
  {
    (*cmd)+=5;
    retValue = STATE_MACRO;
  }
  else if(memcmp((*cmd), "thchase", 7) == 0)
  {
    (*cmd)+=7;
    retValue = STATE_THCHASE;
  }
  else if(memcmp((*cmd), "cwipe", 5) == 0)
  {
    (*cmd)+=5;
    retValue = STATE_COLOURWIPE;
  }
  else if(memcmp((*cmd), "wheel", 5) == 0)
  {
    (*cmd)+=5;
    retValue = STATE_WHEEL;
  }
  else if(memcmp((*cmd), "raibow", 6) == 0)
  {
    (*cmd)+=6;
    retValue = STATE_RAINBOW;
  }
  else if(memcmp((*cmd), "rand", 4) == 0)
  {
    (*cmd)+=4;
    retValue = STATE_RAND;
  }
  else if(memcmp((*cmd), "solid", 5) == 0)
  {
    (*cmd)+=5;
    retValue = STATE_SOLID;
  }
  
  //look for a space after the command, if there is one, skip over it
  if((**cmd) == ' ')
    (*cmd)++;
  
  return(retValue);
}

uint8_t optcode_Selector(char **cmd)
{
  uint8_t retValue = OPTCODE_UNKNOWN;
  
  //if this is the end of the options, indicate it
  if((**cmd) == 0)
    return(OPTCODE_END);
  
  if(memcmp((*cmd), "-n", 2) == 0)
  {
    (*cmd)+=2;
    retValue = OPTCODE_N;
  }
  else if(memcmp((*cmd), "-c1", 3) == 0)
  {
    (*cmd)+=3;
    retValue = OPTCODE_C1;
  }
  else if(memcmp((*cmd), "-c2", 3) == 0)
  {
    (*cmd)+=3;
    retValue = OPTCODE_C2;
  }
  else if(memcmp((*cmd), "-w", 2) == 0)
  {
    (*cmd)+=2;
    retValue = OPTCODE_W;
  }
  else if(memcmp((*cmd), "-i", 2) == 0)
  {
    (*cmd)+=2;
    retValue = OPTCODE_I;
  }
  else if(memcmp((*cmd), "-imm", 4) == 0)
  {
    (*cmd)+=4;
    retValue = OPTCODE_IMM;
  }
  
  //if there is a space after the option, pass over it
  if((**cmd) == ' ')
    (*cmd)++;
  
  //else, return the code for the command which was parsed.
  return(retValue);
}

uint8_t optionParser(uint8_t optCode, char** cmd, uint8_t Location)
{
  switch(optCode)
  {
    case OPTCODE_N:
    {
      //this setting is the macro name, which we can find using the cmd selector
      //this is why the main LED state machine and the cmd parser use the same
      //enum states
      cmdSettings.State = cmdSelector(cmd);
      //if the macro name given is unknown, or there was none give, throw an error
      if((cmdSettings.State == STATE_UNKNOWN) || (cmdSettings.State == STATE_END))
        return(OPP_FAIL);
      break;
    }
    case OPTCODE_C1:
    {
      if((**cmd) == 0)
        return(OPP_FAIL);
      return(colourParser(cmd, &cmdSettings.Settings.colouor1, Location));
    }
    case OPTCODE_C2:
    {
      if((**cmd) == 0)
        return(OPP_FAIL);
      return(colourParser(cmd, &cmdSettings.Settings.colouor2, Location));
    }
    case OPTCODE_W:
    {
      //fix this for now
      cmdSettings.Settings.wait = 50;
      break;
    }
    case OPTCODE_I:
    {
      cmdSettings.Settings.itterations = 2;
      break;
    }
    case OPTCODE_IMM:
    {
      cmdImmediateFlag = 1;
      break;
    }
    case OPTCODE_UNKNOWN:
    {
      return(OPP_FAIL);
    }
  }
  return(OPP_SUCCESS);
}

uint8_t colourParser(char** cmd, uint32_t *colour, uint8_t Location)
{
  char temp[30];
  if(memcmp((*cmd), "rgb(", 4) == 0)
  {
    (*cmd)+=4;
    uint8_t r=0,g=0,b=0;
    scanf((*cmd), "%d,%d,%d)", &r, &g, &b);
    sprintf(temp, "Colour: r=%d g=%d b=%b", r, g, b);
    outputPrintLn(temp, Location);
    (*colour) = strip.Color(r,g,b);
    return(OPP_SUCCESS);
  }
  return(OPP_FAIL);
}

void printCMDhelp(uint8_t cmdCode, uint8_t Location)
{
  switch(cmdCode)
  {
    case STATE_HELP:
    {
      outputPrintLn("What you do right, is type google into google! Be careful though,", Location);
      outputPrintLn("I hear it breaks the internet", Location);
      break;
    }
    case STATE_MACRO:
    {
      outputPrintLn("macro -n name -c1 colour [-c2 colour] -w time -i itterations [-imm]", Location);
      outputPrintLn("Type help and the name of a macro for more details.", Location);
      outputPrintLn("The supported macros are:", Location);
      outputPrintLn("    thchase  -  Theatre style chase", Location);
      outputPrintLn("    cwipe    -  Colour wipe", Location);
      outputPrintLn("    wheel    -  All LEDs the same colour, the colour changes", Location);
      outputPrintLn("    raibow   -  Colour wheel unrolled along the LED strip", Location);
      outputPrintLn("    rand     -  Each LED is a random colour", Location);
      outputPrintLn("    solid    -  Set all the LEDs to a static colour", Location);
      break;
    }
    case STATE_THCHASE:
    {
      outputPrintLn("Theatre style chase program. The built in red chase sequence is done like this:", Location);
      outputPrintLn("macro -n thchase -c1 rgb(255,0,0) -w 75 -i 20  ", Location);
      outputPrintLn("  -c1:  The colour to chase. You can specify in three formats:", Location);
      outputPrintLn("        rgb(<r>,<g>,<b>), wheel(<0 to 768>) or HEX code (eg. FFFFFF = White)", Location);
      outputPrintLn("  -w:   The time in milliseconds between each LED transition", Location);
      outputPrintLn("  -i:   The number of repeats", Location);
      outputPrintLn("  -imm: Execute the macro now, don't wait for the current one to finish", Location);
      break;
    }
    case STATE_COLOURWIPE:
    {
      break;
    }
    case STATE_WHEEL:
    case STATE_RAINBOW:
    case STATE_RAND:
    case STATE_SOLID:
    default:
    {
      printMainHelp(Location);
      break;
    }
  }
}

void printMainHelp(uint8_t Location)
{
  outputPrintLn("Welcome to lighting control 1.0", Location);
  outputPrintLn("The following commands are supported:", Location);
  outputPrintLn("help   :  Print this help, add a command or macro name for specific help", Location);
  outputPrintLn("macro  :  Call a macro to run on the LEDs", Location);
}

void outputPrintLn(char* buffer, uint8_t Location)
{
  if(Location == OUTPUTTO_NETWORK)
  {
    server.println(buffer);
  }
  else if(Location == OUTPUTTO_SERIAL)
  {
    Serial.println(buffer);
  }
}

void outputPrint(char* buffer, uint8_t Location)
{
  if(Location == OUTPUTTO_NETWORK)
  {
    server.print(buffer);
  }
  else if(Location == OUTPUTTO_SERIAL)
  {
    Serial.print(buffer);
  }
}

