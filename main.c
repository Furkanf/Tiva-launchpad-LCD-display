#include <stdint.h>
//#include <stdbool.h>
//#include "inc/hw_ints.h"
//#include "inc/hw_types.h"
//#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
//#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"
#include <stdio.h>
#include <stdlib.h>



#define LCDPORT                  GPIO_PORTB_BASE
#define LCDPORTENABLE    SYSCTL_PERIPH_GPIOB
#define RS                               GPIO_PIN_0
#define E                                GPIO_PIN_1
#define PortD4                               GPIO_PIN_4
#define PortD5                               GPIO_PIN_5
#define PortD6                               GPIO_PIN_6
#define PortD7                               GPIO_PIN_7



int main(void) {

        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);

        Lcd_init();
        Lcd_Goto(1,1);
        Lcd_Puts("Hello World");
        Lcd_Goto(2,1);
        Lcd_Puts("Merhaba Dunya");

        while (1) {

        }
}

void Lcd_init() {

        SysCtlPeripheralEnable(LCDPORTENABLE);
        GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, RS,  0x00 );

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x30 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7,  0x20 );
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        Lcd_Komut(0x28);
        Lcd_Komut(0xC0);
        Lcd_Komut(0x06);
        Lcd_Komut(0x80);
        Lcd_Komut(0x28);
        Lcd_Komut(0x0f);
        Lcd_Temizle();

}

void Lcd_Komut(unsigned char c) {

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (c & 0xf0) );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(50000);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (c & 0x0f) << 4 );
        GPIOPinWrite(LCDPORT, RS, 0x00);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

}

void Lcd_Putch(unsigned char d) {

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (d & 0xf0) );
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

        GPIOPinWrite(LCDPORT, PortD4 | PortD5 | PortD6 | PortD7, (d & 0x0f) << 4 );
        GPIOPinWrite(LCDPORT, RS, 0x01);
        GPIOPinWrite(LCDPORT, E, 0x02);
        SysCtlDelay(10);
        GPIOPinWrite(LCDPORT, E, 0x00);

        SysCtlDelay(50000);

}
void Lcd_Goto(char x, char y){

        if(x==1)
                Lcd_Komut(0x80+((y-1)%16));
        else
                Lcd_Komut(0xC0+((y-1)%16));
}

void Lcd_Temizle(void){
        Lcd_Komut(0x01);
        SysCtlDelay(10);
}

void Lcd_Puts( char* s){

        while(*s)
                Lcd_Putch(*s++);
}
