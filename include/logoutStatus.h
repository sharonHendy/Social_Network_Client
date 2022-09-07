#ifndef ASSIGNMENT3_LOGOUTSTATUS_H
#define ASSIGNMENT3_LOGOUTSTATUS_H

class LogoutStatus{
public:
    LogoutStatus(bool _received, bool _gotAckResponse);
    bool received;
    bool gotAckResponse;
};
#endif //ASSIGNMENT3_LOGOUTSTATUS_H
