#ifndef SCAN_H_
#define SCAN_H_

#include "utils.h"

typedef enum _ScanAction
{
    Measure,
    Record,
    Start,
    Next,
    Finish,
} ScanAction;

extern void tryScan(ScanAction action);

#endif // SCAN_H_
