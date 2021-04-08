/* 
 *  \InterruptRoutines_TIMER.c file
 *  \author: Elena Stocco and Riccardo Mustoni
 *  \date: 31/03/2021 - 09/04/2021
 *  \assignment 02
 *  \group 4
 * 
*/

//#include "InterruptRoutines_TIMER.h"
#include "InterruptRoutines_UART_ISR.h"

extern volatile int timeout;
extern volatile int counter;
extern volatile int status;
extern volatile uint8_t counter_flag;

char messageX[21]="Counters:";
char messageY[21]="reached time out";

CY_ISR(Custom_Timer_ISR)
{   
    Timer_ReadStatusRegister();
    //the variable counter increases by one every time an isr is triggered
    counter++;
    
    
    if (counter == timeout){
        //if 5 sec has passed
        //UART_PutString(messageX);
        counter_flag = 1;
        //UART_PutString(messageY);
        //printf("%d", b);
    }
}

/* [] END OF FILE */
