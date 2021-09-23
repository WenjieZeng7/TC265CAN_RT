#include "jtt808.h"
#include "string.h"
#include "Platform_Types.h"
#include "ASCLIN_UART.h"
#include "jt808.h"
#include "Cpu0_Main.h"
#include "remoteLock.h"

//terminal id located in jt808message header
#define PHONE "768901005626"

#define TermAuthID      0x0102 //终端上发的鉴权消息
#define CanMsgID        0x0705 //终端上传Can消息
#define ServGeneResID   0x8001 //平台通用应答
#define LockStateID     0x0F05 //锁状态上报

#define TermGeneResID   0x0001 //终端通用应答
#define LockAuthMsgID   0x8F00 //授权信息下发
#define LockContMsgID   0x8F01 //锁控制信息下发

uint16 tl_net_flow_id = 0x06E1;
//static TLGPSData g_tl_gps_data = {0};
//LockAuthMsg tl_canbus_lock_auth_msg={0};

void test (void)
{
//    uint8 in[21] = {0x7E, 0x01, 0x02, 0x00, 0x06, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x06, 0xE1, 0x44, 0x45, 0x4A,
//            0x54, 0x4E, 0x50, 0x6D, 0x7E};  //终端上传鉴权信息
//    uint8 in[33] = {0x7E, 0x8F, 0x00, 0x00, 0x12, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x00, 0x03, 0x30,0x31, 0x32, 0x33,
//            0x34, 0x35, 0x21, 0x09, 0x19, 0x14, 0x51, 0x45, 0x21, 0x10, 0x31, 0x02, 0x56, 0x03, 0x44, 0x7E}; //服务器下发的授权信息
    uint8 in[16] = {0x7E, 0x8F, 0x01, 0x00, 0x01, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x06, 0xE1, 0x02, 0xE6, 0x7E}; //服务器下发的锁控制命令
//    UART_PutBuff(out, 42);

//    UART_PutBuff(in, 16);
    unsigned char *buff = in;
    for (int i = 0; i < 16; i++)
    {
        UART_PutChar(*buff);
        buff++;
    }
    delayMs(1000);
//    UART_GetBuff(out);
//    delayMs(500);
}

/**
 * 根据设置好的终端ID，鉴权码。设置中断鉴权报文，并通过UART连接到4G模块发送到服务器。
 */
void dataReady (void)
{
    //该代码块是终端发送鉴权信息，已测试可用
    uint8 binarySeq[19];  //这是消息头12字节，加消息体6个字节，加1个字节的校验码
    PackageData packageData;
    packageData.msgHeader.msgId = TermAuthID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 6; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列
    //    g_byte_array_prepend(msg_data,(guint8*)binarySeq,12);
    uint8 msg[] = "DEJTNP";  //c语言中的char是1个字节长度
    for (int i = 0; i < 6; i++)
    {
        binarySeq[i + 12] = msg[i];  //消息头统一采用12字节
    }
    binarySeq[18] = 'a'; //先默认赋值一个校验码
    SetCheckSum(binarySeq, 19);
    tl_net_flow_id++;
    int binarySeqResLen = 19 * 2 + 2; // max len
    uint8 binarySeqRes[40] = ""; //字符数组用""赋值，则全部都为'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 19, &binarySeqResLen);  //binarySeqResLen最后是返回的数组长度，这里是21
//    UART_PutBuff(binarySeq, 19); //'\0'是字符串的结束标志符，这里是测试用
//     UART_PutStr(binarySeq);  //采用这种方式，在遇到0x00时,会停止发送，因为ASCII的0为空字符。
//    delayMs(500);
//    UART_PutBuff(binarySeqRes, binarySeqResLen);
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //没有被赋值的，全部都是空字符（ASCII码为0）

    /*以下for循环将16进制数组转换成对应的字符数组，如uint8 hex[1] = 0x7E，
     *转换为uint8 str[2] = "7E"（7为字符7，对应的ASCII码为55；E为字符E，对应的ASCII码为69）在str数组中存放的即为ASCII码值*/
    for (int i = 0; i < binarySeqResLen; i++)
    {
        int ahead = binarySeqRes[i] >> 4;
        int end = binarySeqRes[i] & 0xf;
        if (ahead <= 9)
        {
            sendData[2 * i + 15] = ahead + 48; //0-48
        }
        else
        {
            sendData[2 * i + 15] = ahead + 55;
        }

        if (end <= 9)
        {
            sendData[2 * i + 16] = end + 48; //A-65,a-97
        }
        else
        {
            sendData[2 * i + 16] = end + 55;
        }
    }
    sendData[15 + binarySeqResLen * 2] = '\"';

    UART_PutStr(sendData);
//    UART_PutBuff(binarySeqRes, binarySeqResLen);
    delayMs(1500);
    clearReceive();
    delayMs(1500);

//    delayMs(1500);
//    clearReceive();
    return;

    //以下是为了得到自定义的服务器下发信息的校验码
//    uint8 binarySeq[14];
//    PackageData packageData;
//    packageData.msgHeader.msgId = 0x8F01;
//
//    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
//    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
//    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
//    packageData.msgHeader.msgBodyProperties.msgLenth = 1; //msg_data->len
//    strcpy(packageData.msgHeader.terminalPhone, PHONE);
//    packageData.msgHeader.flowId = tl_net_flow_id;
//
//    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列
//    //    g_byte_array_prepend(msg_data,(guint8*)binarySeq,12);
//    uint8 msg[] = ""; //空字符（不是空格）代表着ASCII的0。
//    for (int i = 0; i < 1; i++)
//    {
//        binarySeq[i + 12] = msg[i];
//    }
//    binarySeq[13] = 'a'; //先默认赋值一个校验码
//    SetCheckSum(binarySeq, 14);
//    UART_PutBuff(binarySeq, 14); //'\0'是字符串的结束标志符
}

