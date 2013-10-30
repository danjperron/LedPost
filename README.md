LedPost
============

  Simple CanOpen module which control 4 Leds on Fence Lamp Post.
  
  It could be use for light chaser  or any kind of light control.
  
  Up to 127 Modules could be connected on a single CANBUS system.
  
  
  Base source code from sourceforge  (GPL License)
    http://sourceforge.net/projects/canopennode/files/canopennode/CANopenNode-1.10/
    
  Base code modified to accept microchip PIC18F25(26)K80.
  
  
  Canbus speed is set to 125KBS but could be modified using  ODD_CANbitRate  from CO_OD.h (need recompile).

  Protocol is CanOpen and  it uses 4 RPDOs signal
  
  
  COB-ID 0x200+NodeID (RPDO_0)  and  COB-ID 0x200 (RPDO_2)  receive information for individual led.
  
    Each Led iformation is a unsigned 16 bits integer containing  32 levels of red, green and blue.
    The bit15 specify if the data is linear or curve using a gamma factor of 1.5.
  
    CANBUS packet for COB-ID 0x200..0x7F  is 8 bytes subdivise in 4 unsigned short, each led info,  which
    - bit0..4   Red   Channel 32 levels value
    - bit5..9   Green Channel 32 levels value
    - bit10..14 Blue  Channel 32 levels value
    - bit15     linear (0)  or gamma 1.5 function (1)
    
    
  COB-ID 0x300+NodeID (RPDO_1) and COB-ID 0x300   receive 3 byte of information for all led has a hole.
  
    - byte 0    Red   Channel 256 levels value
    - byte 1    Green Channel 256 levels value
    - byte 2    Blue  Channel 256 levels value
    
    
  This is a trade off. Control each led using 32 levels RGB or control all of them using the same color but at 256 levels RGB
  
  
  
  The code has been compile for the PIC18F25K80 and it will work without modification on the PIC18F26K80.
        
  I'm using Microchip MATLAB IDE V8.90 with C18 compiler
  
  The current code , if you are using the .MCP project has to be in
  
  c:\CanOpenNode\\_src  
  
  You will have to check the Compiler path Also.
  
  If you start with your own project , be sure that you include "USER_ISR_HIGH" definition in the preprocessor macros.
  
  
  
  
  
  
