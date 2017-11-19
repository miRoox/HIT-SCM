#include <REGX51.H>

#define KeyBoard P1
#define Digit    P2

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

uint8 digitBuffer = NoDigit;

uint8 keyScan(void);
void delay(void);

int main(void)
{
    uint8 keyVal = InvalidKey;

    forever
    {
        if((keyVal=keyScan())!=InvalidKey)
        {
            digitBuffer = DigitCode[keyVal];
        }
        Digit = digitBuffer;
    }

    return 0;
}

uint8 keyScan(void)
{
    uint8 keyVal = InvalidKey;

    KeyBoard = 0xf0;
    // delay();
    if(KeyBoard!=0xf0)
    {
        delay();
        if((KeyBoard&0xf0)!=0xf0)
        {
            uint8 i = 0xff;
            KeyBoard = 0xf0;
            // delay();
            switch((KeyBoard&0xf0))
            {
                case(0x70): keyVal = 3; break;
                case(0xb0): keyVal = 2; break;
                case(0xd0): keyVal = 1; break;
                case(0xe0): keyVal = 0; break;
                default: return InvalidKey;
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
                default: return InvalidKey;
            }
            for(i = 0xff;i && (KeyBoard!=0x0f);--i)	 // 按键松手检测
			{
				delay();
			}
        }
    }

    return keyVal;
}

void delay(void)
{
    volatile uint8 i = 0xff;
    while(--i);
}
