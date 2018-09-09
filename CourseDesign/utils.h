#ifndef UTILS_H_
#define UTILS_H_

#include <STDDEF.H>

typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned char byte;

#define forever for(;;)

enum _Boolean
{
    False = 0,
    True  = 1
};
#define Bool bit

extern void delayus(uint16 t);

#endif // UTILS_H_
