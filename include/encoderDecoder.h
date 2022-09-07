#ifndef ENCODERDECODER_H
#define ENCODERDECODER_H

#include <iostream>
#include "connectionHandler.h"

class EncoderDecoder{
public:
    EncoderDecoder(ConnectionHandler& connectionHandler);
    ConnectionHandler &connectionHandler;
    bool decode(std::string& response);
    bool buildCommandAndSend(char (& command)[1024], char (& newCommand)[1024], std::string opcodeStr);
    short bytesToShort(char* bytesArr);
    void shortToBytes(short num, char* bytesArr);
    int copyArrayAndReplaceSpaces(char command[], char newCommand[], int from, int len);
};

#endif
