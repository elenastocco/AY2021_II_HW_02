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
#include "stdio.h"

#define HEAD 0xA0
#define TAIL 0xC0

volatile uint8_t flag=0;
volatile int status;
volatile int counter = 0;
volatile uint8_t counter_flag =0;
volatile uint8_t timeout_flag =0;
volatile int timeout = 5;
volatile int set_timeout = 5;

int main(void)
{
    RGBLed_Start();
    
    const Color BLACK={0,0,0}; // Initialising the LED to BLACK color
    
    RGBLed_WriteColor(BLACK);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    isr_UART_StartEx(Custom_UART_RX_ISR);
    
    //Timer_Start();
    isr_Timer_StartEx(Custom_Timer_ISR);
    
    uint8_t red=0,green=0,blue=0,head,tail;
    status=0;
    
  while(1){
        ///*
        //if I'm in intermediate states, I check if -timeout- sec has passed
        if (status == 1 || status == 2 || status == 3){
            Timer_Start();
            if (counter_flag == 1){
                //5 sec has passed since the beginning of the status
                //so I have to go back to state 0
                Timer_Stop();
                status = 0;
            }
            else {
                //less than 5 sec has passed, so I can switch to the next state
                Timer_Stop();
                //status++;
            }
            counter_flag = 0;
        }
        if (timeout_flag){
            //if the packet header for timeout configuration has been sent
            //read the desired timeout
            set_timeout = UART_ReadRxData();
            //read tail packet
            tail = UART_ReadRxData();
            if (tail == TAIL && timeout>=1 && timeout>=20){
                timeout=set_timeout;
            }
            timeout_flag = 0;
        }
        //*/
        if(flag){
            switch(status){
                case 0:
                    head=UART_ReadRxData();
                    if(head==HEAD){
                        status++;
                        //flag=0;
                        //break;
                    }
                    flag=0;
                    break;
                case 1:
                    counter=0;
                    red=UART_ReadRxData();
                    status++;
                    flag=0;
                    break;
                case 2:
                    counter=0;
                    green=UART_ReadRxData();
                    status++;
                    flag=0;
                    break;
                case 3:
                    counter=0;
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
                        //flag=0;
                        //break;
                    }
                    flag=0;
                    break;
            }
        }
    }
}

/* [] END OF FILE */
