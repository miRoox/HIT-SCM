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
    MeasureInterval0 = 50000u, // 50ms/t0
    MeasureInterval1 = 4u,     // 200ms
};

void init(void)
{
    initMessageQueue();
    TMOD = M16BT0; // T0 16位定时器
    TH0 = T0IntervalH = T16BH(MeasureInterval0);
    TL0 = T0IntervalL = T16BL(MeasureInterval0);
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
    static uint8 cnt1 = MeasureInterval1;
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
                cnt1 = MeasureInterval1;
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
