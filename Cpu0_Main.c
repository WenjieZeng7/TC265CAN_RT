/**********************************************************************************************************************
 * \file Cpu0_Main.c
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
#include "remoteLock.h"
#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "SysSe/Bsp/Bsp.h"   
#include "Port/Io/IfxPort_Io.h"
#include "Multican/Can/IfxMultican_Can.h"       
#include "Flash_Programming.h"


IfxCpu_syncEvent g_cpuSyncEvent = 0;

//????CAN??卯锟�?????MessageObject??
// CAN handle
IfxMultican_Can can;
// Nodes handles
IfxMultican_Can_Node canSrcNode;
// Message Object handles
IfxMultican_Can_MsgObj canSrcMsgObj;
IfxMultican_Can_MsgObj canRcvMsgObj;
IfxMultican_Can_MsgObj canRcvMsgObj2;

void CAN_SendSingle(uint32 id, uint32 high, uint32 low);


#define ISR_PRIORITY_CAN_RX         1                           /* Define the CAN RX interrupt priority              */
#define ISR_PRIORITY_CAN_TX         2                           /* Define the CAN TX interrupt priority              */
#define ISR_PRIORITY_CAN_RX_2       3                           /* Define the CAN RX interrupt priority              */

#define TX_INTERRUPT_SRC_ID         IfxMultican_SrcId_0         /* Transmit interrupt service request ID             */
#define RX_INTERRUPT_SRC_ID         IfxMultican_SrcId_1         /* Receive interrupt service request ID              */
#define RX_INTERRUPT_SRC_ID_2       IfxMultican_SrcId_2         /* Receive interrupt service request ID              */
IFX_INTERRUPT(canIsrTxHandler, 0, ISR_PRIORITY_CAN_TX);
IFX_INTERRUPT(canIsrRxHandler, 0, ISR_PRIORITY_CAN_RX);
IFX_INTERRUPT(canIsrRxHandler_2, 0, ISR_PRIORITY_CAN_RX_2);

#define MEM(address)                *((uint32 *)(address))      /* Macro to simplify the access to a memory address */
#define DFLASH_STARTING_ADDRESS     0xAF000000                  /* Address of the DFLASH where the data is written  */
struct UploadMessage uploadMessage = {
    .lockState = 0  //0 means lock state is unknown
};
uint64 ICCode = 1234567;
void canIsrRxHandler_2(void)
{
    IfxPort_togglePin(&MODULE_P33, 9);
    IfxMultican_Status readStatus;
    IfxMultican_Message rxMsg; 

    /* Read the received CAN message and store the status of the operation */
    readStatus = IfxMultican_Can_MsgObj_readMessage(&canRcvMsgObj2, &rxMsg); 
    
    if (rxMsg.id == 0 /*ICCode*/)
    {
        ICCode = rxMsg.data[1];
        ICCode = (ICCode << 32) + rxMsg.data[0];
    }
    else if (rxMsg.id == 1 /*Can Route 1*/)
    {
        uploadMessage.send = uploadMessage.send | 0x01;
        uploadMessage.canRoute1 = rxMsg.data[1];
        uploadMessage.canRoute1 = (uploadMessage.canRoute1 << 32) + rxMsg.data[0];
        uploadMessage.canID1 = 1;
    }
    else if (rxMsg.id == 2 /*Can Route 2*/)
    {
        uploadMessage.send = uploadMessage.send | 0x02;
        uploadMessage.canRoute2 = rxMsg.data[1];
        uploadMessage.canRoute2 = (uploadMessage.canRoute1 << 32) + rxMsg.data[0];
        uploadMessage.canID2 = 2;
    }
    else if (rxMsg.id == 3 /*Lock state*/)
    {
        uploadMessage.send = uploadMessage.send | 0x04;
        uploadMessage.lockState = rxMsg.data[0];
    }
    CAN_SendSingle(0x12345200, rxMsg.data[0], rxMsg.data[1]); 
}

#define WAIT_TIME 500   /* Wait time constant in milliseconds   */

//?????搂脴???????? ???LED1
void canIsrTxHandler(void)
{
    IfxPort_togglePin(&MODULE_P33, 8);
}

