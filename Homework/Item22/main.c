#include <REGX51.H>

#define Led P0

#define forever for(;;)

typedef unsigned char uint8;
typedef unsigned short uint16;

const uint8 code DigCode[10] = { // 共阳数码管段码
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90
};//  0     1     2     3     4     5     6     7     8     9

void delay(void);

int main(void)
{
    uint8 num = 0;

    forever
    {
        Led = DigCode[num++];
        if(num==10) num = 0;
        delay();
    }

    return 0;
}

void delay(void)
{
    volatile uint16 i = 0xffff;
    while(--i);
}
