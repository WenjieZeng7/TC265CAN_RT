/**********************************************************************************************************************
 * \file ASCLIN_UART.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxAsclin_Asc.h"
#include "IfxCpu_Irq.h"
#include "ASCLIN_UART.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define UART_BAUDRATE           115200                                  /* UART baud rate in bit/s                  */

//这里使用回环模式，两个管脚一直
//#define UART_PIN_RX             IfxAsclin1_RXB_P15_5_IN                 /* UART receive port pin                    */
//#define UART_PIN_TX             IfxAsclin1_TX_P15_4_OUT                 /* UART transmit port pin                   */

#define UART_PIN_RX             IfxAsclin0_RXB_P15_3_IN                 /* UART receive port pin                    */
#define UART_PIN_TX             IfxAsclin0_TX_P15_2_OUT                 /* UART transmit port pin                   */

/* Definition of the interrupt priorities */
#define INTPRIO_ASCLIN0_RX      18
#define INTPRIO_ASCLIN0_TX      19

#define UART_RX_BUFFER_SIZE     64                                      /* Definition of the receive buffer size    */
#define UART_TX_BUFFER_SIZE     64                                     /* Definition of the transmit buffer size   */
#define SIZE                    13                                      /* Size of the string                       */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
/* Declaration of the ASC handle */
static IfxAsclin_Asc g_ascHandle;

/* Declaration of the FIFOs parameters */
static uint8 g_ascTxBuffer[UART_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 g_ascRxBuffer[UART_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
//uint8 g_ascRxBuffer[UART_RX_BUFFER_SIZE] = {0};

/* Definition of txData and rxData */
uint8 g_txData[] = "Hello World!";
uint8 g_rxData[SIZE] = {''};

/* Size of the message */
Ifx_SizeT g_count = sizeof(g_txData);

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
//发送单个字符
//UART_PutStr(UART3, 'A');
void UART_PutChar (char ch)
{
    IfxAsclin_Asc_blockingWrite(&g_ascHandle, ch);
}

//发送字符串
//UART_PutStr(UART3, "123456789"); //发送9个字节
void UART_PutStr (char *str)
{
    while (*str)
    {
        UART_PutChar(*str++);
    }
}

//UART_PutBuff(UART4, "123456789",5);//实际发送5个字节‘1’‘2’‘3’‘4’‘5’
void UART_PutBuff (unsigned char *buff, unsigned long len)
{
    while (len--)
    {
        UART_PutChar(*buff);
        buff++;
//        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1));
    }
}

//获取接收单个字符
char UART_GetChar (void)
{
    uint8 data = 0;
    Ifx_SizeT count = 1;  //接收1个字节数据

    /* 获取接收到的一个字节数据 注意如果没有接收到，会一直等待 */
    IfxAsclin_Asc_read(&g_ascHandle, &data, &count, TIME_INFINITE);

    return data;
}

/**
 * 获取UART缓冲区数据
 * out：
 *      data
 * return：
 *      0：读取正常
 */
char UART_GetBuff (uint8 *data)
{
    Ifx_SizeT count = IfxAsclin_Asc_getReadCount(&g_ascHandle);  //获取数据个数
    count = count < 100 ? count : 100;
//    Ifx_SizeT count = 128;
    /* 获取接收到数据  */
    IfxAsclin_Asc_read(&g_ascHandle, data, &count, TIME_INFINITE);
    return 0;
}

void delayMs (uint32 milliSeconds)
{
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, milliSeconds));
}

sint32 getReadCount (void)
{
    return IfxAsclin_Asc_getReadCount(&g_ascHandle);
}

/* Adding of the interrupt service routines */
IFX_INTERRUPT(asclin0TxISR, 0, INTPRIO_ASCLIN0_TX);
//Each time when the last byte is taken out of the transmit FIFO (size is 16-bytes),
//the Transmit FIFO Level (TFL) flag is set and the interrupt service routine asclin0TxISR() is entered.
//The ISR calls IfxAsclin_Asc_isrTransmit() which refills the FIFO with the remaining bytes to be transmitted and clears the interrupt flag.
void asclin0TxISR (void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascHandle);
    /**
     * 这里写用户代码
     */
}

IFX_INTERRUPT(asclin0RxISR, 0, INTPRIO_ASCLIN0_RX);
//Each time when an UART byte is received, the Receive FIFO Level (RFL) flag is set and the interrupt service routine asclin0RxISR() is entered.
//The ISR calls IfxAsclin_Asc_isrReceive() which moves the received byte to the global array g_ascRxBuffer and clears the interrupt flag
int i = 0;

void asclin0RxISR (void)
{

    IfxAsclin_Asc_isrReceive(&g_ascHandle);

//    g_ascRxBStr("hahahaha123456\n");
    //用户代码uffer[i++] = i;
//
//    UART_Put
//    int count = sizeof(g_ascRxBuffer);
//    UART_PutChar('a');

//    uint8 date[] = "Hello World!";
//    uint8 data[13] = {''};
//    Ifx_SizeT count = sizeof(date)-1;
//    IfxAsclin_Asc_write(&g_ascHandle, date, &count, TIME_INFINITE);
//    char *p[] = {
//            "abcdefg\n",
//            "12345678\n",
//    };
//    for (int i = 0; i < 2; i ++){
//        UART_PutStr(p[i]);
//    }

}