/**
 * 终端向服务器发送CAN消息，目前是速度和重量。
 */
void canMsgSent (void)
{
    uint8 binarySeq[25];  //这是消息头12字节，加消息体12个字节，加1个字节的校验码
    PackageData packageData;
    packageData.msgHeader.msgId = CanMsgID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 12; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列

    /* 这里放入CAN消息的ID和数据域内容，ID占4个字节uint32，数据域占8个字节uint64*/
    extern struct UploadMessage uploadMessage;
    uint8 *p = &uploadMessage;
    if (uploadMessage.send & 0x01 == 1) //发送CAN消息1
    {
        for (int i = 0; i < 4; i++)
        {
            binarySeq[15 - i] = *(p + 16 + i);
        }
        for (int i = 0; i < 8; i++)
        {
            binarySeq[23 - i] = *(p + i);
        }
        uploadMessage.send = uploadMessage.send & 0xFFFFFFFE;
    }
    else if (uploadMessage.send & 0x02 == 1)  //发送CAN消息2
    {
        for (int i = 0; i < 4; i++)
        {
            binarySeq[15 - i] = *(p + 24 + i);
        }
        for (int i = 0; i < 8; i++)
        {
            binarySeq[23 - i] = *(p + 8 + i);
        }
        uploadMessage.send = uploadMessage.send & 0xFFFFFFFD;

    }
    else // 其他情况不发送，直接结束该函数
    {
        return;
    }
    binarySeq[24] = 'a'; //先默认赋值一个校验码
    SetCheckSum(binarySeq, 25);
    tl_net_flow_id++;
    int binarySeqResLen = 25 * 2 + 2; // max len
    uint8 binarySeqRes[52] = ""; //字符数组用""赋值，则全部都为'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 25, &binarySeqResLen);  //binarySeqResLen最后是返回的数组长度
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //没有被赋值的，全部都是空字符（ASCII码为0）

    /*以下for循环将16进制数组转换成对应的字符数组，如uint8 hex[1] = 0x7E，
     *转换为uint8 str[2] = "7E"（7为字符7，对应的ASCII码为55；E为字符E，对应的ASCII码为69）在str数组中存放的即为ASCII码值*/
    for (int i = 0; i < binarySeqResLen; i++)
    {
        int ahead = binarySeqRes[i] >> 4;
        int end = binarySeqRes[i] & 0xf;
        if (ahead <= 9)
        {
            sendData[2 * i + 15] = ahead + 48; //0-48
        }
        else
        {
            sendData[2 * i + 15] = ahead + 55;
        }

        if (end <= 9)
        {
            sendData[2 * i + 16] = end + 48; //A-65,a-97
        }
        else
        {
            sendData[2 * i + 16] = end + 55;
        }
    }
    sendData[15 + binarySeqResLen * 2] = '\"';

    UART_PutStr(sendData);
    delayMs(1500);
    clearReceive();
    delayMs(1500);

    return;
}

