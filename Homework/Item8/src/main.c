#include <REGX51.H>

#define LED P1

#define forever for(;;)

typedef unsigned short uint16;

void delay(void);

int main(void)
{
    LED = 0; // 共阳，低电平有效
    
    EX0=1;  // 允许外部INT0中断
    IT0=1;  // INT0边沿触发
    EA=1;   // 允许中断
    
    forever
    {
        LED = 0;
    }
    
    return 0;
}

void onTrigger(void) interrupt 0 // INT0
{
    LED = 0xaa; //0b10101010
    delay();
    LED = 0x55; //0b01010101
    delay();
}

void delay(void)
{
    volatile uint16 i = 0xffff;
    while(--i);
}
