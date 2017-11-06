#include "max7219.h"
#include <intrins.h>

static void sendByte(uint8 dat);

/*!
    low level api.

    send dat to register addr of MAX7219.
*/
void send7219Data(MAX7219Addr addr, uint8 dat)
{
    pinLOAD=0;
    _nop_();
    sendByte(addr);
    _nop_();
    sendByte(dat);
    _nop_();
    pinLOAD=1; //第16个上升沿之后，第17个上升沿之前必须把pinLOAD置高，否则数据丢失
}

/*!
    normal initialize MAX7219.
*/
void init7219(void)
{
    send7219Data(Shutdown,0x01);    // 设置电源工作模式
    send7219Data(Intensity,0x09);   // 设置亮度  19/32
    send7219Data(ScanLimit,0x03);   // 设置扫描界限 0~7
    send7219Data(DecodeMode,0x00);  // 设置译码模式
    send7219Data(DisplayTest,0x00); // 正常工作状态
}

#ifndef NO_SENIOR_7219_FUNC

/*!
    Intensity control.
    Digital control of display brightness is provided by an internal pulse-width modulator, 
    which is controlled bythe lower nibble of the intensity register. 
    The modulator scales the average segment current in 16 steps from
    a maximum of 31/32 down to 1/32 of the peak current set by RSET.

    level can be 0x0 ~ 0xf
*/
void set7219Intensity(uint8 level)
{
    send7219Data(Intensity,level);
}

/*!
    scan limit control.
    The scan-limit register sets how many digits are displayed, from 1 to 8. 
    They are displayed in a multiplexed manner with a typical display scan rate of 800Hz with 8 digits displayed. 
    If fewer digits are displayed, the scan rate is 8fOSC/N, where N is the number of digits scanned.
    Since the number of scanned digits affects the display brightness, 
    the scan-limit register shouldnot be used to blank portions of the display (such as leading zero suppression). 

    limit can be 0 ~ 7
*/
void set7219ScanLimit(uint8 limit)
{
    send7219Data(ScanLimit,limit);
}

/*!
    The display-test register operates in two modes: normaland display test. 
    Display-test mode turns all LEDs onby overriding, 
    but not altering, all controls and digit reg-isters (including the shutdown register). 
    In display-testmode, 8 digits are scanned and the duty cycle is 31/32.
*/
void displayTest7219(bit test)
{
    if(_testbit_(test)) send7219Data(DisplayTest,0x01);
    else send7219Data(DisplayTest,0x00);
}

void startup7219(void)
{
    send7219Data(Shutdown,0x01);
}

void shutdown7219(void)
{
    send7219Data(Shutdown,0x00);
}

/*!
    dat segment:

    | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 |
    | dp |  a |  b |  c |  d |  e |  f |  g |
*/
void digitDisplay7219(uint8 digit, uint8 dat)
{
    send7219Data(digit+Digit0,dat);
}

#endif // NO_SENIOR_7219_FUNC

/*!
    \internal
*/
void sendByte(uint8 dat)
{
    uint8 i,temp;
    _nop_();
    for(i=0;i<8;i++)
    {
        temp=dat&0x80;
        dat=dat<<1;
        if(temp)
           pinDIN=1;
        else
           pinDIN=0;
        pinCLK=0;
        _nop_();
        pinCLK=1;
    }
}
