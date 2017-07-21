#ifndef SERVER_H
#define SERVER_H

#include "File.h"

class Server
{
public:
    Server();
    ~Server() = default;
    static string address;

	static void initializeManager();
    static void receiveFiles();
    static void sendFile(File file);
    static void synchronize();
};

#endif // !SERVER_H
