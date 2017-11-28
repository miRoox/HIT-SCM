#include <REGX51.H>

#define DAOut P1
sbit DACS = P2^0;
sbit DAWR1 = P2^1;

#define forever for(;;)

typedef unsigned char uint8;

void delay(void);

int main(void)
{
    uint8 i = 0;

    DACS = 0;  // 片选
    DAWR1 = 0; // 一直写入

    forever
    {
        DAOut = i++;
        delay();
    }

    return 0;
}

void delay(void)
{
    volatile uint8 i = 0xff;
    do {
        volatile uint8 j =0xff;
        while(--j);
    }
    while(--i);
}
