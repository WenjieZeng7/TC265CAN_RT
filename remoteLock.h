#ifndef REMOTE_LOCK_H_
#define REMOTE_LOCK_H_
#include "Platform_Types.h"
#define BufferLen (10)
/*global varialbe related to remotelock controller */
struct AuthInfo {
    uint64 indentification;
    uint64 startTime;
    uint64 endTime;
};


struct AuthInfo_BCD {
    uint8 indentification[6];
    uint8 startTime[6];
    uint8 endTime[6];
};


struct BackEndInfo {
    struct AuthInfo authInfo[BufferLen];
    int remoteLockControl;  //0 means no lock according to time stamp,1 means open,2 means lock
};

struct UploadMessage {
    uint64 canRoute1;
    uint64 canRoute2;
    uint32 canID1;
    uint32 canID2;
    int lockState;  //0 unknown ,1 means open,2 means lock,
    int send;
};


#endif 
