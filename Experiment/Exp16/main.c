#include <REGX51.H>

#define Led P0
#define AD_OUT P1
sbit AD_CLK = P2^0;
sbit AD_START = P2^1;
sbit AD_EOC = P2^3;
sbit AD_A = P2^4;
sbit AD_B = P2^5;
sbit AD_C = P2^6;
sbit AD_OE = P2^7;

#define forever for(;;)

typedef unsigned char uint8;

int main(void)
{
    TMOD = 0x02; // T0 方式2: 8位自动重装
    TH0 = 0xf6;  // 50 kHz
    TL0 = 0xf6;
    ET0 = 1;
    EA = 1;

    AD_A = 0;
    AD_B = 0;
    AD_C = 0;  // 通道 IN0

    TR0 = 1;

    forever
    {
        AD_START = 0; // 启动A/D转换
        AD_START = 1;
        AD_START = 0;
        while(AD_EOC==0); // 等待转换完成
        AD_OE = 1;
        Led = AD_OUT;
        AD_OE = 0;
    }

    return 0;
}

void timeout(void) interrupt 1 // T0
{
    AD_CLK = ~AD_CLK;
}
