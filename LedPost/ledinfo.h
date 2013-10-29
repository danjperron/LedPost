#ifndef LEDINFO

#define LEDINFO


#define MAX_LED  4
#define MAX_CHANNEL (MAX_LED * 3) 


typedef union {
   struct  {
    unsigned char Array[MAX_LED];
   };  
   struct {
      unsigned char Led1R;
      unsigned char Led1G;
      unsigned char Led1B;
      unsigned char Led2R;
      unsigned char Led2G;
      unsigned char Led2B;
      unsigned char Led3R;
      unsigned char Led3G;
      unsigned char Led3B;
      unsigned char Led4R;
      unsigned char Led4G;
      unsigned char Led4B;
  };
}  LedsChannelUnion;

extern unsigned char LedPWMCounter;
extern LedsChannelUnion LedsChannel;
extern unsigned short RGB555Led[MAX_LED];



// Led pack version RGB555




 #define LED1R_INIT()    {TRISAbits.TRISA0 = 0; LATAbits.LATA0 = 1;}
 #define LED1G_INIT()    {TRISAbits.TRISA1 = 0; LATAbits.LATA1 = 1;}
 #define LED1B_INIT()    {TRISAbits.TRISA2 = 0; LATAbits.LATA2 = 1;}

 #define LED2R_INIT()    {TRISAbits.TRISA3 = 0; LATAbits.LATA3 = 1;}
 #define LED2G_INIT()    {TRISAbits.TRISA5 = 0; LATAbits.LATA5 = 1;}
 #define LED2B_INIT()    {TRISCbits.TRISC0 = 0; LATCbits.LATC0 = 1;}

 #define LED3R_INIT()    {TRISCbits.TRISC1 = 0; LATCbits.LATC1 = 1;}
 #define LED3G_INIT()    {TRISCbits.TRISC2 = 0; LATCbits.LATC2 = 1;}
 #define LED3B_INIT()    {TRISCbits.TRISC5 = 0; LATCbits.LATC5 = 1;}

 #define LED4R_INIT()    {TRISBbits.TRISB0 = 0; LATBbits.LATB0 = 1;}
 #define LED4G_INIT()    {TRISBbits.TRISB1 = 0; LATBbits.LATB1 = 1;}
 #define LED4B_INIT()    {TRISBbits.TRISB4 = 0; LATBbits.LATB4 = 1;}


 #define LED1R(A)    LATAbits.LATA0 = A
 #define LED1G(A)    LATAbits.LATA1 = A
 #define LED1B(A)    LATAbits.LATA2 = A

 #define LED2R(A)    LATAbits.LATA3 = A
 #define LED2G(A)    LATAbits.LATA5 = A
 #define LED2B(A)    LATCbits.LATC0 = A

 #define LED3R(A)    LATCbits.LATC1 = A
 #define LED3G(A)    LATCbits.LATC2 = A
 #define LED3B(A)    LATCbits.LATC5 = A

 #define LED4R(A)    LATBbits.LATB0 = A
 #define LED4G(A)    LATBbits.LATB1 = A
 #define LED4B(A)    LATBbits.LATB4 = A


// this is for the irq assembly routine

#define LED1R_PORT LATA
#define LED1R_BIT     0

#define LED1G_PORT LATA
#define LED1G_BIT     1

#define LED1B_PORT LATA
#define LED1B_BIT     2

#define LED2R_PORT LATA
#define LED2R_BIT     3

#define LED2G_PORT LATA
#define LED2G_BIT     5

#define LED2B_PORT LATC
#define LED2B_BIT     0

#define LED3R_PORT LATC
#define LED3R_BIT     1

#define LED3G_PORT LATC
#define LED3G_BIT     2

#define LED3B_PORT LATC
#define LED3B_BIT     5

#define LED4R_PORT LATB
#define LED4R_BIT     0

#define LED4G_PORT LATB
#define LED4G_BIT     1

#define LED4B_PORT LATB
#define LED4B_BIT     4


#endif
