#ifndef MAX7219_H_
#define MAX7219_H_

#include <REGX51.H>

/**** MAX7219驱动管脚 ****/
sbit pinDIN  = P3^0;   //串行数据输入端
sbit pinLOAD = P3^1;   //数据锁存端
sbit pinCLK  = P3^2;   //时钟输入端
/*************************/

typedef enum
{
    NoOp = 0x0,
    Digit0 = 0x1,
    Digit1 = 0x2,
    Digit2 = 0x3,
    Digit3 = 0x4,
    Digit4 = 0x5,
    Digit5 = 0x6,
    Digit6 = 0x7,
    Digit7 = 0x8,
    DecodeMode = 0x9,
    Intensity = 0xa,
    ScanLimit = 0xb,
    Shutdown = 0xc,
    DisplayTest = 0xf,
} MAX7219Addr;

typedef unsigned char uint8;

extern void init7219(void);
#ifndef NO_SENIOR_7219_FUNC
extern void set7219Intensity(uint8 level);
extern void set7219ScanLimit(uint8 limit);
extern void displayTest7219(bit test);
extern void startup7219(void);
extern void shutdown7219(void);
extern void digitDisplay7219(uint8 digit, uint8 dat);
#endif // NO_SENIOR_7219_FUNC

extern void send7219Data(MAX7219Addr addr, uint8 dat);

#endif // MAX7219_H_
