#include "movement.h"
#include <REGX52.H>
#include "utils.h"
#include "scan.h"

sbit RightWheel = P1 ^ 0;
sbit LeftWheel = P1 ^ 1;

static Direction direction = Forward;

static void runWith(int r, int l)
{
    RightWheel = 1;
    delayus(1500 - r);
    RightWheel = 0;
    delayus(20000);
    LeftWheel = 1;
    delayus(1500 + l);
    LeftWheel = 0;
    delayus(20000);
}

void turn(Direction direct)
{
    direction = direct;
}

Direction currentDirection(void)
{
    return direction;
}

void runAsUsual(void)
{
    switch (direction)
    {
    case Forward:
        runWith(200, 200);
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
    scanRecord();
}
