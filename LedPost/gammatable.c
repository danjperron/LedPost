#include "gammatable.h"


// linear table to convert 32 levels into 256  LED levels
// factor  is 255/31 = 8.2258
rom unsigned char LinearTable[32]={
   0,   8,  16,  25,  33,  41,  49,  58,
  66,  74,  82,  90,  99, 107, 115, 123,
 132, 140, 148, 156, 165, 173, 181, 189,
 197, 206, 214, 222, 230, 239, 247, 255 
};

// Gamma 1.5 table to convert 32 levels into 256 LED levels
//
//
// Gamma function is
//
//
//   f(x)  =  CFactor *  x^gamma
//
//   where CFactor =  31/31^gamma * 255/31
//   CFactor for 1.5 gamma is 1,4774
//   ex: level 15 in linear = 123
//         and in 1.5 gamma = 1.4774 *  15^1.5 = 86


rom unsigned char GammaTable[32]= {
   0,   1,   4,   8,  12,  17,  22,  27,
  33,  40,  47,  54,  61,  69,  77,  86,
  95, 104, 113, 122, 132, 142, 152, 163,
 174, 185, 196, 207, 219, 231, 243, 255
};


   

