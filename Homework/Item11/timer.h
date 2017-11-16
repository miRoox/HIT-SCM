#ifndef TIMER_H_
#define TIMER_H_

#ifndef FREQ
#error Need 'FREQ' as the crystal frequency.
#endif // FREQ

enum // TMOD
{
    GATE0 = 0x08,
    GATE1 = 0x80,
    CONT0 = 0x04, // 相反则为定时器
    CONT1 = 0x40,
    M13BT0 = 0x00, // 13位定时/计数器
    M13BT1 = 0x00,
    M16BT0 = 0x01, // 16位定时/计数器
    M16BT1 = 0x10,
    M8BTA0 = 0x02, // 8位自动重装定时/计数器
    M8BTA1 = 0x20,
    M8BTSP = 0x03, // T0分成两个独立的8位定时/计数器
};

#define LOBYTE(x) ((unsigned char)((x)&0x00ffU))
#define HIBYTE(x) ((unsigned char)((x)>>8))

// 定时器定时计算 单位：us
#define T8BH(x)  HIBYTE((unsigned short)(0x100-(x)/(12/FREQ)))
#define T8BL(x)  LOBYTE((unsigned short)(0x100-(x)/(12/FREQ)))
#define T13BH(x) HIBYTE((unsigned short)(0x2000-(x)/(12/FREQ)))
#define T13BL(x) LOBYTE((unsigned short)(0x2000-(x)/(12/FREQ)))
#define T16BH(x) HIBYTE((unsigned short)(0x10000-(x)/(12/FREQ)))
#define T16BL(x) LOBYTE((unsigned short)(0x10000-(x)/(12/FREQ)))

#endif // TIMER_H_