/**
 * 终端上传锁状态：0: 上锁，1开锁
 */
void lockStateSend (void)
{
    extern struct UploadMessage uploadMessage;
    if (uploadMessage.lockState == 0){
        return;
    }
    uint8 binarySeq[14];  //这是消息头12字节，加消息体1个字节，加1个字节的校验码
    PackageData packageData;
    packageData.msgHeader.msgId = LockStateID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 12; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列

    /* 这里放入CAN消息的ID和数据域内容，ID占4个字节uint32，数据域占8个字节uint64*/
    binarySeq[12] = uploadMessage.lockState - 1;
    binarySeq[13] = 'a'; //先默认赋值一个校验码
    SetCheckSum(binarySeq, 14);
    tl_net_flow_id++;
    int binarySeqResLen = 14 * 2 + 2; // max len
    uint8 binarySeqRes[30] = ""; //字符数组用""赋值，则全部都为'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 14, &binarySeqResLen);  //binarySeqResLen最后是返回的数组长度
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //没有被赋值的，全部都是空字符（ASCII码为0）

    /*以下for循环将16进制数组转换成对应的字符数组，如uint8 hex[1] = 0x7E，
     *转换为uint8 str[2] = "7E"（7为字符7，对应的ASCII码为55；E为字符E，对应的ASCII码为69）在str数组中存放的即为ASCII码值*/
    for (int i = 0; i < binarySeqResLen; i++)
    {
        int ahead = binarySeqRes[i] >> 4;
        int end = binarySeqRes[i] & 0xf;
        if (ahead <= 9)
        {
            sendData[2 * i + 15] = ahead + 48; //0-48
        }
        else
        {
            sendData[2 * i + 15] = ahead + 55;
        }

        if (end <= 9)
        {
            sendData[2 * i + 16] = end + 48; //A-65,a-97
        }
        else
        {
            sendData[2 * i + 16] = end + 55;
        }
    }
    sendData[15 + binarySeqResLen * 2] = '\"';

    UART_PutStr(sendData);
    delayMs(1500);
    clearReceive();
    delayMs(1500);

    return;
}
/**
 * 终端往服务器发送的心跳包。
 */
void heartBeat (void)
{
    uint8 binarySeq[13];  //这是消息头12字节，加消息体0个字节，加1个字节的校验码
    PackageData packageData;
    packageData.msgHeader.msgId = 0x0002;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 0; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列
    binarySeq[12] = 'a'; //先默认赋值一个校验码
    SetCheckSum(binarySeq, 13);
    tl_net_flow_id++;
    int binarySeqResLen = 13 * 2 + 2; // max len
    uint8 binarySeqRes[28] = ""; //字符数组用""赋值，则全部都为'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 13, &binarySeqResLen);  //binarySeqResLen最后是返回的数组长度
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //没有被赋值的，全部都是空字符（ASCII码为0）

    /*以下for循环将16进制数组转换成对应的字符数组，如uint8 hex[1] = 0x7E，
     *转换为uint8 str[2] = "7E"（7为字符7，对应的ASCII码为55；E为字符E，对应的ASCII码为69）在str数组中存放的即为ASCII码值*/
    for (int i = 0; i < binarySeqResLen; i++)
    {
        int ahead = binarySeqRes[i] >> 4;
        int end = binarySeqRes[i] & 0xf;
        if (ahead <= 9)
        {
            sendData[2 * i + 15] = ahead + 48; //0-48
        }
        else
        {
            sendData[2 * i + 15] = ahead + 55;
        }

        if (end <= 9)
        {
            sendData[2 * i + 16] = end + 48; //A-65,a-97
        }
        else
        {
            sendData[2 * i + 16] = end + 55;
        }
    }
    sendData[15 + binarySeqResLen * 2] = '\"';

    UART_PutStr(sendData);
    delayMs(1500);
    clearReceive();
    delayMs(1500);

    return;
}

