#include <REGX52.H>
#include "utils.h"
#include "message.h"
#include "movement.h"
#include "scan.h"

#define FREQ 11.0592

#include "timer.h"

void init(void);
void onTimeout(uint8 id);

int main(void)
{
    init();
    forever processMessage();
    return 0;
}

enum //Constant
{
    T0Interval = 50000u,       // 50ms/T0
    MeasureIntervalExtra = 3u, // 150ms
};

void init(void)
{
    initMessageQueue();
    TMOD = (M16BT0 | M16BT1 | GATE1);
    TH0 = T0IntervalH = T16BH(T0Interval);
    TL0 = T0IntervalL = T16BL(T0Interval);
    ET0 = 1; // 允许定时器T0中断
    EA = 1;  // 允许中断
    TR0 = 1; // 启动定时器T0运行
}

void messageHandler(MessageType msgtype, byte msgdata)
{
    switch (msgtype)
    {
    case Idle:
        runAsUsual();
        break;
    case Timeout:
        onTimeout(msgdata);
        break;
    case TurnTo:
        turn(msgdata);
        break;
    case TryScan:
        tryScan(msgdata);
        break;
    default:
        break;
    }
}

// -- timeout --

void onTimeout(uint8 id)
{
    static uint8 cnt1 = MeasureIntervalExtra;
    if (id == 0)
    {
        if (cnt1 > 0)
        {
            --cnt1;
        }
        else
        {
            if (postMessage(TryScan, Measure))
            {
                cnt1 = MeasureIntervalExtra;
            }
        }
    }
}

// -- utils --

void delayus(uint16 t)
{
    t /= 10;
    while (--t);
}
