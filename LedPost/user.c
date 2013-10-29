/*******************************************************************************

   User functions

*******************************************************************************/

#include "CANopen.h"

#include <delays.h>
#include "ledinfo.h"
#include "gammatable.h"



// Timer
unsigned short Timersec;
unsigned short Timerms;
unsigned char SecFlag;

void SetRGBToAllLeds(unsigned char R, unsigned char G, unsigned char B);
void UnpackRGB555(unsigned short RGB555Value, unsigned char * LedChannelValue);
void  UnpackAllLeds(unsigned short *data, unsigned char * LedChannelsValue);

void  DecodeAllLeds(unsigned short *data);
void  User_PrepareTPDO(unsigned char i);
void User_Process_RPDO(void);

/******** FLASH DATA  *****/

// NodeID is store on flash offset 4

#pragma romdata eedata_scn=0xf00000
  rom sODE_EEPROM  rom_flash={0,ODD_CANnodeID};   //first PowerOnCounter, and second is NodeID
 #pragma romdata





/*******************************************************************************
   User_Init - USER INITIALIZATION OF NODE
   Function is called after start of program.
*******************************************************************************/
void User_Init(void){
int loop;
ADCON0 =0;
ADCON1 =0xF;  // disable analog
ODE_EEPROM.PowerOnCounter++;
 Timersec=0;
 Timerms=0;
 SecFlag=0;
 // let's set the I/O for the LED
LED1R_INIT() 
LED1G_INIT()
LED1B_INIT() 

LED2R_INIT() 
LED2G_INIT()
LED2B_INIT() 

LED3R_INIT() 
LED3G_INIT()
LED3B_INIT() 

LED4R_INIT() 
LED4G_INIT()
LED4B_INIT() 

//let's reset the LED INFO
for(loop=0;loop<MAX_LED;loop++)
 UnpackRGB555(0,&LedsChannel.Array[loop*3]);
SetRGBToAllLeds(0,0,0);




// OK timer 1 setup fo

   TMR1H=0x0;
// T1CON= 0x01;  // /1 prescaler
// use T1 prescaler /1
 T1CON= 0x41;  
//enable timer interrupts
   PIR1bits.TMR1IF = 0;
   IPR1bits.TMR1IP = 1;  //High riority interrupt
   PIE1bits.TMR1IE = 1;


}

/*******************************************************************************
   User_Remove - USER EXECUTION ON EXIT OF PROGRAM
   Function is called before end of program. Not used in PIC.
*******************************************************************************/
void User_Remove(void){

}

/*******************************************************************************
   User_ResetComm - USER RESET COMMUNICATION
   Function is called after start of program and after CANopen NMT command: Reset
   Communication.
*******************************************************************************/
void User_ResetComm(void){

}

/*******************************************************************************
   User_ProcessMain - USER PROCESS MAINLINE
   This function is cyclycally called from main(). It is non blocking function.
   It is asynchronous. Here is longer and time consuming code.
*******************************************************************************/
void User_ProcessMain(void){
 
User_Process_RPDO();
 
}

/*******************************************************************************
   User_Process1msIsr - 1 ms USER TIMER FUNCTION
   Function is executed every 1 ms. It is deterministic and has priority over
   mainline functions.
*******************************************************************************/
void User_Process1msIsr(void){

  Timerms++;

  if(Timerms>=1000)
   {
     Timerms=0;
     Timersec++;
     SecFlag=1;
  }
}

