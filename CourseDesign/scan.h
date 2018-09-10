#ifndef SCAN_H_
#define SCAN_H_

#include "utils.h"

typedef enum _ScanAction
{
    Measure,
    Start,
    Next,
    Finish,
    Restart,
} ScanAction;

enum // Constant
{
    MaxRangingDistance = 450,
};

extern void tryScan(ScanAction action);
extern void scanRecord(void);
extern uint16 getDistance(void);

#endif // SCAN_H_
