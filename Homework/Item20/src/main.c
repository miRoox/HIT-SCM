#include <REGX51.H>
#include "max7219.h"

#define buttons P1 // 抢答按钮
sbit setTimeKey = P2^0; // 设置/清除时间
sbit startKey = P2^1; // 开始
sbit bee = P3^7; // 蜂鸣器

#define FREQ 12 // 晶振频率12MHz

#define forever for(;;)

//typedef unsigned char uint8; // defined in max7219.h
typedef unsigned short uint16;

#include "timer.h" // 关于定时器的一些常量和便利宏

const uint8 code DigCode[10] = { // 共阴数码管段码，高低位排序: dp,a,b,c,d,e,f,g
    0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b
};//  0     1     2     3     4     5     6     7     8     9

enum
{
    Pressed = 0,
    TimerInterval = 50000U, // 定时器 50ms
    DesignPrecisionPerInterval = 20, // 精度 1s
    MaxRangePerInterval = 30*DesignPrecisionPerInterval, // 范围 30s
};

void setting(void);
void beep(void);

volatile uint16 settingRange = MaxRangePerInterval;
volatile uint16 timePerInterval = MaxRangePerInterval;

int main(void)
{
    init7219();
    set7219ScanLimit(3);
    // init timer
    TMOD = M16BT0;  // T0 16位定时器
    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);
    ET0 = 1;  // 允许定时器T0中断
    EA = 1;   // 允许中断

    setting();
    timePerInterval = settingRange;
    TR0 = 1; // 启动定时器运行

    forever
    {
        if(TR0==1) // 抢答进行中
        {
            static uint8 keyNum = 0xff;
            if((keyNum=buttons)!=0xff) // 有按键按下
            {
                TR0 = 0; // 中止
                digitDisplay7219(0,DigCode[(timePerInterval/DesignPrecisionPerInterval)/10]);
                digitDisplay7219(1,DigCode[(timePerInterval/DesignPrecisionPerInterval)%10]);
                digitDisplay7219(2,DigCode[0]);
                switch(keyNum)
                {
				case(0xFE):	  // S0
                    digitDisplay7219(3,DigCode[1]);
					break;
				case(0xFD):	  // S1
					digitDisplay7219(3,DigCode[2]);
					break;
				case(0xFB):	  // S2
					digitDisplay7219(3,DigCode[3]);
					break;
				case(0xF7):	  // S3
					digitDisplay7219(3,DigCode[4]);
					break;
				case(0xEF):	  // S4
					digitDisplay7219(3,DigCode[5]);
					break;
				case(0xDF):	  // S5
					digitDisplay7219(3,DigCode[6]);
					break;
				case(0xBF):	  // S6
					digitDisplay7219(3,DigCode[7]);
					break;
				case(0x7F):	  // S7
					digitDisplay7219(3,DigCode[8]);
					break;
				default:
                    digitDisplay7219(2,1); // '-'
					digitDisplay7219(3,1); // '-'
					break;
                }
                beep();
            }
        }
        else // 抢答已中止
        {
            if(setTimeKey==Pressed)
            {
                timePerInterval = settingRange; // 清零计时
                digitDisplay7219(0,DigCode[0]);
                digitDisplay7219(1,DigCode[0]);
                digitDisplay7219(2,0);
                digitDisplay7219(3,0);
                while(setTimeKey==Pressed); // 长按无效
            }
            else if(startKey==Pressed)
            {
                if(timePerInterval==settingRange)
                {
                    digitDisplay7219(0,DigCode[(timePerInterval/DesignPrecisionPerInterval)/10]);
                    digitDisplay7219(1,DigCode[(timePerInterval/DesignPrecisionPerInterval)%10]);
                    TR0 = 1;
                }
            }
        }
    }

    return 0;
}

void timeout(void) interrupt 1 using 1 // T0
{
    TH0 = T16BH(TimerInterval);
    TL0 = T16BL(TimerInterval);

    if(timePerInterval>0)
    {
        --timePerInterval;
    }
    else
    {
        beep();
        TR0 = 0; // 超时中止
    }
    if(timePerInterval%DesignPrecisionPerInterval==0)
    {
        // 注意 digitDisplay7219() 不可重入
        digitDisplay7219(0,DigCode[(timePerInterval/DesignPrecisionPerInterval)/10]);
        digitDisplay7219(1,DigCode[(timePerInterval/DesignPrecisionPerInterval)%10]);
    }
}

void setting(void)
{
    digitDisplay7219(0,DigCode[(settingRange/DesignPrecisionPerInterval)/10]);
    digitDisplay7219(1,DigCode[(settingRange/DesignPrecisionPerInterval)%10]);
    while(startKey!=Pressed)
    {
        if(setTimeKey==Pressed)
        {
            if(settingRange==MaxRangePerInterval)
            {
                settingRange = 0;
            }
            else
            {
                settingRange += DesignPrecisionPerInterval;
            }
            digitDisplay7219(0,DigCode[(settingRange/DesignPrecisionPerInterval)/10]);
            digitDisplay7219(1,DigCode[(settingRange/DesignPrecisionPerInterval)%10]);
            while(setTimeKey==Pressed); // 长按无效
        }
    }
}

void beep(void)
{
    uint16 j;
    uint8 k;
	for(j=0;j<1000;++j)
    {
		for(k=0;k<20;++k)
        {
			bee=0;
		}
		for(k=0;k<20;++k)
        {
			bee=1;
		}
	}
}
