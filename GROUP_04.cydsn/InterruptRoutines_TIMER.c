/* ========================================
 *
 * 
*/

#include "InterruptRoutines_TIMER.h"

extern volatile int timeout;
extern volatile int counter;
extern volatile int status;

char messageX[21]="Counters:";

CY_ISR(Custom_Timer_ISR)
{   
    Timer_ReadStatusRegister();
    
    UART_PutString(messageX);
    //the variable counter increases by one every time an isr is triggered
    counter++;
    if (counter == timeout){
        //if 5 sec has passed
        //UART_PutString(messageX);
        status = 0;
    }
}

/* [] END OF FILE */
