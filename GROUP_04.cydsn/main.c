/*
* timer: ogni volta che sono in uno stato intermedio (case 1,2,3), per prima cosa
inizializzo un counter che viene incrementato di uno ad ogni esecuzione di InterruptRoutines_TIMER.
all'inizio di ogni while(1), se sono in status 1,2,3 controllo il valore del counter.
se sono passati -timeout- secondi, torno a status 0, altrimenti passo allo stato successivo
*
*/

#include "project.h"
#include "InterruptRoutines_UART.h"
#include "InterruptRoutines_TIMER.h"
#include "RGBLedDriver.h"

#define HEADER_TIMEOUT 0xA1
#define HEAD 0xA0
#define TAIL 0xC0
#define LOW_TIM_TH 0x01
#define HIGH_TIM_TH 0x14

volatile int flag = 0;
volatile int status = 0;
volatile int counter = 0;
volatile uint8_t timeout = 5;
volatile uint8_t set_timeout = 5;

char message_1[20]="Hello there!\n";
char message_2[20]="Fine!\n";

int main(void)
{
    RGBLed_Start();
    
    const Color BLACK={0,0,0}; // Initialising the LED to BLACK color
    
    RGBLed_WriteColor(BLACK);
    
    UART_Start();
    isr_UART_StartEx(Custom_UART_RX_ISR);
    
    isr_Timer_StartEx(Custom_Timer_ISR);
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint8_t red=0,green=0,blue=0,head,tail;
    
    while(1){
        if(flag){
            switch(status){
                case 0:
                    Timer_Stop();
                    head=UART_ReadRxData();
                    if(head==HEAD)
                        status++;
                    else if(head==HEADER_TIMEOUT)
                        status=5;
                    flag=0;
                    break;
                case 1:
                    counter=0;
                    Timer_Start();
                    red=UART_ReadRxData();
                    Timer_Stop();
                    status++;
                    flag=0;
                    break;
                case 2:
                    counter=0;
                    Timer_Start();
                    green=UART_ReadRxData();
                    Timer_Stop();
                    status++;
                    flag=0;
                    break;
                case 3:
                    counter=0;
                    Timer_Start();
                    blue=UART_ReadRxData();
                    Timer_Stop();
                    status++;
                    flag=0;
                    break;
                case 4:
                    tail=UART_ReadRxData();
                    if(tail==TAIL){
                        Color color={red,green,blue};
                        RGBLed_WriteColor(color);
                        if(timeout != set_timeout)
                            timeout = set_timeout;
                        status=0;
                        //UART_PutString(message_2);
                    }
                    flag=0;
                    break;
                case 5:
                    set_timeout = UART_ReadRxData();
                    status--;
                    flag=0;
                    //UART_PutString(message_1);
                    break;
            }
        }
    }
}

/* [] END OF FILE */
