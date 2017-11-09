#include <REGX51.H>

sbit Led = P1^0;
sbit Key = P3^2;

#define FREQ 12 // 晶振频率12MHz

#define forever for(;;)
    
typedef unsigned char uint8;

#include "timer.h" // 关于定时器的一些常量和便利宏

enum //Constant
{
    TimerInterval = 50000U, // 定时器 50ms
    ToggleTime = 20, // 触发时间 1s = 20*50ms
    Pressed = 0,
};

int main(void)
{
    Led = 1; // 共阳
    
    TMOD = M16BT0;  // T0 16位定时器
    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);
    ET0 = 1;  // 允许定时器T0中断
    EA = 1;   // 允许中断
    
    while(Key!=Pressed); // wait for start
    TR0 = 1; // 启动定时器运行
    
    forever;
    
    return 0;
}

void timeout(void) interrupt 1 // T0
{
    static uint8 times = 0;
    
    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);
    
    if(times<ToggleTime)
    {
        ++times;
    }
    else
    {
        Led = ~Led;
        times = 0;
    }
}
