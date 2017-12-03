#include <REGX51.H>

#define DigitSegment P0 // 数码管段选
#define DigitPlace   P2 // 数码管位选
#define KeyBoard     P1 // 矩阵键盘

#define forever for(;;)

typedef unsigned char uint8;

const uint8 code DigitCode[16] = {  // 共阳
    0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e
};// 0    1    2    3    4    5    6    7    8    9    A    b    C    d    E    F

enum
{
    InvalidKey = 16,
    NoDigit = 0xff,
};

bit aboutToSend = 0;
bit received = 0;
uint8 dataToSend;
uint8 receivedData;

uint8 digitBuffer[8];

void digitDisplay(void);
uint8 keyScan(void);
void serialSend(uint8 dat);
void delay(void);

int main(void) using 0
{
    // init digitBuffer
    uint8 i;
    for(i=0;i<8;++i)
    {
        digitBuffer[i] = NoDigit;
    }
    // init KeyBoard
    KeyBoard = 0xf0;

    SCON = 0X50;     // 方式1 + 允许串口接收
    TMOD = 0X20;     // T1 8位自动重装定时器
    PCON |= 0X80;
    TL1  = 0Xfa;     // 波特率 9600
    TH1  = 0Xfa;
    IT0 = 1;
    IT1 = 1;
    EX0 = 1;
    EX1 = 1;
    ES = 1;
    EA = 1;
    TR1 = 1;

    forever
    {
        if(aboutToSend)
        {
            digitBuffer[1] = DigitCode[dataToSend];
        }
        if(received)
        {
            digitBuffer[6] = DigitCode[receivedData];
        }
        digitDisplay();
    }

    return 0;
}

void onKeyDown(void) interrupt 0 using 1 // INT0
{
    uint8 keyVal = keyScan();
    if(keyVal!=InvalidKey)
    {
        aboutToSend = 1;
        dataToSend = keyVal;
    }
}

void onTrigger(void) interrupt 2 using 2 // INT1
{
    if(aboutToSend)
    {
        serialSend(dataToSend);
    }
}

void serialRecieve(void) interrupt 4 using 3 // RX
{
    RI = 0;
    receivedData = SBUF;
    received = 1;
}

void digitDisplay(void)
{
    uint8 i = 0;
    for(;i<8;++i)
    {
        volatile uint8 j;

        DigitPlace = (1<<i);
        DigitSegment = digitBuffer[i];

        for(j=10;--j;);    // 扫描间隔时间
        DigitSegment = NoDigit;
    }
}

uint8 keyScan(void)
{
    uint8 keyVal = InvalidKey;

    KeyBoard = 0xf0;
    // delay();

    if((KeyBoard&0xf0)!=0xf0)
    {
        KeyBoard = 0xf0;
        // delay();
        switch((KeyBoard&0xf0))
        {
            case(0x70): keyVal = 3; break;
            case(0xb0): keyVal = 2; break;
            case(0xd0): keyVal = 1; break;
            case(0xe0): keyVal = 0; break;
            default: keyVal = InvalidKey; goto ret;
        }
        KeyBoard = 0xff;
        // delay();
        KeyBoard = 0x0f;
        // delay();
        switch((KeyBoard&0x0f))
        {
            case(0x07): keyVal += 12; break;
            case(0x0b): keyVal += 8; break;
            case(0x0d): keyVal += 4; break;
            case(0x0e): keyVal += 0; break;
            default: keyVal = InvalidKey; goto ret;
        }
    }

ret:
    KeyBoard = 0xf0;    // restore
    return keyVal;
}

void serialSend(uint8 dat)
{
    SBUF = dat;
    while(!TI);
    TI = 0;
}

// void delay(void)
// {
    // volatile uint8 i = 0xff;
    // while(--i);
// }
