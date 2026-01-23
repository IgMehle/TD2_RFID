/*
 * mfrc522_td2.c
 *
 *  Created on: 21 ene. 2026
 *      Author: Ignacio
 */

#include "mfrc522_td2.h"

volatile uint8_t mfrc522_registros[64] = {0};
#define PEEK(reg) mfrc522_registros[reg] = readMFRC522(reg);

void mfrc522_init(void)
{
    /*Incializacion del rfid*/
    MFRC522_RST(1); // Chip_GPIO_SetPinState(LPC_GPIO, RST_PORT, RST_PIN, 1);
    // vTaskDelay(200/portTICK_RATE_MS);      //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
    reset();

    // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
    writeMFRC522(TModeReg, 0x8D); // Tauto=1; f(Timer) = 6.78MHz/TPreScaler
    //   delay_ms(200);//vTaskDelay(200/portTICK_RATE_MS);     //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
    writeMFRC522(TPrescalerReg, 0x3E); // TModeReg[3..0] + TPrescalerReg
    //   delay_ms(200);//vTaskDelay(200/portTICK_RATE_MS);     //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
    writeMFRC522(TReloadRegL, 30);
    //   delay_ms(200);// vTaskDelay(200/portTICK_RATE_MS);    //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
    writeMFRC522(TReloadRegH, 0);
    //   delay_ms(200);// vTaskDelay(200/portTICK_RATE_MS);     //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK

    writeMFRC522(TxAutoReg, 0x40); // 100%ASK
    //   delay_ms(200);// vTaskDelay(200/portTICK_RATE_MS);     //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
    writeMFRC522(ModeReg, 0x3D); // CRC valor inicial de 0x6363
                                 //   delay_ms(200);//vTaskDelay(200/portTICK_RATE_MS);     //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK

    // ClearBitMask(Status2Reg, 0x08);   //MFCrypto1On=0
    // writeMFRC522(RxSelReg, 0x86);      //RxWait = RxSelReg[5..0]
    // writeMFRC522(RFCfgReg, 0x7F);      //RxGain = 48dB

    antennaOn();   // Abre  la antena
    delay_ms(100); // vTaskDelay(1000/portTICK_RATE_MS);     //DELAY DE FREERTOS 1000ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
}

void reset(void)
{
    /*Resetea el rfid*/
    writeMFRC522(CommandReg, PCD_RESETPHASE);
    delay_ms(200); // vTaskDelay(200/portTICK_RATE_MS);              //DELAY DE FREERTOS 200ms - REEMPLAZAR POR IMPLEMENTACIÓN CON SYSTICK
}

void antennaOn(void)
{
    /*Abre la antena del rfid*/
    unsigned char temp;

    temp = readMFRC522(TxControlReg);
    if (!(temp & 0x03))
    {
        setBitMask(TxControlReg, 0x03);
    }
}

bool isCard(void)
{
    /*Se fija si hay un tag presente*/
    unsigned char status;
    unsigned char str[MAX_LEN];

    status = MFRC522Request(PICC_REQIDL, str);
    if (status == MI_OK)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char MFRC522Request(unsigned char reqMode, unsigned char *TagType)
{
    /*Recibe los valores para la comunicacion tag-modulo*/
    unsigned char status;
    unsigned int backBits; //   Recibiï¿½ bits de datos

    writeMFRC522(BitFramingReg, 0x07); // TxLastBists = BitFramingReg[2..0]   ???

    TagType[0] = reqMode;
    status = MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

    if ((status != MI_OK) || (backBits != 0x10))
    {
        status = MI_ERR;
    }

    return status;
}

unsigned char MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
    /*Realiza la comunicacion con el tag*/
    unsigned char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitIRq = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;

    switch (command)
    {
    case PCD_AUTHENT: // Tarjetas de certificaciï¿½n cerca
    {
        irqEn = 0x12;
        waitIRq = 0x10;
        break;
    }
    case PCD_TRANSCEIVE: // La transmisiï¿½n de datos FIFO
    {
        irqEn = 0x77;
        waitIRq = 0x30;
        break;
    }
    default:
        break;
    }

    writeMFRC522(CommIEnReg, irqEn | 0x80); // De solicitud de interrupciï¿½n
    clearBitMask(CommIrqReg, 0x80);         // Borrar todos los bits de peticiï¿½n de interrupciï¿½n
    setBitMask(FIFOLevelReg, 0x80);         // FlushBuffer=1, FIFO de inicializaciï¿½n

    writeMFRC522(CommandReg, PCD_IDLE); // NO action;Y cancelar el comando

    // Escribir datos en el FIFO
    for (i = 0; i < sendLen; i++)
    {
        writeMFRC522(FIFODataReg, sendData[i]);
    }

    //???? ejecutar el comando
    writeMFRC522(CommandReg, command);
    if (command == PCD_TRANSCEIVE)
    {
        setBitMask(BitFramingReg, 0x80); // StartSend=1,transmission of data starts
    }

    // A la espera de recibir datos para completar
    i = 2000; // i????????,??M1???????25ms   ??? i De acuerdo con el ajuste de frecuencia de reloj, el tiempo mï¿½ximo de espera operaciï¿½n M1 25ms tarjeta??
    do
    {
        // CommIrqReg[7..0]
        // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
        n = readMFRC522(CommIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    clearBitMask(BitFramingReg, 0x80); // StartSend=0

    if (i != 0)
    {
        if (!(readMFRC522(ErrorReg) & 0x1B)) // BufferOvfl Collerr CRCErr ProtecolErr
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {
                status = MI_NOTAGERR; //??
            }

            if (command == PCD_TRANSCEIVE)
            {
                n = readMFRC522(FIFOLevelReg);
                lastBits = readMFRC522(ControlReg) & 0x07;
                if (lastBits)
                {
                    *backLen = (n - 1) * 8 + lastBits;
                }
                else
                {
                    *backLen = n * 8;
                }

                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAX_LEN)
                {
                    n = MAX_LEN;
                }

                //??FIFO??????? Lea los datos recibidos en el FIFO
                for (i = 0; i < n; i++)
                {
                    backData[i] = readMFRC522(FIFODataReg);
                }
            }
        }
        else
        {
            status = MI_ERR;
        }
    }

    // SetBitMask(ControlReg,0x80);           //timer stops
    // Write_MFRC522(CommandReg, PCD_IDLE);

    return status;
}

