#include <REGX51.H>
#include <absacc.h>

#define con8255 XBYTE[0x7003]
#define pA8255 XBYTE[0x7000]
#define pB8255 XBYTE[0X7001]
#define pC8255 XBYTE[0x7002]
sbit reset8255 = P3^5;

#define forever for(;;)

typedef unsigned char uint8;

void delay(void);

int main(void)
{
    reset8255 = 1;  // 使用前必须复位
    delay();
    reset8255 = 0;
    con8255 = 0x82; // 0b10000010 PA方式0输出；PB方式0输入
    forever
    {
        pA8255 = pB8255;
    }

    return 0;
}

void delay(void)
{
    volatile i = 0xff;
    while(--i);
}
