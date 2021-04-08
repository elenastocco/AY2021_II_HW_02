/* 
 *  \InterruptRoutines.c file
 *  \author: Elena Stocco and Riccardo Mustoni
 *  \date: 31/03/2021 - 09/04/2021
 *  \assignment 02
 *  \group 4
 * 
*/

//#include "InterruptRoutines_TIMER.h"
#include "InterruptRoutines.h"
#include "stdio.h"

extern volatile int timeout;
extern volatile int counter;
extern volatile int status;
extern volatile int counter_flag;

char messageX[21]={"\0"};
char messageY[21]="reached time out\n";

CY_ISR(Custom_Timer_ISR)
{   
    Timer_ReadStatusRegister();
    //the variable counter increases by one every time an isr is triggered
    counter++;
    
    sprintf(messageX,"% d",counter);
    UART_PutString(messageX);
    
    if (counter == timeout){
        //if 5 sec has passed
        counter_flag = 1;
        UART_PutString(messageY);
    }
}

#include "UART.h"

static char def_message[21]="RGB Led Program $$$\r\n";
extern volatile int flag;
extern volatile int counter;
extern volatile uint8_t received;

CY_ISR(Custom_UART_RX_ISR){
    // Connecting the Firmware to the GUI
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
        received=UART_ReadRxData();
        if(received=='v')
            UART_PutString(def_message);
        else
            flag=1;
    }
}
/* [] END OF FILE */