//void gpsData (void)
//{
//    g_tl_gps_data.state &= ~1;
//    g_tl_gps_data.initialized = TRUE;
//
//    UART_PutStr("AT+QGPSGNMEA=\"RMC\"");
//    delayMs(200);
//    uint8 rxData[100];
//    count = count < 100 ? count : 100;
//    UART_GetBuff(rxData);
//
//    LocationMsgUp msg, be_msg;
//    msg.alarm = 0;
//    msg.status = 0;
//    msg.latitude = g_tl_gps_data.latitude;
//    msg.longitude = g_tl_gps_data.longitude;
//    msg.speed = g_tl_gps_data.speed;
//    LocationMsgUpToBE(&msg, &be_msg);
//    g_byte_array_append(garray, (guint8 *) &be_msg, sizeof(be_msg));
//}

/**
 * 从串口的接收fifo中读取数据到rxData[]中；
 * 对rxData[]中的数据进行拆包；
 * 依次对拆包的数据除去标示位，并反转义。
 */
void getData (void)
{
    UART_PutStr("AT+QIRD=0");
    delayMs(500);
    sint16 count = getReadCount();
//    sint16 count = 100;
    uint8 rxData[100];
    count = count < 100 ? count : 100;
    UART_GetBuff(rxData);
    int raw_msg_start, raw_msg_len, msg_len;
    int buffer[100];
    while (findMsg(rxData, count, &raw_msg_start, &raw_msg_len))
    {
        DoEscapeForReceive(rxData + raw_msg_start, buffer, raw_msg_len, &msg_len);
        if (Validate(buffer, msg_len))
        {
            tl_net_update_receive_msg(buffer, msg_len);  //对于接收到的消息，根据ID进行不同的操作。
        }
        else
        {

        }
        count = count - raw_msg_len;
        for (int i = 0; i < count; i++)
        {
            rxData[i] = rxData[i + raw_msg_len + raw_msg_start];
        }
        if (count != 0)
        {
            rxData[count] = '\0';

        }
    }
}

/**
 * 接收的808消息包按照消息ID进行处理
 * binary_seq: 808消息包
 * len：808消息包的总长度
 */
int tl_net_update_receive_msg (uint8* binary_seq, int len)
{
    PackageData pack_data;
    //decode a received binary_seq
    DecodeForMsgHeader(binary_seq, &pack_data, len); //终端号不对，流水号不对，消息体在这里错误地包括了终端号+流水号  +校验码。
    int msg_id = pack_data.msgHeader.msgId;  //√
    int body_len = pack_data.msgHeader.msgBodyProperties.msgLenth; //√

    switch (msg_id)
    {
        case ServGeneResID :  //平台通用应答
        {
            serv_gene_resp_rev(pack_data.msgBody, body_len);
            break;
        }
        case LockAuthMsgID :  //授权信息下发
        {
            lock_auth_msg_rev(pack_data.msgBody, body_len);   //保存授权信息
            resp_msg_add(&pack_data.msgHeader, CRR_SUCCESS);  //发送终端通用应答
            break;
        }
        case LockContMsgID :
        {
            /*获取锁控制命令*/
            extern struct BackEndInfo backEndInfo;
            backEndInfo.remoteLockControl = pack_data.msgBody[0];
            resp_msg_add(&pack_data.msgHeader, CRR_SUCCESS);  //发送终端通用应答
            break;
        }
        default :
            resp_msg_add(&pack_data.msgHeader, CRR_UNSUPPORTED);
            break;
    }
    return TRUE;
}

/**
 * @brief resp_msg_add,发送终端通用应答
 * @param msg_head
 * @param res
 */
