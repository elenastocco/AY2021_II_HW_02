/*
*
*
*/

#include "InterruptRoutines_UART.h"

#include "UART.h"

static char def_message[21]="RGB Led Program $$$\r\n";
extern volatile int flag;
extern volatile int counter;

CY_ISR(Custom_UART_RX_ISR){
    // Connecting the Firmware to the GUI
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
        uint8_t received=UART_ReadRxData();
        if(received=='v')
            UART_PutString(def_message);
        else
            flag=1;
    }
}

/* [] END OF FILE */
