#include <REGX52.H>
#include "utils.h"
#include "message.h"
#include "movement.h"
#include "scan.h"
#include "lcd.h"

#define FREQ 11.0592

#include "timer.h"

typedef enum _RepaintArea
{
    DistanceArea,
    DirectionArea,
} RepaintArea;

void init(void);
void onTimeout(uint8 id);
void repaint(RepaintArea area);

int main(void)
{
    init();
    forever processMessage();
    return 0;
}

enum //Constant
{
    T0Interval = 50000u, // 50ms/T0
    MeasureIntervalExtra = 3u,     // 150ms
    RepaintIntervalExtra = 7u, // 350ms 
};

void init(void)
{
    initMessageQueue();
    lcdInit();
    TMOD = M16BT0; // T0 16位定时器
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
        postMessage(Repaint,DirectionArea);
        break;
    case TryScan:
        tryScan(msgdata);
        break;
    case Repaint:
        repaint(msgdata);
        break;
    default:
        break;
    }
}

// -- lcd repaint --

void repaintDistance(void)
{
    uint16 distance = getDistance();
    lcdMoveCursor(0);
    if (distance < MaxRangingDistance)
    {
        lcdPuts("Distance:");
        lcdPutDigit(distance);
        lcdPuts("cm");
    }
    else
    {
        lcdPuts("Out of Range!");
    }
}

void repaintDirection(void)
{
    lcdMoveCursor(SCDLINE + 3);
    switch (currentDirection())
    {
    case Forward:
        lcdPuts("Go Forward");
        break;
    case Right:
        lcdPuts("Turn Right");
        break;
    case Left:
        lcdPuts("Turn Left");
        break;
    default:
        break;
    }
}

void repaint(RepaintArea area)
{

    switch (area)
    {
    case DistanceArea:
        repaintDistance();
        break;
    case DirectionArea:
        repaintDirection();
        break;
    default:
        break;
    }
}

// -- timeout --

void onTimeout(uint8 id)
{
    static uint8 cnt1 = MeasureIntervalExtra;
    static uint8 cnt2 = RepaintIntervalExtra;
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
        if (cnt2 > 0)
        {
            --cnt2;
        }
        else
        {
            postMessage(Repaint, DistanceArea);
            cnt2 = RepaintIntervalExtra;
        }
    }
}

// -- utils --

void delayus(uint16 t)
{
    t /= 10;
    while (--t);
}
