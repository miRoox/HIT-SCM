#include <REGX51.H>

#define LED P2

#define forever for(;;)

typedef unsigned short uint16;

void delay(void);

int main(void)
{
    LED = 0;

    forever
    {
        if(LED==0) LED = 0xff; // 共阳，低电平有效
        LED <<= 1; // 从上到下
        delay();
    }

    return 0;
}

void delay(void)
{
    volatile uint16 i = 0xffff;
    while(--i);
}
