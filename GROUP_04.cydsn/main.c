/* 
*  \main.c file 
*  \authors: Elena Stocco and Riccardo Mustoni
*  \date: 31/03/2021 - 09/04/2021
*  \assignment 02
*  \group 4
*
*/

#include "project.h"
#include "InterruptRoutines.h"
#include "RGBLedDriver.h"

#define HEADER_TIMEOUT 0xA1
#define HEAD 0xA0
#define TAIL 0xC0
#define LOW_TIM_TH 0x01
#define HIGH_TIM_TH 0x14

volatile int flag = 0;
volatile int status = 0;
volatile int counter = 0;
volatile int timeout = 5;
volatile uint8_t set_timeout = 5;
volatile int counter_flag = 0;
volatile uint8_t received;

//function to count the time passed between intermediate state
void Counter_Waiting(void){
    counter=0;
    counter_flag=0;
    Timer_Start();
    while(!flag){
        if(counter_flag){
            status=0;
            break;   
        }
    }
    Timer_Stop();
}

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
        switch(status){
            case 0:
                while(!flag);
                head=received;
                if(head==HEAD)
                    status++;
                else if(head==HEADER_TIMEOUT)
                    status=5;
                flag=0;
                break;
            case 1:
                // Start the counter
                Counter_Waiting();
                if(flag){
                    red=received;
                    status++;
                    flag=0;
                }
                break;
            case 2:
                Counter_Waiting();
                if(flag){
                    green=received;
                    status++;
                    flag=0;
                }
                break;
            case 3:
                Counter_Waiting();
                if(flag){
                    blue=received;
                    status++;
                    flag=0;
                }
                break;
            case 4:
                Counter_Waiting();
                if(flag){
                    tail=received;
                    if(tail==TAIL){
                        Color color={red,green,blue};
                        RGBLed_WriteColor(color);
                        if(timeout != set_timeout)
                            timeout = set_timeout;
                        status=0;
                    }
                    flag=0;
                }
                break;
            case 5:
                while(!flag);
                if(received >= LOW_TIM_TH && received <= HIGH_TIM_TH){
                    set_timeout = received;
                    status--;
                }
                else
                    status=0;
                flag=0;
                break;
        }
    }
}

/* [] END OF FILE */