//?????搂脴???????? ????LED2
void canIsrRxHandler(void)
{
    IfxPort_togglePin(&MODULE_P33, 9);
    IfxMultican_Status readStatus;      
    IfxMultican_Message rxMsg;          
    /* Read the received CAN message and store the status of the operation */
    readStatus = IfxMultican_Can_MsgObj_readMessage(&canRcvMsgObj, &rxMsg);     

    
    if(rxMsg.id == 0/*ICCode*/)
    {
        ICCode = rxMsg.data[1];
        ICCode = (ICCode << 32) + rxMsg.data[0];
    }else if(rxMsg.id == 1/*Can Route 1*/)
    {
        uploadMessage.send = uploadMessage.send | 0x01;
        uploadMessage.canRoute1 = rxMsg.data[1];
        uploadMessage.canRoute1 = (uploadMessage.canRoute1 << 32) + rxMsg.data[0];
        uploadMessage.canID1 = 1;
    }
    else if(rxMsg.id == 2/*Can Route 2*/)
    {
        uploadMessage.send = uploadMessage.send | 0x02;
        uploadMessage.canRoute2 = rxMsg.data[1];
        uploadMessage.canRoute2 = (uploadMessage.canRoute1 << 32) + rxMsg.data[0];
        uploadMessage.canID2 = 2;
    }
    else if(rxMsg.id == 3/*Lock state*/)
    {
        uploadMessage.send = uploadMessage.send | 0x04;
        uploadMessage.lockState = rxMsg.data[0];
    }
    CAN_SendSingle(0x12345201, rxMsg.data[0], rxMsg.data[1]);

}

void CanApp_init(void)
{
    // create configuration
    IfxMultican_Can_Config canConfig;
    IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
    // initialize interrupt priority
    canConfig.nodePointer[TX_INTERRUPT_SRC_ID].priority = ISR_PRIORITY_CAN_TX;
    canConfig.nodePointer[RX_INTERRUPT_SRC_ID].priority = ISR_PRIORITY_CAN_RX;  
    canConfig.nodePointer[RX_INTERRUPT_SRC_ID_2].priority = ISR_PRIORITY_CAN_RX_2;   
    // initialize module
    IfxMultican_Can_initModule(&can, &canConfig);

    // create CAN node config
    IfxMultican_Can_NodeConfig canNodeConfig;
    IfxMultican_Can_Node_initConfig(&canNodeConfig, &can);
    canNodeConfig.baudrate = 125000; //
    canNodeConfig.nodeId = IfxMultican_NodeId_0;
    canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
    canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
    IfxMultican_Can_Node_init(&canSrcNode, &canNodeConfig);

    // create message object config
    IfxMultican_Can_MsgObjConfig canMsgObjConfig;
    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
    // assigned message object:
    canMsgObjConfig.msgObjId = 0;
    canMsgObjConfig.frame = IfxMultican_Frame_transmit;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = TRUE;
    canMsgObjConfig.txInterrupt.enabled = TRUE;  
    canMsgObjConfig.txInterrupt.srcId = TX_INTERRUPT_SRC_ID;  
    // initialize receive message object
    IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);

    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
    canMsgObjConfig.msgObjId = 1;
    canMsgObjConfig.messageId = 0x00000000;     
    canMsgObjConfig.acceptanceMask = 0xFFFFFFF0;
    //canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
    canMsgObjConfig.frame = IfxMultican_Frame_receive;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = TRUE;
    canMsgObjConfig.rxInterrupt.enabled = TRUE;   
    canMsgObjConfig.rxInterrupt.srcId = RX_INTERRUPT_SRC_ID;   
    // initialize message object
    IfxMultican_Can_MsgObj_init(&canRcvMsgObj, &canMsgObjConfig);


    IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
    canMsgObjConfig.msgObjId = 2;
    canMsgObjConfig.messageId = 0x12345678;   
    canMsgObjConfig.acceptanceMask = 0xFFFFFFFF;
    //canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL; 
    canMsgObjConfig.frame = IfxMultican_Frame_receive;
    canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
    canMsgObjConfig.control.extendedFrame = TRUE;
    canMsgObjConfig.rxInterrupt.enabled = TRUE;    
    canMsgObjConfig.rxInterrupt.srcId = RX_INTERRUPT_SRC_ID_2;    
    // initialize message object
    IfxMultican_Can_MsgObj_init(&canRcvMsgObj2, &canMsgObjConfig);
}


