/* 
*  \InterruptRoutines_UART.c file 
*  \author: Elena Stocco and Riccardo Mustoni
*  \date: 31/03/2021 - 09/04/2021
*  \assignment 02
*  \group 4
*
*/

#include "InterruptRoutines_UART_ISR.h"

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
