/*
*
*
*/

#include "project.h"
#include "InterruptRoutines_UART.h"
#include "RGBLedDriver.h"
#include "stdio.h"

#define HEAD 0xA0
#define TAIL 0xC0

volatile uint8_t flag=0;
volatile int status;

int main(void)
{
    RGBLed_Start();
    
    const Color BLACK={0,0,0}; // Initialising the LED to BLACK color
    
    RGBLed_WriteColor(BLACK);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    isr_UART_StartEx(Custom_UART_RX_ISR);
    
    uint8_t red=0,green=0,blue=0,head,tail;
    status=0;
    
    while(1){
        
        if(flag){
            switch(status){
                case 0:
                    head=UART_ReadRxData();
                    if(head==HEAD){
                        status++;
                        flag=0;
                        break;
                    }
                    flag=0;
                    break;
                case 1:
                    red=UART_ReadRxData();
                    status++;
                    flag=0;
                    break;
                case 2:
                    green=UART_ReadRxData();
                    status++;
                    flag=0;
                    break;
                case 3:
                    blue=UART_ReadRxData();
                    status++;
                    flag=0;
                    break;
                case 4:
                    tail=UART_ReadRxData();
                    if(tail==TAIL){
                        Color color={red,green,blue};
                        RGBLed_WriteColor(color);
                        status=0;
                        flag=0;
                        break;
                    }
                    flag=0;
                    break;
            }
        }
    }
}

/* [] END OF FILE */
