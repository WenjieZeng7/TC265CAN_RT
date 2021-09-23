#include "jtt808.h"
#include "string.h"
#include "Platform_Types.h"
#include "ASCLIN_UART.h"
#include "jt808.h"
#include "Cpu0_Main.h"
#include "remoteLock.h"

//terminal id located in jt808message header
#define PHONE "768901005626"

#define TermAuthID      0x0102 //�ն��Ϸ��ļ�Ȩ��Ϣ
#define CanMsgID        0x0705 //�ն��ϴ�Can��Ϣ
#define ServGeneResID   0x8001 //ƽ̨ͨ��Ӧ��
#define LockStateID     0x0F05 //��״̬�ϱ�

#define TermGeneResID   0x0001 //�ն�ͨ��Ӧ��
#define LockAuthMsgID   0x8F00 //��Ȩ��Ϣ�·�
#define LockContMsgID   0x8F01 //��������Ϣ�·�

uint16 tl_net_flow_id = 0x06E1;
//static TLGPSData g_tl_gps_data = {0};
//LockAuthMsg tl_canbus_lock_auth_msg={0};

void test (void)
{
//    uint8 in[21] = {0x7E, 0x01, 0x02, 0x00, 0x06, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x06, 0xE1, 0x44, 0x45, 0x4A,
//            0x54, 0x4E, 0x50, 0x6D, 0x7E};  //�ն��ϴ���Ȩ��Ϣ
//    uint8 in[33] = {0x7E, 0x8F, 0x00, 0x00, 0x12, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x00, 0x03, 0x30,0x31, 0x32, 0x33,
//            0x34, 0x35, 0x21, 0x09, 0x19, 0x14, 0x51, 0x45, 0x21, 0x10, 0x31, 0x02, 0x56, 0x03, 0x44, 0x7E}; //�������·�����Ȩ��Ϣ
    uint8 in[16] = {0x7E, 0x8F, 0x01, 0x00, 0x01, 0x76, 0x89, 0x01, 0x00, 0x56, 0x26, 0x06, 0xE1, 0x02, 0xE6, 0x7E}; //�������·�������������
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
 * �������úõ��ն�ID����Ȩ�롣�����жϼ�Ȩ���ģ���ͨ��UART���ӵ�4Gģ�鷢�͵���������
 */
void dataReady (void)
{
    //�ô�������ն˷��ͼ�Ȩ��Ϣ���Ѳ��Կ���
    uint8 binarySeq[19];  //������Ϣͷ12�ֽڣ�����Ϣ��6���ֽڣ���1���ֽڵ�У����
    PackageData packageData;
    packageData.msgHeader.msgId = TermAuthID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 6; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������
    //    g_byte_array_prepend(msg_data,(guint8*)binarySeq,12);
    uint8 msg[] = "DEJTNP";  //c�����е�char��1���ֽڳ���
    for (int i = 0; i < 6; i++)
    {
        binarySeq[i + 12] = msg[i];  //��Ϣͷͳһ����12�ֽ�
    }
    binarySeq[18] = 'a'; //��Ĭ�ϸ�ֵһ��У����
    SetCheckSum(binarySeq, 19);
    tl_net_flow_id++;
    int binarySeqResLen = 19 * 2 + 2; // max len
    uint8 binarySeqRes[40] = ""; //�ַ�������""��ֵ����ȫ����Ϊ'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 19, &binarySeqResLen);  //binarySeqResLen����Ƿ��ص����鳤�ȣ�������21
//    UART_PutBuff(binarySeq, 19); //'\0'���ַ����Ľ�����־���������ǲ�����
//     UART_PutStr(binarySeq);  //�������ַ�ʽ��������0x00ʱ,��ֹͣ���ͣ���ΪASCII��0Ϊ���ַ���
//    delayMs(500);
//    UART_PutBuff(binarySeqRes, binarySeqResLen);
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //û�б���ֵ�ģ�ȫ�����ǿ��ַ���ASCII��Ϊ0��

    /*����forѭ����16��������ת���ɶ�Ӧ���ַ����飬��uint8 hex[1] = 0x7E��
     *ת��Ϊuint8 str[2] = "7E"��7Ϊ�ַ�7����Ӧ��ASCII��Ϊ55��EΪ�ַ�E����Ӧ��ASCII��Ϊ69����str�����д�ŵļ�ΪASCII��ֵ*/
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

    //������Ϊ�˵õ��Զ���ķ������·���Ϣ��У����
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
//    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������
//    //    g_byte_array_prepend(msg_data,(guint8*)binarySeq,12);
//    uint8 msg[] = ""; //���ַ������ǿո񣩴�����ASCII��0��
//    for (int i = 0; i < 1; i++)
//    {
//        binarySeq[i + 12] = msg[i];
//    }
//    binarySeq[13] = 'a'; //��Ĭ�ϸ�ֵһ��У����
//    SetCheckSum(binarySeq, 14);
//    UART_PutBuff(binarySeq, 14); //'\0'���ַ����Ľ�����־��
}

/**
 * �ն������������CAN��Ϣ��Ŀǰ���ٶȺ�������
 */
void canMsgSent (void)
{
    uint8 binarySeq[25];  //������Ϣͷ12�ֽڣ�����Ϣ��12���ֽڣ���1���ֽڵ�У����
    PackageData packageData;
    packageData.msgHeader.msgId = CanMsgID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 12; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������

    /* �������CAN��Ϣ��ID�����������ݣ�IDռ4���ֽ�uint32��������ռ8���ֽ�uint64*/
    extern struct UploadMessage uploadMessage;
    uint8 *p = &uploadMessage;
    if (uploadMessage.send & 0x01 == 1) //����CAN��Ϣ1
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
    else if (uploadMessage.send & 0x02 == 1)  //����CAN��Ϣ2
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
    else // ������������ͣ�ֱ�ӽ����ú���
    {
        return;
    }
    binarySeq[24] = 'a'; //��Ĭ�ϸ�ֵһ��У����
    SetCheckSum(binarySeq, 25);
    tl_net_flow_id++;
    int binarySeqResLen = 25 * 2 + 2; // max len
    uint8 binarySeqRes[52] = ""; //�ַ�������""��ֵ����ȫ����Ϊ'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 25, &binarySeqResLen);  //binarySeqResLen����Ƿ��ص����鳤��
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //û�б���ֵ�ģ�ȫ�����ǿ��ַ���ASCII��Ϊ0��

    /*����forѭ����16��������ת���ɶ�Ӧ���ַ����飬��uint8 hex[1] = 0x7E��
     *ת��Ϊuint8 str[2] = "7E"��7Ϊ�ַ�7����Ӧ��ASCII��Ϊ55��EΪ�ַ�E����Ӧ��ASCII��Ϊ69����str�����д�ŵļ�ΪASCII��ֵ*/
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
 * �ն��ϴ���״̬��0: ������1����
 */
void lockStateSend (void)
{
    extern struct UploadMessage uploadMessage;
    if (uploadMessage.lockState == 0){
        return;
    }
    uint8 binarySeq[14];  //������Ϣͷ12�ֽڣ�����Ϣ��1���ֽڣ���1���ֽڵ�У����
    PackageData packageData;
    packageData.msgHeader.msgId = LockStateID;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 12; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������

    /* �������CAN��Ϣ��ID�����������ݣ�IDռ4���ֽ�uint32��������ռ8���ֽ�uint64*/
    binarySeq[12] = uploadMessage.lockState - 1;
    binarySeq[13] = 'a'; //��Ĭ�ϸ�ֵһ��У����
    SetCheckSum(binarySeq, 14);
    tl_net_flow_id++;
    int binarySeqResLen = 14 * 2 + 2; // max len
    uint8 binarySeqRes[30] = ""; //�ַ�������""��ֵ����ȫ����Ϊ'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 14, &binarySeqResLen);  //binarySeqResLen����Ƿ��ص����鳤��
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //û�б���ֵ�ģ�ȫ�����ǿ��ַ���ASCII��Ϊ0��

    /*����forѭ����16��������ת���ɶ�Ӧ���ַ����飬��uint8 hex[1] = 0x7E��
     *ת��Ϊuint8 str[2] = "7E"��7Ϊ�ַ�7����Ӧ��ASCII��Ϊ55��EΪ�ַ�E����Ӧ��ASCII��Ϊ69����str�����д�ŵļ�ΪASCII��ֵ*/
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
 * �ն������������͵���������
 */
void heartBeat (void)
{
    uint8 binarySeq[13];  //������Ϣͷ12�ֽڣ�����Ϣ��0���ֽڣ���1���ֽڵ�У����
    PackageData packageData;
    packageData.msgHeader.msgId = 0x0002;

    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 0; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;

    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������
    binarySeq[12] = 'a'; //��Ĭ�ϸ�ֵһ��У����
    SetCheckSum(binarySeq, 13);
    tl_net_flow_id++;
    int binarySeqResLen = 13 * 2 + 2; // max len
    uint8 binarySeqRes[28] = ""; //�ַ�������""��ֵ����ȫ����Ϊ'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 13, &binarySeqResLen);  //binarySeqResLen����Ƿ��ص����鳤��
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //û�б���ֵ�ģ�ȫ�����ǿ��ַ���ASCII��Ϊ0��

    /*����forѭ����16��������ת���ɶ�Ӧ���ַ����飬��uint8 hex[1] = 0x7E��
     *ת��Ϊuint8 str[2] = "7E"��7Ϊ�ַ�7����Ӧ��ASCII��Ϊ55��EΪ�ַ�E����Ӧ��ASCII��Ϊ69����str�����д�ŵļ�ΪASCII��ֵ*/
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
 * �Ӵ��ڵĽ���fifo�ж�ȡ���ݵ�rxData[]�У�
 * ��rxData[]�е����ݽ��в����
 * ���ζԲ�������ݳ�ȥ��ʾλ������ת�塣
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
            tl_net_update_receive_msg(buffer, msg_len);  //���ڽ��յ�����Ϣ������ID���в�ͬ�Ĳ�����
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
 * ���յ�808��Ϣ��������ϢID���д���
 * binary_seq: 808��Ϣ��
 * len��808��Ϣ�����ܳ���
 */
int tl_net_update_receive_msg (uint8* binary_seq, int len)
{
    PackageData pack_data;
    //decode a received binary_seq
    DecodeForMsgHeader(binary_seq, &pack_data, len); //�ն˺Ų��ԣ���ˮ�Ų��ԣ���Ϣ�����������ذ������ն˺�+��ˮ��  +У���롣
    int msg_id = pack_data.msgHeader.msgId;  //��
    int body_len = pack_data.msgHeader.msgBodyProperties.msgLenth; //��

    switch (msg_id)
    {
        case ServGeneResID :  //ƽ̨ͨ��Ӧ��
        {
            serv_gene_resp_rev(pack_data.msgBody, body_len);
            break;
        }
        case LockAuthMsgID :  //��Ȩ��Ϣ�·�
        {
            lock_auth_msg_rev(pack_data.msgBody, body_len);   //������Ȩ��Ϣ
            resp_msg_add(&pack_data.msgHeader, CRR_SUCCESS);  //�����ն�ͨ��Ӧ��
            break;
        }
        case LockContMsgID :
        {
            /*��ȡ����������*/
            extern struct BackEndInfo backEndInfo;
            backEndInfo.remoteLockControl = pack_data.msgBody[0];
            resp_msg_add(&pack_data.msgHeader, CRR_SUCCESS);  //�����ն�ͨ��Ӧ��
            break;
        }
        default :
            resp_msg_add(&pack_data.msgHeader, CRR_UNSUPPORTED);
            break;
    }
    return TRUE;
}

/**
 * @brief resp_msg_add,�����ն�ͨ��Ӧ��
 * @param msg_head
 * @param res
 */
void resp_msg_add (MsgHeader *msg_head, CommonReplyResult res)
{
    /*�����Ҫ�����ն�ͨ��Ӧ���ƽ̨��Ϣ��ID����ˮ��*/
    int *id = TermGeneResID;
    CommonRespMsgBody resp_msg;
    resp_msg.replyId = msg_head->msgId;  //�ն�ҪӦ��� ƽ̨��Ϣ��ID
    resp_msg.replyFlowId = msg_head->flowId;  //�ն�ҪӦ��� ƽ̨��Ϣ����ˮ��
    resp_msg.replyCode = res; //���
    uint8 bf[5];
    EncodeForCRMB(&resp_msg, bf);

    //���¼���֮�󣬱���һֱ����
    uint8 binarySeq[18];  //������Ϣͷ12�ֽڣ�����Ϣ��5���ֽڣ���1���ֽڵ�У����
    PackageData packageData;
    packageData.msgHeader.msgId = 0x0001;
    packageData.msgHeader.msgBodyProperties.reservedBit = 0;
    packageData.msgHeader.msgBodyProperties.hasSubPackage = false;
    packageData.msgHeader.msgBodyProperties.encryptionType = NONE;
    packageData.msgHeader.msgBodyProperties.msgLenth = 5; //msg_data->len
    strcpy(packageData.msgHeader.terminalPhone, PHONE);
    packageData.msgHeader.flowId = tl_net_flow_id;
    EncodeForMsgHeader(&packageData, binarySeq);  //����Ϣͷת����2��������
    for (int i = 0; i < 5; i++)
    {
        binarySeq[i + 12] = bf[i];  //��Ϣͷͳһ����12�ֽ�
    }
    binarySeq[17] = 'a'; //��Ĭ�ϸ�ֵһ��У����
    SetCheckSum(binarySeq, 18);
    tl_net_flow_id++;
    int binarySeqResLen = 18 * 2 + 2; // max len
    uint8 binarySeqRes[38] = ""; //�ַ�������""��ֵ����ȫ����Ϊ'\0'
    DoEscapeForSend(binarySeq, binarySeqRes, 18, &binarySeqResLen);  //binarySeqResLen����Ƿ��ص����鳤��
    uint8 sendData[100] = "AT+QISENDEX=0,\""; //û�б���ֵ�ģ�ȫ�����ǿ��ַ���ASCII��Ϊ0��

    /*����forѭ����16��������ת���ɶ�Ӧ���ַ����飬��uint8 hex[1] = 0x7E��
     *ת��Ϊuint8 str[2] = "7E"��7Ϊ�ַ�7����Ӧ��ASCII��Ϊ55��EΪ�ַ�E����Ӧ��ASCII��Ϊ69����str�����д�ŵļ�ΪASCII��ֵ*/
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
 * @brief serv_gene_resp_rev,����serverͨ��Ӧ��
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
 * @brief lock_auth_msg_rev,������Ȩ��Ϣ
 * @param msg_body
 * @param len
 */
void lock_auth_msg_rev (uint8* msg_body, int len)
{
    extern struct BackEndInfo backEndInfo; //��main�����е�ȫ�ֱ�����
    extern int authID;
    LockAuthMsg *msg = (LockAuthMsg *) msg_body;
    struct AuthInfo_BCD *authInfo_BCD = msg;
    memcpy(&backEndInfo.authInfo[authID++], authInfo_BCD, sizeof(backEndInfo.authInfo[authID])); //tl_canbus_lock_auth_msg������Ȩ��Ϣ�ṹ��
    if (authID == 10)
    {
        authID = 0;
    }
}

/**
 * �ҳ����ڽ���fifo�е�808��Ϣ������ʼλ�úͳ���
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
