#include "movement.h"
#include <REGX52.H>
#include "utils.h"
#include "message.h"
#include "scan.h"

sbit RightWheel = P1 ^ 0;
sbit LeftWheel = P1 ^ 1;

static Direction direction = Forward;
static Bool isSpeedUp = True;

#define runWith(r, l)        \
    do                       \
    {                        \
        RightWheel = 1;      \
        delayus(1500 - (r)); \
        RightWheel = 0;      \
        delayus(20000);      \
        LeftWheel = 1;       \
        delayus(1500 + (l)); \
        LeftWheel = 0;       \
        delayus(20000);      \
    } while (0)

void turn(Direction direct)
{
    if (direct == direction)
        return;
    runWith(0, 0); // stop then turn
    direction = direct;
}

void speedUpOrSlowDown(Bool speedUp)
{
    isSpeedUp = speedUp;
}

Direction currentDirection(void)
{
    return direction;
}

Direction oppositeDirection(Direction direct)
{
    switch (direct)
    {
    case Right:
        return Left;
    case Left:
        return Right;
    default:
        break;
    }
    return Forward;
}

void runAsUsual(void)
{
    switch (direction)
    {
    case Forward:
        if (isSpeedUp)
            runWith(200, 200);
        else
            runWith(100, 100);
        break;
    case Right:
        runWith(-200, 200);
        break;
    case Left:
        runWith(200, -200);
        break;
    default:
        break;
    }
    postMessage(TryScan, Record);
}