void User_Process_RPDO(void){
 unsigned char R,G,B;

 // let's check if we receive RPDO(0)
 // Specific Node RGB555 data
  if(CO_RPDO_New(0))
   {
       // ok we got a new Led information
      CO_DISABLE_CANRX_TMR();
      RGB555Led[0] = CO_RPDO(0).WORD[0];
      RGB555Led[1] = CO_RPDO(0).WORD[1];
      RGB555Led[2] = CO_RPDO(0).WORD[2];
      RGB555Led[3] = CO_RPDO(0).WORD[3];
      CO_RPDO_New(0) = 0;
      CO_ENABLE_CANRX_TMR();
      UnpackAllLeds(RGB555Led,LedsChannel.Array);
   }
 
 // let's check if we receive RPDO(1)
 //
 //  Specific Node All Leds RGB value  
  if(CO_RPDO_New(1))
     {
       CO_DISABLE_CANRX_TMR();
       R = CO_RPDO(1).BYTE[0];
       G = CO_RPDO(1).BYTE[1];
       B = CO_RPDO(1).BYTE[2];
       CO_RPDO_New(1) = 0;
       CO_ENABLE_CANRX_TMR();
       SetRGBToAllLeds(R,G,B);
     }


 // All Node RGB555 data 
 if(CO_RPDO_New(2))
   {
       // ok we got a new Led information
      CO_DISABLE_CANRX_TMR();
      RGB555Led[0] = CO_RPDO(2).WORD[0];
      RGB555Led[1] = CO_RPDO(2).WORD[1];
      RGB555Led[2] = CO_RPDO(2).WORD[2];
      RGB555Led[3] = CO_RPDO(2).WORD[3];
      CO_RPDO_New(2) = 0;
      CO_ENABLE_CANRX_TMR();
     UnpackAllLeds(RGB555Led,&LedsChannel.Array[0]);
   }

  // RPDO(3)
  //  Allc Node All Leds RGB value  
  //  
  if(CO_RPDO_New(3))
     {
       CO_DISABLE_CANRX_TMR();
       R = CO_RPDO(3).BYTE[0];
       G = CO_RPDO(3).BYTE[1];
       B = CO_RPDO(3).BYTE[2];
       CO_RPDO_New(3) = 0;
       CO_ENABLE_CANRX_TMR();
       SetRGBToAllLeds(R,G,B);
     }
}



 
#ifdef __LARGE__
#pragma   MSG_LARGE_MODEL
#define  ROMVOID far rom void
#endif

#ifdef __SMALL__
#pragma   MSG_SMALL_MODEL
#define  ROMVOID rom void 
#endif


void  User_PrepareTPDO(unsigned char i)
{
  // no TPDO defined yet
}


// Unpack RGB555  led info
// bit 0..4   Red Led (5bits)
// bit 5..9   Green Led (5bits)
// bit 10..14 Blue Led (5 bits)
// bit 15     Linear=0 or Gamma=1 (gamma 1.5)


void UnpackRGB555(unsigned short RGB555Value, unsigned char * LedChannelValue)
{
   int loop;
   unsigned char idx;
   unsigned char GammaFlag= (RGB555Value & 0x8000)== 0x8000 ? 1 : 0;
   // convert each R,G and B  5 bits channel to full 8 bits
   // by shifting 5 bits a the time
   for(loop=0;loop<3;loop++)
    {
     idx = RGB555Value & 0x1f;
     *(LedChannelValue++) =  GammaFlag ? GammaTable[idx] :LinearTable[idx];
     RGB555Value >>=5;
    }
}

void  UnpackAllLeds(unsigned short *data, unsigned char * LedChannelsValue)
{
   int loop;
   
   for(loop=0;loop<MAX_LED;loop++)
     {
       UnpackRGB555((*data++),LedChannelsValue);
       LedChannelsValue+=3;
     }
}


void SetRGBToAllLeds(unsigned char R, unsigned char G, unsigned char B)
{
      unsigned char loop;
      unsigned char *pt;

      pt = & LedsChannel.Array[0];
      for(loop=0;loop<4;loop++)
      {
         *(pt++) = R;
         *(pt++) = G;
         *(pt++) = B;
      }
}


// Timer 1 interrupt period  for LED PWM
//
//  8 bits = 256 level. At 60Hz we need TMR1H at 0xFE00

#define TMR1_INITH 0xFC

#define USE_ASSEMBLY_IRQ

