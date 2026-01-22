#include "board.h"
#include "fsl_usart.h"
#include "fsl_swm.h"
#include "pin_mux.h"
#include <stdbool.h>
#include <string.h>

int main(void)
{
    //uint8_t ch;
    //usart_config_t config;
    uint8_t txdBuffer[] = "1";
    uint8_t rxdBuffer[3];


    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_TXD, kSWM_PortPin_P0_18);
    SWM_SetMovablePinSelect(SWM0, kSWM_USART0_RXD, kSWM_PortPin_P0_19);
    CLOCK_DisableClock(kCLOCK_Swm);

    CLOCK_Select(kUART0_Clk_From_MainClk);
    /* Default config by using USART_GetDefaultConfig():
     * config->baudRate_Bps = 9600U;
     * config->parityMode = kUSART_ParityDisabled;
     * config->stopBitCount = kUSART_OneStopBit;
     * config->bitCountPerChar = kUSART_8BitsPerChar;
     * config->loopback = false;
     * config->enableRx = false;
     * config->enableTx = false;
     * config->syncMode = kUSART_SyncModeDisabled;
     */

    usart_config_t user_config;
    USART_GetDefaultConfig(&user_config);
    user_config.baudRate_Bps = 9600;
    user_config.enableTx = true;
    user_config.enableRx = true;

    USART_Init(USART0, &user_config, CLOCK_GetFreq(kCLOCK_MainClk));

   // USART_WriteBlocking(USART1, txdBuffer, sizeof(txdBuffer) - 1);

    while (1)
    {
        USART_WriteBlocking(USART0, &txdBuffer, 1);		// envia datos al arduino
        USART_ReadBlocking(USART0, &rxdBuffer, 2);		// recibe respuesta del arduino
        memset(rxdBuffer,0,sizeof(rxdBuffer)); 		//reset del buffer
    }
}
