#include "lcd.h"
#include <ctype.h>

#ifdef LCD1602_4PINS
#   define HI4BITS(x) ((x)&0xf0)
#   define LO4BITS(x) ((x)<<4)
#endif

static void delay(unsigned int t);
static void lcdWrite(bit op, unsigned char dat);

/*!
    lcd initialization
*/
void lcdInit(void)
{
#ifndef  LCD1602_4PINS
    lcdWrite(OP_COMD,CMD_8PINS|CMD_2LINE);
    lcdWrite(OP_COMD,CMD_NOCUR);      // 开显示不显示光标
    // lcdWrite(OP_COMD,CMD_CURRT);    // 写一个指针加1，这个好像是自动的
    lcdWrite(OP_COMD,CMD_CLS);
    lcdWrite(OP_COMD,DDRAMADD(0x00)); // 设置数据指针起点
#else
    lcdWrite(OP_COMD,0x32);           // 将8位总线转为4位总线
    lcdWrite(OP_COMD,CMD_2LINE);
    lcdWrite(OP_COMD,CMD_NOCUR);      // 开显示不显示光标
    // lcdWrite(OP_COMD,CMD_CURRT);    // 写一个指针加1，这个好像是自动的
    lcdWrite(OP_COMD,CMD_CLS);
    lcdWrite(OP_COMD,DDRAMADD(0x00)); // 设置数据指针起点
#endif
}

/*!
    write command to lcd
*/
void lcdWriteComd(unsigned char cmd)
{
    lcdWrite(OP_COMD,cmd);
}

/*!
    write data to lcd
*/
void lcdWriteData(unsigned char dat)
{
    lcdWrite(OP_DATA,dat);
}

/*!
    wait when lcd is busy
*/
void lcdWaitBusy(void)
{
    unsigned char dat;
#ifndef  LCD1602_4PINS

    do {

        LCD1602_E = 0;
        delay(3);

        LCD1602_RS = OP_COMD;
        LCD1602_RW = 1;    //选择读出
        LCD1602_DATAPINS=0xff;
        LCD1602_E = 1;
        dat = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
    }
    while(dat&0x80);
#else
    unsigned char buf;
    LCD1602_E = 0;
    delay(3);

    LCD1602_RS = OP_COMD;
    LCD1602_RW = 1;    //选择读出

    do {
        LCD1602_DATAPINS |= 0xf0; //高四位置一
        delay(1);
        LCD1602_E = 1;
        dat = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
        dat &= 0xf0;  //取高位

        LCD1602_DATAPINS |= 0xf0; //高四位置一
        delay(1);
        LCD1602_E = 1;
        buf = LCD1602_DATAPINS;
        delay(1);
        LCD1602_E = 0;
        delay(1);
        dat |= (buf>>4); //得到低位
    }
    while(dat&0x80);
#endif

}

/*!
    lcd clear screen
*/
void lcdCls(void)
{
    lcdWriteComd(CMD_CLS);
}

/*!
    put character to lcd
*/
void lcdPutchar(unsigned char c)
{
    if(isprint(c))
    {
        lcdWriteData(c);
    }
    else if(c=='\n')
    {
        lcdLineFeed();
    }
}

/*!
    put string to lcd
*/
void lcdPuts(unsigned char *str)
{
    for(;*str;++str)
    {
        lcdPutchar(*str);
    }
}

/*!
    line feed on lcd
*/
void lcdLineFeed(void)
{
#ifndef LCD1602_10ROWS
    lcdWriteComd(CMD_CURES);
    lcdWriteComd(DDRAMADD(SCDLINE));
#else
    lcdCls();
#endif
}

/*!
    \internal
*/
void delay(unsigned int t)
{
    do {
        volatile unsigned char i = 10;
        while(--i);
    }
    while(--t);
}

/*!
    \internal
*/
void lcdWrite(bit op, unsigned char dat)   //写入命令/数据
{
    lcdWaitBusy();

    LCD1602_E = 0;     //使能清零
    LCD1602_RS = op;   //选择发送命令|数据
    delay(1);
    LCD1602_RW = 0;    //选择写入

#ifndef LCD1602_4PINS
    LCD1602_DATAPINS = dat; //放入命令/数据
    delay(1);            //等待数据稳定

    LCD1602_E = 1;          //写入时序
    delay(5);            //保持时间
    LCD1602_E = 0;

#else
    LCD1602_DATAPINS &= 0x0f;
    LCD1602_DATAPINS = HI4BITS(dat);    //发送高四位
    delay(1);

    LCD1602_E = 1;            //写入时序
    delay(5);      //保持时间
    LCD1602_E = 0;

    LCD1602_DATAPINS = LO4BITS(dat); //发送低四位
    delay(1);

    LCD1602_E = 1;            //写入时序
    delay(5);      //保持时间
    LCD1602_E = 0;

#endif
}

