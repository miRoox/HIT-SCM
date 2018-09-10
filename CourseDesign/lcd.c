#include "lcd.h"
#include <ctype.h>
#include <REGX52.H>

#define LCD1602_DATAPINS P1
sbit LCD1602_E = P2 ^ 7;
sbit LCD1602_RW = P2 ^ 5;
sbit LCD1602_RS = P2 ^ 6;

#ifdef LCD1602_4PINS
#define HI4BITS(x) ((x)&0xf0)
#define LO4BITS(x) ((x) << 4)
#endif

static void delay(uint16 t);
static void lcdWrite(bit op, byte dat);

/*!
    lcd initialization
*/
void lcdInit(void)
{
#ifndef LCD1602_4PINS
    lcdWrite(OP_COMD, CMD_8PINS | CMD_2LINE);
    lcdWrite(OP_COMD, CMD_NOCUR); // 开显示不显示光标
    // lcdWrite(OP_COMD,CMD_CURRT);    // 写一个指针加1，这个好像是自动的
    lcdWrite(OP_COMD, CMD_CLS);
    lcdWrite(OP_COMD, DDRAMADD(0x00)); // 设置数据指针起点
#else
    lcdWrite(OP_COMD, 0x32); // 将8位总线转为4位总线
    lcdWrite(OP_COMD, CMD_2LINE);
    lcdWrite(OP_COMD, CMD_NOCUR); // 开显示不显示光标
    // lcdWrite(OP_COMD,CMD_CURRT);    // 写一个指针加1，这个好像是自动的
    lcdWrite(OP_COMD, CMD_CLS);
    lcdWrite(OP_COMD, DDRAMADD(0x00)); // 设置数据指针起点
#endif
}

/*!
    write command to lcd
*/
void lcdWriteComd(byte cmd)
{
    lcdWrite(OP_COMD, cmd);
}

/*!
    write data to lcd
*/
void lcdWriteData(byte dat)
{
    lcdWrite(OP_DATA, dat);
}

/*!
    wait when lcd is busy
*/
void lcdWaitBusy(void)
{
    byte dat;
#ifndef LCD1602_4PINS

    do
    {

        LCD1602_E = 0;
        delay(3);

        LCD1602_RS = OP_COMD;
        LCD1602_RW = 1; //选择读出
        LCD1602_DATAPINS = 0xff;
        LCD1602_E = 1;
        dat = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
    } while (dat & 0x80);
#else
    byte buf;
    LCD1602_E = 0;
    delay(3);

    LCD1602_RS = OP_COMD;
    LCD1602_RW = 1; //选择读出

    do
    {
        LCD1602_DATAPINS |= 0xf0; //高四位置一
        delay(1);
        LCD1602_E = 1;
        dat = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
        dat &= 0xf0; //取高位

        LCD1602_DATAPINS |= 0xf0; //高四位置一
        delay(1);
        LCD1602_E = 1;
        buf = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
        dat |= (buf >> 4); //得到低位
    } while (dat & 0x80);
#endif
}

/*!
    put character to lcd
*/
void lcdPutchar(char c)
{
    if (isprint(c))
    {
        lcdWriteData(c);
    }
    else if (c == '\n')
    {
        lcdMoveCursor(SCDLINE);
    }
}

/*!
    put string to lcd
*/
void lcdPuts(const char *str)
{
    for (; *str; ++str)
    {
        lcdPutchar(*str);
    }
}

/*!
    put digit to lcd
*/
void lcdPutDigit(uint16 num)
{
    uint16 d = 1;
    while (num / d >= 10)
        d *= 10;
    while (d != 0)
    {
        lcdPutchar(num / d + '0');
        num %= d;
        d /= 10;
    }
}

/*!
    \internal
*/
void delay(uint16 t)
{
    do
    {
        volatile uint8 i = 10;
        while (--i);
    } while (--t);
}

/*!
    \internal
*/
void lcdWrite(bit op, byte dat) //写入命令/数据
{
    lcdWaitBusy();

    LCD1602_E = 0;   //使能清零
    LCD1602_RS = op; //选择发送命令|数据
    delay(1);
    LCD1602_RW = 0; //选择写入

#ifndef LCD1602_4PINS
    LCD1602_DATAPINS = dat; //放入命令/数据
    delay(1);               //等待数据稳定

    LCD1602_E = 1; //写入时序
    delay(5);      //保持时间
    LCD1602_E = 0;

#else
    LCD1602_DATAPINS &= 0x0f;
    LCD1602_DATAPINS = HI4BITS(dat); //发送高四位
    delay(1);

    LCD1602_E = 1; //写入时序
    delay(5);      //保持时间
    LCD1602_E = 0;

    LCD1602_DATAPINS = LO4BITS(dat); //发送低四位
    delay(1);

    LCD1602_E = 1; //写入时序
    delay(5);      //保持时间
    LCD1602_E = 0;

#endif
}
