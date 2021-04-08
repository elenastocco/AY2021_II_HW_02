/* 
*  \main.c file 
*  \author: Elena Stocco and Riccardo Mustoni
*  \date: 31/03/2021 - 09/04/2021
*  \assignment 02
*  \group 4
*
*/

#include "project.h"
#include "InterruptRoutines_UART_ISR.h"

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
volatile uint8_t counter_flag = 0;
volatile uint8_t received;

char message_1[20]="start again";
char message_2[20]="Fine!\n";

void Counter_Waiting(void){
    Timer_Start();
    counter_flag=0;
    while(!flag){
        if(counter_flag){
            //UART_PutString("passati 5 sec\r\n");
            Timer_Stop();
            status=0;
            //counter_flag=0;
            //flag=1;
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
    char timeout_char[21];
    
    //sprintf(timeout_char, "%d", timeout);
    //UART_PutString("timeout:");
    //UART_PutString(timeout_char);
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint8_t red=0,green=0,blue=0,head,tail;
    
    while(1){
            switch(status){
                case 0:
                    UART_PutString("inizio stato 0");
                    Timer_Stop();
                    while(!flag); //aspetto che arrivi il dato
                    head=received; //metti received
                    
                    //UART_PutString(head);
                    if(head==HEAD){
                        UART_PutString("head received");
                        status++;
                        //UART_PutString("timeout vale:");
                        //UART_PutString(timeout_char);
                        //Timer_Start();
                    }
                    else if(head==HEADER_TIMEOUT)
                        status=5;
                    flag=0;
                    break;
                case 1:
                    counter=0;
                    //UART_PutString("Inizio conteggio stato 1\r\n");
                    if(!flag){
                        Counter_Waiting();
                    }
                    //flag=0;
                    if(counter_flag){
                        break;
                    }
                    
                    else{
                        UART_PutString("arrivato rosso");
                        red=received;
                        Timer_Stop();
                        status++;
                        flag=0;
                        break;
                    }
                    break;
                    //UART_PutString(timeout_char);
                    //else{
                       // Timer_Stop();    
                       // counter=0;
                    //}
                case 2:
                    counter=0;
                    //Timer_Start();
                    //UART_PutString("Inizio conteggio stato 2\r\n");
                    if(!flag){
                        Counter_Waiting();
                    }
                    if(counter_flag){
                        break;
                    }
                    else{
                        UART_PutString("arrivato verde");
                        green=received;
                        Timer_Stop();
                        status++;
                        flag=0;
                        break;
                    }
                    //else{
                        //Timer_Stop();
                        //counter=0;
                    //}
                case 3:
                    counter=0;
                    //Timer_Start();
                    //UART_PutString("Inizio conteggio stato 3");
                    if(!flag){
                        Counter_Waiting();
                    }
                    if(counter_flag){
                        break;
                    }
                    else{
                        UART_PutString("arrivato BLU");
                        blue=received;
                        Timer_Stop();
                        status++;
                        flag=0;
                        break;
                    }
                    //else{
                       // Timer_Stop();
                        //counter=0;
                    //}
                case 4:
                    while(!flag);
                    tail=received;
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
                    while(!flag);
                    set_timeout = received;
                    status--;
                    flag=0;
                    //UART_PutString(message_1);
                    break;
            }
    }
}

/* [] END OF FILE */
