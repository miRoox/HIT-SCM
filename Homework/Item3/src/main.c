#include <REGX51.H>

#define LED P0

#define forever for(;;)

typedef unsigned int uint8;
typedef unsigned short uint16;

enum
{
    false = 0,
    true = 1
};

void delay(void);

int main(void)
{
    bit toLeft = true;
    uint8 light = 1 << 7;

    forever
    {
        LED = ~light; // 共阳，低电平有效
        delay();
        if(toLeft)
        {
            light >>= 1;
            if(light==1) toLeft = false;
        }
        else
        {
            light <<= 1;
            if(light==(1<<7)) toLeft = true;
        }
    }

    return 0;
}

void delay(void)
{
    volatile uint16 i = 0xffff;
    while(--i);
}
