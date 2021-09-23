
/**********************************************************************************************************************
 * \file    jt808.h
 * \brief
 * \version V1.0.0
 * \date    2021Äê9ÔÂ15ÈÕ
 * \author  10947
 *********************************************************************************************************************/


#ifndef JT808_H_
#define JT808_H_

void dataReady (void);
void lock_auth_msg_rev (uint8* msg_body, int len);
void serv_gene_resp_rev (uint8* msg_body, int len);
void resp_msg_add(MsgHeader *msg_head, CommonReplyResult res);
int findMsg (uint8 *rxData, sint16 count, int *raw_msg_start, int *raw_msg_len);
void canMsgSent (void);
void lockStateSend (void);




#endif /* JT808_H_ */
