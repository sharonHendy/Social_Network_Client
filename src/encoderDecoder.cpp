#include "../include/encoderDecoder.h"

EncoderDecoder::EncoderDecoder(ConnectionHandler& connectionHandler): connectionHandler(connectionHandler){};

bool EncoderDecoder::decode(std::string &response) {
    bool isSuccessful;
    char opcode[2];
    short shortOpcode;

    isSuccessful = connectionHandler.getBytes(opcode, 2);
    if(!isSuccessful){return false;}
    shortOpcode = bytesToShort(opcode);

    if(shortOpcode == 10){
        response += "ACK ";
        char msgOpcode[2];
        //gets the msg opcode
        isSuccessful = connectionHandler.getBytes(msgOpcode, 2);
        if(!isSuccessful){return false;}
        short shortMsgOpcode = bytesToShort(msgOpcode);
        response.append(std::to_string(shortMsgOpcode));
        //appends the optional part for - logstat stat and follow
        if(shortMsgOpcode ==4){
            response.append(" ");
            isSuccessful = connectionHandler.getFrameAscii(response,'\0'); //appends the userName
            if(!isSuccessful){return false;}
            response = response.substr(0, response.length() - 1);
        }else if(shortMsgOpcode == 7 || shortMsgOpcode == 8){
            response.append(" ");
            char optional[2];
            for(int i = 0; i < 4 ; i++){
                isSuccessful = connectionHandler.getBytes(optional, 2);
                if(!isSuccessful){return false;}
                short shortOptional = bytesToShort(optional);
                response.append(std::to_string(shortOptional) + " ");
            }
        }

    }else if(shortOpcode == 11){
        response += "ERROR ";
        char msgOpcode[2];
        //gets the msg opcode
        isSuccessful = connectionHandler.getBytes(msgOpcode, 2);
        if(!isSuccessful){return false;}
        short shortMsgOpcode = bytesToShort(msgOpcode);
        response.append(std::to_string(shortMsgOpcode));

    }else{
        response += "NOTIFICATION ";
        //checks if it's a PM or a POST
        char PMOrPublic[1];
        isSuccessful = connectionHandler.getBytes(PMOrPublic, 1);
        if(!isSuccessful){return false;}
        if(PMOrPublic[0] == '1'){
            response += "PUBLIC ";
        }else{
            response += "PM ";
        }
        isSuccessful = connectionHandler.getFrameAscii(response,'\0'); //appends the posting user
        if(!isSuccessful){return false;}
        response += " ";
        isSuccessful = connectionHandler.getFrameAscii(response,'\0'); //appends the content
        if(!isSuccessful){return false;}
    }
    //check that it reached the end of the message as it should
    char end[1];
    isSuccessful = connectionHandler.getBytes(end, 1);
    if(!isSuccessful || end[0] != ';'){
        return false;
    }

    return true;
}

bool EncoderDecoder::buildCommandAndSend(char (&command)[1024], char (&newCommand)[1024], std::string opcodeStr) {

    int len = 2;
    int i = opcodeStr.length()+1;
    if(opcodeStr == "REGISTER"){
        shortToBytes(1, newCommand);
        len = copyArrayAndReplaceSpaces(command, newCommand, i, len);
        newCommand[len] = '\0';
        len++;
    }else if(opcodeStr == "LOGIN"){
        shortToBytes(2, newCommand);
        len = copyArrayAndReplaceSpaces(command, newCommand, i, len);
        newCommand[len] = '\0';
        len++;
//        newCommand[len] = '1';
//        len++;

    }else if(opcodeStr == "LOGOUT"){
        shortToBytes(3, newCommand);

    }else if(opcodeStr == "FOLLOW"){
        shortToBytes(4, newCommand);
        newCommand[len] = command[i]; //copy follow/unfollow byte
        i = i + 2;
        len = len +1;
        len = copyArrayAndReplaceSpaces(command, newCommand, i, len);

    }else if(opcodeStr == "POST"){
        shortToBytes(5, newCommand);
        while(command[i] != '\n' && command[i] != '\0'){ //copies without replacing spaces
            newCommand[len] = command[i];
            len = len + 1;
            i = i + 1;
        }
        newCommand[len] = '\0'; //adds \0 at the end of the content
        len = len + 1;

    }else if(opcodeStr == "PM"){
        shortToBytes(6, newCommand);
        while(command[i] != ' '){ //copies userName only
            newCommand[len] = command[i];
            len = len + 1;
            i = i + 1;
        }
        newCommand[len] = '\0';
        i++;
        len++;
        while(command[i] != '\0' && command[i] != '\n'){ //copies content without replacing spaces
            newCommand[len] = command[i];
            len = len + 1;
            i = i + 1;
        }
        newCommand[len] = '\0'; //adds \0 at the end of the content
        i = i + 1;
        len = len + 1;

        //adding the sending time
        time_t rawtime;
        struct tm *ltm;
        char MY_TIME[18];
        time(&rawtime);
        ltm = localtime(&rawtime);
        strftime(MY_TIME,sizeof (MY_TIME), "%d-%m-%Y %H:%M", ltm);
        int r = 0;
        while (r < 16){
            newCommand[len] = MY_TIME[r];
            r++;
            i++;
            len = len + 1;
        }
        newCommand[len] = '\0';
        len = len + 1;

    }else if(opcodeStr == "LOGSTAT"){
        shortToBytes(7, newCommand);

    }else if(opcodeStr == "STAT"){
        shortToBytes(8, newCommand);
        while(command[i] != '\0'){
            newCommand[len] = command[i];
            len = len + 1;
            i = i + 1;
        }
        newCommand[len] = '\0'; //adds \0 at the end of the content
        len = len + 1;

    }else if(opcodeStr == "BLOCK"){
        shortToBytes(12, newCommand);
        len = copyArrayAndReplaceSpaces(command, newCommand, i, len);
        newCommand[len] = '\0';
        len++;
    }else{
        return false;
    }

    newCommand[len] = ';';
    return connectionHandler.sendBytes(newCommand, len+1);
}

short EncoderDecoder::bytesToShort(char* bytesArr){
    short result = (short)((bytesArr[0] & 0xff) << 8);
    result += (short)(bytesArr[01] & 0xff);
    return result;
}

void EncoderDecoder::shortToBytes(short num, char* bytesArr){
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

int EncoderDecoder::copyArrayAndReplaceSpaces(char command[], char newCommand[], int from, int len){
    while(command[from] != '\0' && command[from] != '\n'){ //copies the rest of the array, replacing space with '\0'.
        if(command[from] == ' '){
            newCommand[len] = '\0';
        }else{
            newCommand[len] = command[from];
        }
        len = len + 1;
        from = from + 1;
    }
    return len;
}



