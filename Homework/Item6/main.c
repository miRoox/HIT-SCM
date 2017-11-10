#include <REGX51.H>
#include <intrins.h>

#define LED P2

#define forever for(;;)

typedef unsigned char uint8;
typedef unsigned short uint16;

void delay(void) reentrant;
void progress(void);
void flash(void);

int main(void)
{
    EX1=1;  // 允许外部INT1中断
    IT1=0;  // INT1电平触发
    EA=1;   // 允许中断

    forever
    {
        progress();
    }

    return 0;
}

void progress(void)
{
    static uint8 led = 1;
    static bit toLeft = 0;
    LED = led; // 共阴
    if(toLeft)
    {
        led >>= 1;
        if(led==1) toLeft = 0;
    }
    else
    {
        led <<= 1;
        if(led==(1<<7)) toLeft = 1;
    }
    delay();
}

void flash(void)
{
    static uint8 led = 0x55; //0b01010101
    LED = led;
    led = _crol_(led,1);
    delay();
}

void onPress(void) interrupt 2 using 1 // INT1
{
    flash();
}

void delay(void) reentrant
{
    volatile uint16 i = 0xffff;
    while(--i);
}