void CAN_SendSingle(uint32 id, uint32 high, uint32 low)
{
    // Initialize the message structure
    IfxMultican_Message txMsg;
    IfxMultican_Message_init(&txMsg, id, low, high, IfxMultican_DataLengthCode_8);

    // Transmit Data
    while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy );

}



/*
@currentTime variable address passed in, used to store the current time;
@return 0 means success, 1 means failure
*/
int GetCurrentTime(uint64* currentTime)
{
    //dummy function that increase the time;
    *currentTime = ((*currentTime)++)%3600000*12 ;
    return 0;
}

int core0_main(void)
{
    struct AuthInfo authInfo[BufferLen];
    struct AuthInfo temp[BufferLen]={0};
    struct BackEndInfo backEndInfo ={
        .remoteLockControl = 0
    };
    uint64 currentTime=0;
    int LockStatus;     //useless for now
    //the two least significant bits determine the desired lock state: 0 means no operation, 1 means open,2 means lock
    //the third bit is on if lock is determined by timestamp
    //the fourth bit is on if lock is determined by cloud command
    int LockControl=0;    
    int ret;
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    initTime();

    //LED
    IfxPort_setPinMode(&MODULE_P33, 8, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(&MODULE_P33, 9, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinHigh(&MODULE_P33, 8);
    IfxPort_setPinLow(&MODULE_P33, 9);


    CanApp_init();

    /*can usart rte initialization, read from the flash to get the AuthInfo structure*/

    /*flash test**************************************************************************************/
    // for(int i=0;i<BufferLen;i++)
    // {
    //     authInfo[i].indentification = 3 * i;
    //     authInfo[i].startTime = 3 * i + 1;
    //     authInfo[i].endTime = 3 * i + 2;
    // }
    // eraseDflash();
    // writeAuthInfo(authInfo);
    // readAuthInfo(temp);
    /************************************************************************************************/

    readAuthInfo(authInfo);
    for (int i = 0; i < BufferLen; i++)
    {
        backEndInfo.authInfo[i]=authInfo[i];
    }

    while(1)
    {
        //restore lock control to no operation
        LockControl=0;
//        CAN_SendSingle(0x01234567,0x5678,0x1234);   //碌脥脦禄 赂脽脦禄
// //
//        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));    /* Wait 500 milliseconds            */
//        IfxPort_togglePin(&MODULE_P33, 8);
    /*1. get the current time zeng*/
    ret = GetCurrentTime(&currentTime);
    if(ret)
    {
        while(1);
    }
    /*2. authenticate if the driver can use the car to determine the desiring lock state*/
    if (backEndInfo.remoteLockControl == 0)
    {
        for (int i = 0; i < BufferLen; i++)
        {
            if (authInfo[i].indentification == ICCode && currentTime > authInfo[i].startTime && currentTime < authInfo[i].endTime)
            {
                //1 means open, the third bit is on means the lock state is determined by time stamp
                LockControl = 1 | 4;
                break;
            }
        }
    }else if(backEndInfo.remoteLockControl == 1)
    {
        //1 means open, the forth bit is on means the lock state is determined by cloud command
        LockControl = 1 | 8;
    }else if(backEndInfo.remoteLockControl == 2)
    {
        //1 means closed, the forth bit is on means the lock state is determined by cloud command
        LockControl = 2 | 8;
    }
    /*3. send can message to the electric control module that indicate whether the lock should be open or on*/ 
    CAN_SendSingle(0x98354990, LockControl, 0);
    /*4. read usart port for message from the 4G module, fill in the struct backEndInfo zeng*/
    /*5. send the struct UploadMessage to the cloud according to the send value zeng*/
    /*6. compare AuthInfo and BackEndInfo, update AuthInfo and dflash if they are not equal*/
    int i = 0;
    for (; i < BufferLen; i++)
    {
        if (authInfo[i].indentification != backEndInfo.authInfo[i].indentification ||
            authInfo[i].startTime != backEndInfo.authInfo[i].startTime ||
            authInfo[i].endTime != backEndInfo.authInfo[i].endTime)
        {
            break;
        }
    }
    if( i != BufferLen){
        for (i=0; i < BufferLen; i++)
        {
            authInfo[i].indentification = backEndInfo.authInfo[i].indentification;
            authInfo[i].startTime = backEndInfo.authInfo[i].startTime;
            authInfo[i].endTime = backEndInfo.authInfo[i].endTime;
        }
        eraseDflash();
        writeAuthInfo(authInfo);
    }
    }
    return (1);
}
