/*
*
*
*/

#include "InterruptRoutines.h"

#include "UART.h"

static char def_message[21]="RGB Led Program $$$\r\n";

CY_ISR(Custom_UART_RX_ISR){
    
    // Connecting the Firmware to the GUI
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
        uint8_t received=UART_ReadRxData();
        if(received=='v')
            UART_PutString(def_message);
    }
}

/* [] END OF FILE */
