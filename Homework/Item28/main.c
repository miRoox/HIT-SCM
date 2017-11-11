#include <REGX51.H>

#define Led P1

#define FREQ 12 // 晶振频率12MHz

#define forever for(;;)

typedef unsigned char uint8;

#include "timer.h" // 关于定时器的一些常量和便利宏

enum // Constant
{
    TimerInterval = 50000U, // 定时器 50ms
    DesignPrecisionPerInterval = 10, // 精度 0.5s
};

int main(void)
{
    TMOD = M16BT0;  // T0 16位定时器
    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);
    ET0 = 1;  // 允许定时器T0中断
    EA = 1;   // 允许中断

    Led = 0;
    TR0 = 1; // 启动定时器运行

    forever;

    return 0;
}

void timeout(void) interrupt 1 using 1 // T0
{
    static uint8 timePerInterval = 0;

    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);

    if(timePerInterval<DesignPrecisionPerInterval)
    {
        ++timePerInterval;
    }
    else
    {
        Led = ~Led;
        timePerInterval = 0;
    }
}
