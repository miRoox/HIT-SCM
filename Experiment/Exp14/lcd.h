#ifndef __LCD_H_
#define __LCD_H_
#include <REGX51.H>

#define LCD1602_DATAPINS P0
sbit LCD1602_E=P2^2;
sbit LCD1602_RW=P2^1;
sbit LCD1602_RS=P2^0;

#undef LCD1602_4PINS // 不设置为4位数据接口

enum
{
/* - 基本操作 (读/写~) - */
    OP_COMD = 0, // 命令
    OP_DATA = 1, // 数据
/* - 命令集 - */
    CMD_CLS = 0x01,     // 清屏 - DB0
    CMD_CURES = 0x02,   // 光标归位 - DB1
    // 显示开关控制指令 - DB3
    CMD_SCOFF = 0x08,   // 关显示
    CMD_NOCUR = 0x0c,   // 无光标
    CMD_CURFLS = 0x0e,  // 有光标闪烁
    CMD_CURSTC = 0x0f,  // 有光标不闪烁
    // 进入模式设置指令 - DB2
    CMD_CURLF = 0x04,   // 写入新数据后光标左移模式
    CMD_CURRT = 0x06,   // 写入新数据后光标右移模式
    CMD_SCMOV = 0x05,   // 写入新数据后显示屏整体移动1个字(应与上面这两个(通过位或)结合使用)
    // 功能设定指令(通过位与组合使用) - DB5
    CMD_8PINS = 0x30,   // 数据总线为8位;默认:数据总线为4位
    CMD_2LINE = 0x28,   // 显示2行;默认:显示1行
    CMD_10ROW = 0x24,   // 每字符5×10点阵;默认:每字符5×8点阵
    // 设定显示屏或光标移动方向指令 -DB4
    CMD_SCRMV = 0x1c,   // 整屏右移
    CMD_SCLMV = 0x18,   // 整屏左移
    CMD_CRRMV = 0x14,   // 光标右移1格
    CMD_CRLMV = 0x10,   // 光标左移1格
    // 设定CGRAM地址指令 - DB6
#   define CGRAMADD(x) (((x)&0x3f)|0x40)
    // 设置DDRAM地址指令 - DB7
#   define DDRAMADD(x) (((x)&0x7f)|0x80)
/* - 杂 - */
    SCDLINE = 0x40,     // 第二行开始的地址
};

extern void lcdWriteComd(unsigned char cmd);
extern void lcdWriteData(unsigned char dat);
extern void lcdWaitBusy(void);
extern void lcdInit(void);
extern void lcdCls(void);
extern void lcdPutchar(unsigned char c);
extern void lcdPuts(unsigned char *str);
extern void lcdLineFeed(void);


#endif