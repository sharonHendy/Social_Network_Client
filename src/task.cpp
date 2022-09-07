#include "../include/task.h"

Task::Task(EncoderDecoder& encoderDecoder) :
shouldTerminate(false),encdec(encoderDecoder){}

void Task::terminate() {
    shouldTerminate = true;
}

Task::~Task() = default;

SocketListener::SocketListener(EncoderDecoder& encoderDecoder) : Task(encoderDecoder) {}

void SocketListener::run() {
    while(!shouldTerminate){
            std::string response;

            bool isSuccessful = encdec.decode(response); //gets the response from the server and puts them in response
            if (!isSuccessful) {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
          //  std::lock_guard <std::mutex> lock(_mutex);
            std::cout << response << std::endl;

            if(response == "ACK 3"){ //ack for logout request
                logoutStatus.gotAckResponse = true;
                logoutStatus.received = true;
                break;
            }else if(response == "ERROR 3") {
                logoutStatus.gotAckResponse = false;
                logoutStatus.received = true;
            }
    }
}
SocketListener::~SocketListener()= default ;


InputReader::InputReader(EncoderDecoder& encoderDecoder) : Task(encoderDecoder){}

void InputReader::run() {
    while(!shouldTerminate){
        const short bufsize = 1024;
        char buf[bufsize];

        std::cin.getline(buf, bufsize, '\n'); //read user input

        char newCommand[1024];
        std::string opcodeStr;
        unsigned int i = 0;
        //get the command type (first word)
        while(buf[i] != ' ' && buf[i] != '\0'){
            opcodeStr += buf[i];
            i = i + 1;
        }
        bool sentSuccessfully = encdec.buildCommandAndSend(buf, newCommand,opcodeStr);

        if (!sentSuccessfully) {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

        if(opcodeStr == "LOGOUT"){
            while(!logoutStatus.received){
                //wait
            }
            if(logoutStatus.received && logoutStatus.gotAckResponse){
                break;
            }else{
                logoutStatus.received = false;
            }
        }
    }
}
InputReader::~InputReader() = default;




