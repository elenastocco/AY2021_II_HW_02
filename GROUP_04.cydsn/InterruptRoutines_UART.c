/*
*
*
*/

#include "InterruptRoutines_UART.h"

#include "UART.h"
#define HEADER_TIMEOUT 0xA1

static char def_message[21]="RGB Led Program $$$\r\n";
extern volatile uint8_t flag;
extern volatile int counter;
extern volatile uint8_t counter_flag;
extern volatile uint8_t timeout_flag;

CY_ISR(Custom_UART_RX_ISR){
    
    
    
    // Connecting the Firmware to the GUI
    if(UART_ReadRxStatus()==UART_RX_STS_FIFO_NOTEMPTY){
        uint8_t received=UART_ReadRxData();
        if(received=='v')
            UART_PutString(def_message);
        else if (received == HEADER_TIMEOUT){
            timeout_flag=1;
        }
        else {
            flag=1;
            
        }
    }
}

/* [] END OF FILE */
