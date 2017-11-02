#include <REGX51.H>

#define LED P2

#define forever for(;;)

typedef unsigned short uint16;

void delay(void);

#ifndef IMPROVE

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

#else // 实验的改进

volatile bit stopped;

int main(void)
{
    LED = 0;
    stopped = 0;

    EX0=1;  // 允许外部INT0中断
    IT0=1;  // INT0边沿触发
    EA=1;   // 允许中断

    forever
    {
        if(!stopped)
        {
            if(LED==0) LED = 0xff; // 共阳，低电平有效
            LED <<= 1; // 从上到下
            delay();
        }
    }

    return 0;
}

void onToggle(void) interrupt 0 // INT0
{
    stopped = ~stopped;
}

#endif

void delay(void)
{
    volatile uint16 i = 0xffff;
    while(--i);
}