void clearBitMask(unsigned char reg, unsigned char mask)
{
    /*Limpia el bit*/
    unsigned char tmp;
    tmp = readMFRC522(reg);
    writeMFRC522(reg, tmp & (~mask)); // clear bit mask
}

void setBitMask(unsigned char reg, unsigned char mask)
{
    /*Setea el bit*/
    unsigned char tmp;
    tmp = readMFRC522(reg);
    writeMFRC522(reg, tmp | mask); // set bit mask
}

bool readCardSerial(void)
{
    /*Lee el valor del tag*/
    unsigned char status;
    unsigned char str[MAX_LEN];

    // Anti-colisiï¿½n, devuelva el nï¿½mero de serie de tarjeta de 4 bytes
    status = anticoll(str);
    memcpy(serNum, str, 5);

    if (status == MI_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

unsigned char anticoll(unsigned char *serNum)
{
    unsigned char status;
    unsigned char i;
    unsigned char serNumCheck = 0;
    unsigned int unLen;

    // ClearBitMask(Status2Reg, 0x08);      //TempSensclear
    // ClearBitMask(CollReg,0x80);         //ValuesAfterColl
    writeMFRC522(BitFramingReg, 0x00); // TxLastBists = BitFramingReg[2..0]

    serNum[0] = PICC_ANTICOLL;
    serNum[1] = 0x20;
    status = MFRC522ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

    if (status == MI_OK)
    {
        //?????? Compruebe el nï¿½mero de serie de la tarjeta
        for (i = 0; i < 4; i++)
        {
            serNumCheck ^= serNum[i];
        }
        if (serNumCheck != serNum[i])
        {
            status = MI_ERR;
        }
    }

    // SetBitMask(CollReg, 0x80);      //ValuesAfterColl=1

    return status;
}

void halt()
{
    /*Finaliza la lectura del tag*/
    unsigned char status;
    unsigned int unLen;
    unsigned char buff[4];

    buff[0] = PICC_HALT;
    buff[1] = 0;
    calculateCRC(buff, 2, &buff[2]);

    status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

void calculateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
    /*Realiza el calculo de CRC para la comunicacion con el tag*/
    unsigned char i, n;

    clearBitMask(DivIrqReg, 0x04);  // CRCIrq = 0
    setBitMask(FIFOLevelReg, 0x80); // Claro puntero FIFO
    // Write_MFRC522(CommandReg, PCD_IDLE);

    // Escribir datos en el FIFO
    for (i = 0; i < len; i++)
    {
        writeMFRC522(FIFODataReg, *(pIndata + i));
    }
    writeMFRC522(CommandReg, PCD_CALCCRC);

    // Esperar a la finalizacion de calculo del CRC
    i = 0xFF;
    do
    {
        n = readMFRC522(DivIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x04)); // CRCIrq = 1

    // Lea el calculo de CRC
    pOutData[0] = readMFRC522(CRCResultRegL);
    pOutData[1] = readMFRC522(CRCResultRegM);
}