#ifndef USE_ASSEMBLY_IRQ
void User_IsrLow(void)
{
// ok let's check if it is the timer 1

 if(PIR1bits.TMR1IF){
TMR1H=TMR1_INITH;   //  set MSB byte  to flag count when 1024 is reached. 
PIR1bits.TMR1IF=0;     // ok let's reset the flag

// let's increment the  EndlessCounter (from 0..30  , 31 excluded)

LedPWMCounter++;
if(LedPWMCounter > 254)  // 255 can't exist this mean full ON ;
  LedPWMCounter=0;


// for speed we wil do  each individual channel instead of a loop

LED1R( LedsChannel.Led1R > LedPWMCounter ? 0 : 1);
LED1G( LedsChannel.Led1G > LedPWMCounter ? 0 : 1);
LED1B( LedsChannel.Led1B > LedPWMCounter ? 0 : 1);

LED2R( LedsChannel.Led2R > LedPWMCounter ? 0 : 1);
LED2G( LedsChannel.Led2G > LedPWMCounter ? 0 : 1);
LED2B( LedsChannel.Led2B > LedPWMCounter ? 0 : 1);

LED3R( LedsChannel.Led3R > LedPWMCounter ? 0 : 1);
LED3G( LedsChannel.Led3G > LedPWMCounter ? 0 : 1);
LED3B( LedsChannel.Led3B > LedPWMCounter ? 0 : 1);

LED3R( LedsChannel.Led4R > LedPWMCounter ? 0 : 1);
LED3G( LedsChannel.Led4G > LedPWMCounter ? 0 : 1);
LED3B( LedsChannel.Led4B > LedPWMCounter ? 0 : 1);
}
}

#else

/************************************************************
  Assembly optimisation since Compiler optimization is not enough
 
  linker script 18F26K80 has been modified to create space for
  LedPWMCounter et LedsChannel to be in ACCESS RAM (BANK 0)
  new bank name is  "ledsbank" (use of #pragma udata  ledsbank in ledinfo.c)
  This way we reduce the number of time we need to use MOVLB opcode
  Also we use the compare opcode "CPFSGT" instead of  "subtract".
  This way we only need to load LedPWMCounter once and compare on all

  N.B. Leds I/O  are hardwired to definition in ledinfo.h
***********************************************************/



