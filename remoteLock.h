#ifndef REMOTE_LOCK_H_
#define REMOTE_LOCK_H_
#include "Platform_Types.h"
/*global varialbe related to remotelock controller */
struct AuthInfo {
    uint64 indentification;
    uint64 startTime;
    uint64 endTime;
};

struct BackEndInfo {
    struct AuthInfo authInfo[10];
    int remoteLockControl;
};

struct UploadMessage {
    uint64 canRoute1;
    uint64 canRoute2;
    uint32 canID1;
    uint32 canID2;
    int lockState;
    int send;
};


#endif 