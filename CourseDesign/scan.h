#ifndef SCAN_H_
#define SCAN_H_

#include "utils.h"

typedef enum _ScanAction
{
    Measure,
    Start,
    Next,
    Finish,
} ScanAction;

extern void tryScan(ScanAction action);
extern void scanRecord(void);

#endif // SCAN_H_
