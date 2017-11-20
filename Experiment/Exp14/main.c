#include <REGX51.H>
#include "lcd.h"

#define forever for(;;)

typedef unsigned char uchar;

const uchar code msg[] = "Hello\n"
                         "Welcome to China";

int main(void)
{
    lcdInit();
    lcdPuts(msg);

    forever;

    return 0;
}
