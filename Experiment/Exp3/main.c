#include <REGX51.H>
#include <intrins.h>

#define LED P1

#define forever for(;;)

typedef unsigned short uint16;

void delay(void) reentrant;

int main(void)
{
    LED = 0; // 共阳，低电平有效

    EX0=1;  // 允许外部INT0中断
    IT0=0;  // INT0电平触发
    EA=1;   // 允许中断

    forever
    {
        LED = 0;
        delay();
        LED = ~0;
        delay();
    }

    return 0;
}

void onTrigger(void) interrupt 0 // INT0
{
    static led = ~1;
    LED = led;
    delay();
    led = _crol_(led,1);
}

void delay(void) reentrant
{
    volatile uint16 i = 0xffff;
    while(--i);
}
