#include <REGX51.H>

#define DIG_INT P0 // 数码管整数部分
#define DIG_DEC P2 // 数码管小数部分

#define FREQ 12 // 晶振频率12MHz

#define forever for(;;)

typedef unsigned char uint8;
typedef unsigned short uint16;

#include "timer.h" // 关于定时器的一些常量和便利宏

const uint8 code DigCode[10] = { // 共阴数码管段码
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};//  0     1     2     3     4     5     6     7     8     9

enum // Constant
{
    TimerInterval = 50000U, // 定时器 50ms
    DesignPrecisionPerInterval = 2, // 精度 0.1s
    DesignRangePerInterval = 99*DesignPrecisionPerInterval, // 范围 9.9s
    DotCode = 0x80, // 数码管 '.' 段码
};

void updateDisplay(void);
void updateState(void);

volatile enum
{
    Clear = 0,
    Run,
    Stop,
} state = Clear;

volatile uint8 timePerInterval = 0;

int main(void)
{
    TMOD = M16BT0;  // T0 16位定时器
	TH0 = T16BH(TimerInterval);
	TL0 = T16BL(TimerInterval);
	IT0 = 1;  // INT0边沿触发
	PX0 = 1;  // INT0高优先级
    EX0 = 1;  // 允许外部INT0中断
    ET0 = 1;  // 允许定时器T0中断
    EA = 1;   // 允许中断

    updateState();
    updateDisplay();
    forever;

    return 0;
}

void onTrigger(void) interrupt 0 using 1 // INT0
{

    if(state!=Stop) ++state;
    else state = Clear;
    updateState();
    updateDisplay();
}

void timeout(void) interrupt 1 using 2 // T0
{
    TH0 = T16BH(TimerInterval);
	TL0 = T16BL(TimerInterval);
    if(timePerInterval<DesignRangePerInterval)
    {
        ++timePerInterval;
    }
    else
    {
        state = Stop;
    }
    updateState();
    updateDisplay();
}

void updateDisplay(void)
{
    DIG_DEC = DigCode[(timePerInterval/DesignPrecisionPerInterval)%10];
    DIG_INT = DigCode[(timePerInterval/DesignPrecisionPerInterval)/10] | DotCode;
}

void updateState(void)
{
    switch(state)
    {
        case Clear:
            TR0 = 0; // 停止定时器运行
            timePerInterval = 0; // 清零
            break;
        case Run:
            TR0 = 1; // 启动定时器运行
            break;
        case Stop:
            TR0 = 0; // 停止定时器运行
            break;
    }
}
