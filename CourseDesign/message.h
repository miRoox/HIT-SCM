#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "utils.h"

typedef enum _MessageType
{
    /* internal */
    Idle = 0,
    Timeout, // data: timer id
    /* user-defined */
    TurnTo,  // data: direction
    TryScan, // data: scan action
    Repaint, // data: repaint area
} MessageType;

extern byte data T0IntervalH;
extern byte data T0IntervalL;
extern byte data T1IntervalH;
extern byte data T1IntervalL;

extern void initMessageQueue(void);
extern void processMessage(void);
extern Bool postMessage(MessageType msgtype, byte msgdata);

extern void messageHandler(MessageType msgtype, byte msgdata); // user defined

#endif // MESSAGE_H_