void resp_msg_add (MsgHeader *msg_head, CommonReplyResult res)
{
    /*获得需要进行终端通用应答的平台消息的ID和流水号*/
    int *id = TermGeneResID;
    CommonRespMsgBody resp_msg;
    resp_msg.replyId = msg_head->msgId;  //终端要应答的 平台消息的ID
    resp_msg.replyFlowId = msg_head->flowId;  //终端要应答的 平台消息的流水号
    resp_msg.replyCode = res; //结果
    uint8 bf[5];
    EncodeForCRMB(&resp_msg, bf);

    //以下加入之后，编译一直卡着
    uint8 binarySeq[18];  //这是消息头12字节，加消息体5个字节，加1个字节的校验码
    PackageData packageData;
    packageData.msgHeader.msgId = 0x0001;
    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 5; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;
    EncodeForMsgHeader(&packageData, binarySeq);  //将消息头转换成2进制序列
    for (int i = 0; i < 5; i++)
    {
        binarySeq[i + 12] = bf[i];  //消息头统一采用12字节
    }
    binarySeq[17] = 'a'; //先默认赋值一个校验码
    SetCheckSum(binarySeq, 18);
    tl_net_flow_id++;
    int binarySeqResLen = 18 * 2 + 2; // max len
    uint8 binarySeqRes[38] = ""; //字符数组用""赋值，则全部都为'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 18, &binarySeqResLen);  //binarySeqResLen最后是返回的数组长度
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //没有被赋值的，全部都是空字符（ASCII码为0）

    /*以下for循环将16进制数组转换成对应的字符数组，如uint8 hex[1] = 0x7E，
     *转换为uint8 str[2] = "7E"（7为字符7，对应的ASCII码为55；E为字符E，对应的ASCII码为69）在str数组中存放的即为ASCII码值*/
    for (int i = 0; i < binarySeqResLen; i++)
    {
        int ahead = binarySeqRes[i] >> 4;
        int end = binarySeqRes[i] & 0xf;
        if (ahead <= 9)
        {
            sendData[2 * i + 15] = ahead + 48; //0-48
        }
        else
        {
            sendData[2 * i + 15] = ahead + 55;
        }

        if (end <= 9)
        {
            sendData[2 * i + 16] = end + 48; //A-65,a-97
        }
        else
        {
            sendData[2 * i + 16] = end + 55;
        }
    }
    sendData[15 + binarySeqResLen * 2] = '\"';

    UART_PutStr(sendData);
    delayMs(1500);
    clearReceive();
    delayMs(1500);

    return;
}

/**
 * @brief serv_gene_resp_rev,接收server通用应答
 * @param msg_body
 * @param len
 */
void serv_gene_resp_rev (uint8* msg_body, int len)
{
    CommonRespMsgBody crmb;
    DecodeForCRMB(&crmb, msg_body);
    int flow_id = crmb.replyFlowId;
}

/**
 * @brief lock_auth_msg_rev,接收授权信息
 * @param msg_body
 * @param len
 */
void lock_auth_msg_rev (uint8* msg_body, int len)
{
    extern struct BackEndInfo backEndInfo; //在main函数中的全局变量。
    extern int authID;
    LockAuthMsg *msg = (LockAuthMsg *) msg_body;
    struct AuthInfo_BCD *authInfo_BCD = msg;
    memcpy(&backEndInfo.authInfo[authID++], authInfo_BCD, sizeof(backEndInfo.authInfo[authID])); //tl_canbus_lock_auth_msg就是授权信息结构体
    if (authID == 10)
    {
        authID = 0;
    }
}

/**
 * 找出串口接收fifo中的808消息包的起始位置和长度
 */
int findMsg (uint8 *rxData, sint16 count, int *raw_msg_start, int *raw_msg_len)
{
    int index[2];
    int index_count = 0;
    for (int i = 0; i < count; i++)
    {
        if (rxData[i] == 0x7E)
        {
            index[index_count] = i;
            index_count++;
            if (2 == index_count)
            {
                *raw_msg_start = index[0];
                *raw_msg_len = index[1] - index[0] + 1;
                //                g_message("raw_msg_start; %d",*raw_msg_start);
                //                g_message("raw_msg_len; %d",*raw_msg_len);
                return TRUE;
            }
        }
    }
    return FALSE;
}
