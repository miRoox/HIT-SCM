#include "scan.h"
#include <REGX52.H>
#include "timer.h"
#include "message.h"
#include "movement.h"

sbit ECHO = P3 ^ 3; // INT1
sbit TRIG = P1 ^ 2;

// --

enum // Constant
{
    Margin = 12u,       //12cm
    SafeDistance = 25u, // 25cm
    ScanRange = 100u,   // about 90deg
};

static enum ScanState {
    NotScan,
    ScanOne,
    Backup,
    ScanAnother,
    ToBest,
} state = NotScan;

static uint8 counter = 0;

static struct BestDistance
{
    uint16 distance;
    uint8 position;
    Direction direct;
} best = {0, 0, Right};

static void startScan(void);
static void nextState(void);
static void finishScan(void);
static void measure(void);
static uint16 getDistance(void);

void tryScan(ScanAction action)
{
    switch (action)
    {
    case Measure:
        measure();
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

void scanRecord(void)
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
    case Backup:
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

static void measure(void)
{
    uint16 distance = getDistance();
    if (state == Backup || state == ToBest)
        return;
    if (state != NotScan)
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
    else if (distance <= Margin)
    {
        postMessage(TryScan, ScanOne);
    }
}

static void startScan(void)
{
    if (state == NotScan)
    {
        state = ScanOne;
        postMessage(TurnTo, best.direct);
    }
}

static void nextState(void)
{
    switch (state)
    {
    case ScanOne:
        state = Backup;
        break;
    case Backup:
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
            state = ToBest;
        }
        else // best.distance < Margin
        {
            postMessage(TryScan, Finish);
            postMessage(TryScan, Start); // restart scan
        }
        break;
    default:
        postMessage(TryScan, Finish);
        break;
    }
}

static void finishScan(void)
{
    if (state == NotScan)
        return;
    state = NotScan;
    counter = 0;
    best.distance = 0;
    best.position = 0;
    best.direct = currentDirection(); // counter for next selection
    postMessage(TurnTo, Forward);
}

static uint16 getDistance(void)
{
    uint16 distance = 0;
    TMOD |= (M16BT1 | GATE1);
    TH1 = TL1 = 0;
    TRIG = 1;
    delayus(10);
    TRIG = 0;
    while (ECHO);
    TR1 = 1;
    while (!ECHO);
    while (ECHO);
    TR1 = 0;
    TMOD &= ~(M16BT1 | GATE1);
    distance = TH1 * 0x100u + TL1; // us
    distance /= 58;                // us -> cm
    return distance;
}