#pragma interrupt User_IsrHigh
void User_IsrHigh(void)
{


// ok let's check if it is the timer 1
_asm
         BTFSS PIR1,0,ACCESS     //is TMR1 Flag
         GOTO  NO_TMR1IF             // No than exit
         MOVLW TMR1_INITH         // Load default TMR1H value for correct timing
         MOVWF TMR1H,ACCESS
        
        BCF PIR1,0,ACCESS         // ok let's reset the flag

         MOVLB 0					// SET BANK0 GPR and ACCESSRAM below 0x60   just in case

         INCF LedPWMCounter,1,ACCESS  // increment counter
         
        MOVLW 0xFF                      // is LedPWMCounter < 255
        CPFSLT LedPWMCounter,ACCESS
        CLRF     LedPWMCounter,ACCESS     // ok it is bigger than we will clear it
        
        // let's check Led1
        // first load into  LedPWMCounter into W

         MOVF LedPWMCounter,0,ACCESS
         
       // let's compare to Led1 Red Channel
     
LED1R_CHECK:   
         CPFSGT LedsChannel, ACCESS      	// is Led1R > EndlessCounter
         BRA LED1R_SMALLER               	// No is smaller
         BCF LED1R_PORT,LED1R_BIT,ACCESS    // Led 1 Red ON
         BRA LED1G_CHECK
LED1R_SMALLER:
         BSF LED1R_PORT,LED1R_BIT,ACCESS    // Led 1 Red OFF

LED1G_CHECK:
  	     CPFSGT LedsChannel+1,ACCESS        // is Led1G > EndlessCounter
         BRA LED1G_SMALLER                  // No is smaller
         BCF LED1G_PORT,LED1G_BIT,ACCESS    // Led ON
         BRA LED1B_CHECK
LED1G_SMALLER:
         BSF LED1G_PORT,LED1G_BIT,ACCESS    // Led OFF

LED1B_CHECK:
  	     CPFSGT LedsChannel+2,ACCESS        // is Led1B > EndlessCounter
         BRA LED1B_SMALLER                  // No is smaller
         BCF LED1B_PORT,LED1B_BIT,ACCESS    // Led ON
         BRA LED2R_CHECK
LED1B_SMALLER:
         BSF LED1B_PORT,LED1B_BIT,ACCESS    // Led OFF

LED2R_CHECK:
  	     CPFSGT LedsChannel+3,ACCESS        // is Led2R > EndlessCounter
         BRA LED2R_SMALLER                  // No is smaller
         BCF LED2R_PORT,LED2R_BIT,ACCESS    // Led ON
         BRA LED2G_CHECK
LED2R_SMALLER:
         BSF LED2R_PORT,LED2R_BIT,ACCESS    // Led OFF

LED2G_CHECK:
  	     CPFSGT LedsChannel+4,ACCESS        // is Led2G > EndlessCounter
         BRA LED2G_SMALLER                  // No is smaller
         BCF LED2G_PORT,LED2G_BIT,ACCESS    // Led ON
         BRA LED2B_CHECK
LED2G_SMALLER:
         BSF LED2G_PORT,LED2G_BIT, ACCESS   // Led OFF

LED2B_CHECK:
  	     CPFSGT LedsChannel+5,ACCESS        // is Led2B > EndlessCounter
         BRA LED2B_SMALLER                  // No is smaller
         BCF LED2B_PORT,LED2B_BIT,ACCESS    // Led ON
         BRA LED3R_CHECK
LED2B_SMALLER:
         BSF LED2B_PORT,LED2B_BIT,ACCESS    // Led OFF

LED3R_CHECK:
  	     CPFSGT LedsChannel+6,ACCESS        // is Led3R > EndlessCounter
         BRA LED3R_SMALLER                  // No is smaller
         BCF LED3R_PORT,LED3R_BIT,ACCESS    // Led ON
         BRA LED3G_CHECK
LED3R_SMALLER:
         BSF LED3R_PORT,LED3R_BIT,ACCESS    // Led OFF

LED3G_CHECK:
  	     CPFSGT LedsChannel+7,ACCESS        // is Led3G > EndlessCounter
         BRA LED3G_SMALLER                  // No is smaller
         BCF LED3G_PORT,LED3G_BIT,ACCESS    // Led ON
         BRA LED3B_CHECK
LED3G_SMALLER:
         BSF LED3G_PORT,LED3G_BIT,ACCESS    // Led OFF

LED3B_CHECK:
  	     CPFSGT LedsChannel+8,ACCESS        // is Led3B > EndlessCounter
         BRA LED3B_SMALLER                  // No is smaller
         BCF LED3B_PORT,LED3B_BIT,ACCESS    // Led ON
         BRA LED4R_CHECK
LED3B_SMALLER:
         BSF LED3B_PORT,LED3B_BIT,ACCESS    // Led OFF

LED4R_CHECK:
  	     CPFSGT LedsChannel+9,ACCESS        // is Led4R > EndlessCounter
         BRA LED4R_SMALLER                  // No is smaller
         BCF LED4R_PORT,LED4R_BIT,ACCESS    // Led ON
         BRA LED4G_CHECK
LED4R_SMALLER:
         BSF LED4R_PORT,LED4R_BIT,ACCESS    // Led OFF

LED4G_CHECK:
  	     CPFSGT LedsChannel+10,ACCESS       // is Led4G > EndlessCounter
         BRA LED4G_SMALLER                  // No is smaller
         BCF LED4G_PORT,LED4G_BIT,ACCESS    // Led ON
         BRA LED4B_CHECK
LED4G_SMALLER:
         BSF LED4G_PORT,LED4G_BIT,ACCESS    // Led OFF

LED4B_CHECK:        
         CPFSGT LedsChannel+11,ACCESS       // is Led4B > EndlessCounter
         BRA LED4B_SMALLER                  // No is smaller
         BCF LED4B_PORT,LED4B_BIT,ACCESS    // Led ON
         BRA NO_TMR1IF
LED4B_SMALLER:
         BSF LED4B_PORT,LED4B_BIT,ACCESS    // Led OFF
NO_TMR1IF:
_endasm
}

#endif
