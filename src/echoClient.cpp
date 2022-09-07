#include <stdlib.h>
#include <connectionHandler.h>
#include <task.h>
#include <encoderDecoder.h>
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/thread.hpp>

LogoutStatus logoutStatus(false, false);

int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);
    
    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    EncoderDecoder encdec(connectionHandler);
    SocketListener taskListener(encdec);
    InputReader taskInputReader(encdec);

    boost::thread threadListener(&SocketListener::run, &taskListener);
    boost::thread threadInputReader(&InputReader::run, &taskInputReader);
    threadListener.join();
    threadInputReader.join();

    return 0;
}