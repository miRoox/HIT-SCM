#ifndef MOVEMENT_H_
#define MOVEMENT_H_

typedef enum _Direction
{
    Forward,
    Right,
    Left,
} Direction;

extern void runAsUsual(void);
extern void turn(Direction direct);
extern Direction currentDirection(void);
extern Direction oppositeDirection(Direction direct);

#endif // MOVEMENT_H_
