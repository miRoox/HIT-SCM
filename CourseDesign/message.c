#include "message.h"
#include <REGX52.H>
#include "utils.h"

// -- timer --

byte data T0IntervalH = 0;
byte data T0IntervalL = 0;
byte data T1IntervalH = 0;
byte data T1IntervalL = 0;

static bit flagT0 = False;
static bit flagT1 = False;

void timeoutT0(void) interrupt 1
{
    TH0 = T0IntervalH;
    TL0 = T0IntervalL;
    flagT0 = True;
}

void timeoutT1(void) interrupt 3
{
    TH1 = T1IntervalH;
    TL1 = T1IntervalL;
    flagT1 = True;
}

// -- message --

enum
{
    MessageSize = 8u
};

typedef struct _Message
{
    MessageType type;
    byte mdata;
} Message;

static struct _MessageQueue
{
    uint8 head;
    uint8 tail;
    Message msgs[MessageSize];
} idata msgQueue;

void initMessageQueue(void)
{
    msgQueue.head = msgQueue.tail = 0;
}

Bool postMessage(MessageType msgtype, byte msgdata)
{
    uint8 tailNext = (msgQueue.tail + 1) % MessageSize;
    if (msgtype == Idle || msgtype == Timeout) // generate by system
        return False;
    if (tailNext != msgQueue.head)
    {
        msgQueue.tail = tailNext;
        msgQueue.msgs[msgQueue.tail].type = msgtype;
        msgQueue.msgs[msgQueue.tail].mdata = msgdata;
        return True;
    }
    return False;
}

void processMessage(void)
{
    Message pop;
    if (flagT0)
    {
        flagT0 = False;
        messageHandler(Timeout, 0);
    }
    else if (flagT1)
    {
        flagT1 = False;
        messageHandler(Timeout, 1);
    }
    else if (msgQueue.head == msgQueue.tail)
    {
        messageHandler(Idle, 0);
    }
    else
    {
        pop = msgQueue.msgs[msgQueue.head];
        msgQueue.head = (msgQueue.head + 1) % MessageSize;
        messageHandler(pop.type, pop.mdata);
    }
}
