#include "scan.h"
#include <REGX52.H>
#include "timer.h"
#include "message.h"
#include "movement.h"

sbit ECHO = P3 ^ 3; // INT1
sbit TRIG = P1 ^ 2;

enum // Constant
{
    Margin = 20u,       //20cm
    SafeDistance = 50u, // 50cm
    BackStep = 25u,
    ScanRange = 90u,   // about 90deg
};

static enum ScanState {
    NotScan,
    ScanOne,
    ToStraight,
    ScanAnother,
    ToBest,
} state = NotScan;

static uint8 counter = 0;
static uint16 distance = 0;

static struct BestDistance
{
    uint16 distance;
    uint8 position;
    Direction direct;
} best = {0, 0, Right};

static void startScan(void);
static void nextState(void);
static void finishScan(void);
static void scanRecord(void);
static void measure(void);
static void ranging(void);

void tryScan(ScanAction action)
{
    switch (action)
    {
    case Measure:
        measure();
        break;
    case Record:
        scanRecord();
        break;
    case Start:
        startScan();
        break;
    case Next:
        nextState();
        break;
    case Finish:
        finishScan();
        break;
    default:
        break;
    }
}

static void scanRecord(void)
{
    switch (state)
    {
    case ScanOne:
    case ScanAnother:
        if (counter < ScanRange)
        {
            ++counter;
        }
        else
        {
            postMessage(TryScan, Next);
        }
        break;
    case ToStraight:
    case ToBest:
        if (counter > 0)
        {
            --counter;
        }
        else
        {
            postMessage(TryScan, Next);
        }
        break;
    default:
        break;
    }
}

//uint16 getDistance(void)
//{
//    return distance;
//}

static void measure(void)
{
    ranging();
    if (state == ScanOne || state == ScanAnother)
    {
        if (distance > SafeDistance)
        {
            postMessage(TryScan, Finish);
        }
        else if (distance > best.distance)
        {
            best.distance = distance;
            best.position = counter;
            best.direct = currentDirection();
        }
    }
    else if (state == NotScan)
    {
        if (distance <= Margin)
        {
            postMessage(TryScan, Start);
        }
        else if(distance > SafeDistance)
        {
            speedUpOrSlowDown(True);
        }
        else
        {
            speedUpOrSlowDown(False);
        }
    }
}

static void startScan(void)
{
    if (state == NotScan)
    {
        state = ScanOne;
        counter = 0;
        postMessage(TurnTo, best.direct);
    }
}

static void nextState(void)
{
    switch (state)
    {
    case ScanOne:
        state = ToStraight;
        postMessage(TurnTo,oppositeDirection(currentDirection()));
        break;
    case ToStraight:
        counter = 0;
        state = ScanAnother;
        break;
    case ScanAnother:
        if (best.distance >= Margin)
        {
            if (best.direct == currentDirection())
            {
                counter = ScanRange - best.position;
            }
            else
            {
                counter = ScanRange + best.position;
            }
            postMessage(TurnTo,oppositeDirection(currentDirection()));
            state = ToBest;
        }
        else // best.distance < Margin
        {
            finishScan();
            startScan(); // restart scan
        }
        break;
    default:
        finishScan();
        break;
    }
}

static void finishScan(void)
{
    if (state != NotScan)
    {
        state = NotScan;
        counter = 0;
        best.distance = 0;
        best.position = 0;
        best.direct = currentDirection(); // for next selection
        postMessage(TurnTo, Forward);
    }
}

static void ranging(void)
{
    TH1 = TL1 = 0;
	TRIG = 0;
    TRIG = 1;
    delayus(10);
    TRIG = 0;
    while (ECHO);
    while (!ECHO);
	TR1 = 1;
    while (ECHO);
    TR1 = 0;
    distance = TH1 * 0x100u + TL1; // us
    distance /= 58;                // us -> cm
}
