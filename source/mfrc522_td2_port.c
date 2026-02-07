/*
 * mfrc522_td2_port.c
 *
 *  Created on: 21 ene. 2026
 *      Author: Ignacio
 */

#include "mfrc522_td2_port.h"

// SPI GLOBAL VARIABLES
uint32_t srcFreq;
spi_master_config_t userConfig;

void mfrc522_spi_config(void)
{
    SPI_MasterGetDefaultConfig(&userConfig);
    userConfig.baudRate_Bps = 1000000;
    userConfig.sselNumber = kSPI_SselDeAssertAll;
    userConfig.clockPolarity = kSPI_ClockPolarityActiveLow;
    userConfig.clockPhase = kSPI_ClockPhaseSecondEdge;
    userConfig.delayConfig.frameDelay = 0x0U;

    srcFreq = CLOCK_GetFreq(kCLOCK_MainClk);
}

void writeMFRC522(unsigned char addr, unsigned char val)
{
    /*Escritura del spi*/
    // 0XXXXXX0 formato de direccionn
    // static Chip_SSP_DATA_SETUP_T xf_setup;

    static uint8_t tx_buffer[2];
    static uint8_t rx_buffer[2];

    tx_buffer[0] = (addr << 1) & 0x7E;
    tx_buffer[1] = val;

    spi_transfer_t xfer = {0};
    xfer.txData = tx_buffer;
    xfer.rxData = rx_buffer;
    xfer.dataSize = 2;
    xfer.configFlags = kSPI_EndOfTransfer | kSPI_EndOfFrame;

    SPI_MasterInit(SPI0, &userConfig, srcFreq);
    MFRC522_CS(0);                           // Habilito el CS
    SPI_MasterTransferBlocking(SPI0, &xfer); // Realizo la comunicacion
    SPI_Deinit(SPI0);
    MFRC522_CS(1); // Deshabilito el CS

    /*static uint8_t Tx_Buffer[BUFFER_SIZE_R];
    static uint8_t Rx_Buffer[BUFFER_SIZE_R];

     Asigno el tamaño de los buffers
    xf_setup.length = BUFFER_SIZE_R;
     Asigno el buffer de transmisión
    xf_setup.tx_data = Tx_Buffer;
     Asigno el buffer de recepción
    xf_setup.rx_data = Rx_Buffer;

    xf_setup.rx_cnt = 0;
    xf_setup.tx_cnt = 0;

    Tx_Buffer[0] = ((addr<<1)&0x7E);
    Tx_Buffer[1] = val;

    Chip_GPIO_SetPinState(LPC_GPIO, SDA_PORT, SDA_PIN, 0);
    Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
    Chip_GPIO_SetPinState(LPC_GPIO, SDA_PORT, SDA_PIN,1);*/
}

unsigned char readMFRC522(unsigned char addr)
{
    /*Lectura del spi*/
    static uint8_t tx_buffer[2];
    static uint8_t rx_buffer[2];
    tx_buffer[0] = ((addr << 1) & 0x7E) | 0x80;
    // Byte 1 = 0 cierra la trama de lectura
    tx_buffer[1] = 0;

    spi_transfer_t xfer = {0};
    xfer.txData = tx_buffer;
    xfer.rxData = rx_buffer;
    xfer.dataSize = 2;
    xfer.configFlags = kSPI_EndOfTransfer | kSPI_EndOfFrame;

    SPI_MasterInit(SPI0, &userConfig, srcFreq);
    MFRC522_CS(0);                           // Habilito el CS
    SPI_MasterTransferBlocking(SPI0, &xfer); // Realizo la comunicacion
    MFRC522_CS(1);                           // Deshabilito el CS
    SPI_Deinit(SPI0);

    return rx_buffer[1];
    /*unsigned char val;
    static Chip_SSP_DATA_SETUP_T xf_setup;
    static uint8_t Tx_Buffer[BUFFER_SIZE_R];
    static uint8_t Rx_Buffer[BUFFER_SIZE_R];

     Asigno el tamaño de los buffers
    xf_setup.length = BUFFER_SIZE_R;
     Asigno el buffer de transmisión
    xf_setup.tx_data = Tx_Buffer;
     Asigno el buffer de recepción
    xf_setup.rx_data = Rx_Buffer;

    xf_setup.rx_cnt = 0;
    xf_setup.tx_cnt = 0;

    Tx_Buffer[0] = (((addr<<1)&0x7E) | 0x80);
    Tx_Buffer[1] = 0x00;

    Chip_GPIO_SetPinState(LPC_GPIO, SDA_PORT, SDA_PIN,0);
    Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
    val = Rx_Buffer[1];

    Chip_GPIO_SetPinState(LPC_GPIO, SDA_PORT, SDA_PIN,1);

    return val;*/
}
