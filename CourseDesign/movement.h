#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "utils.h"

typedef enum _Direction
{
    Forward,
    Right,
    Left,
} Direction;

extern void runAsUsual(void);
extern void turn(Direction direct);
extern void speedUpOrSlowDown(Bool speedUp);
extern Direction currentDirection(void);
extern Direction oppositeDirection(Direction direct);

#endif // MOVEMENT_H_
