#include <REGX51.H>

sbit SW1 = P3^5;

#define FREQ 11.0592

#define forever for(;;)
#define waitForRelease(key) while(key==0)

typedef unsigned char uint8;

enum
{
    Pressed = 0,
};

void serialSend(uint8 dat);
void delay(void);

int main(void)
{
    SCON = 0X50;            // 设置为工作方式 1
    TMOD = 0X20;            // 设置计数器工作方式 2

#if FREQ==12
    PCON |= 0x80;
    TH1 = 0Xf3;           // 计数器初始值设置，波特率 4800
    TL1 = 0Xf3;
#elif FREQ==11.0592
    PCON &= ~0x80;
    TH1 = 0Xfa;           // 计数器初始值设置，波特率 4800
    TL1 = 0Xfa;
#else
#   error bad FREQ
#endif

    TR1=1;                      // 打开计数器

    forever
    {
        if(SW1==Pressed)
        {
            serialSend(1);
            delay();  // 消抖
            waitForRelease(SW1);
        }
    }

    return 0;
}

void serialSend(uint8 dat)
{
    SBUF = dat;
    while(!TI);
    TI=0;
}

void delay(void)
{
    volatile uint8 i = 0xff;
    do {
        volatile uint8 j = 0xff;
        while(--j);
    }
    while(--i);
}