/* This function initializes the ASCLIN UART module */
void init_ASCLIN_UART (void)
{
    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

    ascConfig.loopBack = 0;

    /* Set the desired baud rate */
//    ascConfig.baudrate.prescaler = 1;
    ascConfig.baudrate.baudrate = UART_BAUDRATE;

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority = INTPRIO_ASCLIN0_TX;
    ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN0_RX;
    ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());   //决定由什么来执行中断，可以是CPU也可以是DMA控制器

//    ascConfig.frame.frameMode    = IfxAsclin_FrameMode_asc;//数据帧模式
//    ascConfig.frame.dataLength   = IfxAsclin_DataLength_8; //数据长度
//    ascConfig.frame.stopBit      = IfxAsclin_StopBit_1;    //停止位
//    ascConfig.frame.shiftDir     = IfxAsclin_ShiftDirection_lsbFirst;//移位方向
//    ascConfig.frame.parityBit    = FALSE;//无奇偶校验

    /* FIFO configuration */
    ascConfig.txBuffer = &g_ascTxBuffer;
    ascConfig.txBufferSize = UART_TX_BUFFER_SIZE;
    ascConfig.rxBuffer = &g_ascRxBuffer;
    ascConfig.rxBufferSize = UART_RX_BUFFER_SIZE;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins = {
    NULL_PTR, IfxPort_InputMode_pullUp, /* CTS pin not used */
    &UART_PIN_RX, IfxPort_InputMode_pullUp, /* RX pin           */
    NULL_PTR, IfxPort_OutputMode_pushPull, /* RTS pin not used */
    &UART_PIN_TX, IfxPort_OutputMode_pushPull, /* TX pin           */
    IfxPort_PadDriver_cmosAutomotiveSpeed1};
    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_ascHandle, &ascConfig); /* Initialize module with above parameters */
}

/* This function sends and receives the string "Hello World!" */
//void send_receive_ASCLIN_UART_message(void)
//{
//    IfxAsclin_Asc_blockingWrite(&g_ascHandle, 'a'); //发送单个
//    IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);   /* Transmit data via TX */ //流传输
//    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &g_count, TIME_INFINITE);    /* Receive data via RX  */
//}
uint8 date[] = "Hello World!\n";
//uint8 data[13] = {'\n'};
Ifx_SizeT count1 = sizeof(date) - 1;
Ifx_SizeT count = 13;

void initGps (void)
{
    char *gpsInitInfo[] = {"AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1", "AT+QIACT=1", "AT+QHTTPCFG=\"contextid\",1",
            "AT+QHTTPURL=40,80", "http://xtrapath4.izatcloud.net/xtra2.bin", "AT+QHTTPGET=80",
            "AT+QHTTPREADFILE=\"RAM:xtra2.bin\",80", "AT+QIDEACT=1", "AT+QGPSEND", "AT+QGPSXTRA=1",
            "AT+QGPSXTRATIME=0,\"2021/07/19,06:00:00\",1,1,5", "AT+QGPSXTRADATA=\"RAM:xtra2.bin\"", "AT+QGPS=1",
            "AT+QGPSCFG=\"nmeasrc\",1", "AT+QGPSGNMEA=\"RMC\""};

    for (int i = 0; i < 15; i++)
    {
        UART_PutStr(gpsInitInfo[i]);
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1500));  //delay1500ms
    }
}

void initNet (void)
{
    char *netInitInfo[] = {"AT+QICLOSE=0","AT+QIDEACT=1","AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1", "AT+QIACT=1",
            "AT+QIOPEN=1,0,\"TCP\",\"115.29.196.107\",6809,0,0",
//                "AT+QISENDEX=0,\"7E0102000676890100562606E144454A544E506D7E\"",
            };
    for (int i = 0; i < 5; i++)
    {
        UART_PutStr(netInitInfo[i]);
        if (i == 1){
           waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5000)); //delay5000ms
        }
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1500)); //delay1500ms
        clearReceive();
    }
}

/*
 * 将接收fifo的数据清空
 */
void clearReceive (void)
{
    uint8 rxData1[100];
    UART_GetBuff(rxData1);
}
void readtest (void)
{
//    Ifx_SizeT count=9;
//    IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);
//    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 500)); //delay500ms
//    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &g_count, TIME_INFINITE);
//    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 500)); //delay500ms
//    uint8 g_txData1[] = "World!Hello ";
//    IfxAsclin_Asc_write(&g_ascHandle, g_txData1, &g_count, TIME_INFINITE);
//    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 500)); //delay500ms
//    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &g_count, TIME_INFINITE);

//    strncpy(g_txData,g_rxData);
//    Ifx_SizeT count = IfxAsclin_Asc_getReadCount(&g_ascHandle);
//    count = count < 100 ? count : 100;
//    IfxAsclin_Asc_read(&g_ascHandle, rxData, &count, TIME_INFINITE);
//    uint8 rxData[] = "AT+QISENDEX=0,\"7E0102000676890100562606E144454A544E506D7E\"";
//    Ifx_SizeT count = sizeof(rxData)-1;
//    IfxAsclin_Asc_write(&g_ascHandle, rxData, &count, TIME_INFINITE);

    UART_PutStr("AT+QISENDEX=0,\"7E0102000676890100562606E144454A544E506D7E\"");

    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 5000));

}

void test1 (void)
{
    uint8 in[21] = {0x7E, 0x01, 0x02, 0x00, 0x06, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x06, 0xE1, 0x44, 0x45, 0x4A,
            0x54, 0x4E, 0x50, 0x6D, 0x7E};
    Ifx_SizeT count = 22;
    IfxAsclin_Asc_write(&g_ascHandle, in, &count, TIME_INFINITE);
}
