/*
*
*
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"
#include "stdio.h"

#define HEAD 0xA0
#define TAIL 0xC0

// static char message[50]={'\0'};

int main(void)
{
    RGBLed_Start();
    
    const Color BLACK={0,0,0}; // Initialising the LED to BLACK color
    //const Color BLUE={0,0,255};
    //const Color GREEN={0,255,0};
    //const Color RED={255,0,0};
    //const Color GRAY={128,128,128};
    //const Color WHITE={255,255,255};
    
    RGBLed_WriteColor(BLACK);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    isr_UART_StartEx(Custom_UART_RX_ISR);
    
    int status=0;
    uint8_t red=0,green=0,blue=0;
    
    while(1){
        
        switch(status){
            case 0:
                if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
                    uint8_t head=UART_ReadRxData();
                    // sprintf(message,"Status: %d",status);
                    // UART_PutString(message);
                    if(head==HEAD){
                        status++;
                        break;
                    }
                    //RGBLed_WriteColor(RED);
                    //CyDelay(1000);
                    //status++;
                    break;
                }
            case 1:
                if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
                    red=UART_ReadRxData();
                    // sprintf(message," %d",status);
                    // UART_PutString(message);
                    status++;
                    break;
                }
                //RGBLed_WriteColor(GREEN);
                //CyDelay(1000);
                //status++;
                break;
            case 2:
                if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
                    green=UART_ReadRxData();
                    // sprintf(message," %d",status);
                    // UART_PutString(message);
                    status++;
                    break;
                }
                //RGBLed_WriteColor(BLUE);
                //CyDelay(1000);
                //status++;
                break;
            case 3:
                if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
                    blue=UART_ReadRxData();
                    // sprintf(message," %d",status);
                    // UART_PutString(message);
                    status++;
                    break;
                }
                //RGBLed_WriteColor(GRAY);
                //CyDelay(1000);
                //status++;
                break;
            case 4:
                if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
                    uint8_t tail=UART_ReadRxData();
                    // sprintf(message," %d",status);
                    // UART_PutString(message);
                    if(tail==TAIL){
                        Color color={red,green,blue};
                        RGBLed_WriteColor(color);
                        status=0;
                        break;
                    }
                    //RGBLed_WriteColor(WHITE);
                    //CyDelay(1000);
                    //status=0;
                    break;
                }
        
        }

    }
}

/* [] END OF FILE */
