/* ========================================
 *
 * 
*/
#include "InterruptRoutines_TIMER.h"
extern volatile int timeout;
extern volatile int counter;
extern volatile uint8_t counter_flag;
CY_ISR(Custom_Timer_ISR)
{
    //the variable counter increases by one every time an isr is triggered
    counter++;
    if (counter == timeout){
        //if 5 sec has passed
        counter_flag=1;
    }
}

/* [] END OF FILE */
