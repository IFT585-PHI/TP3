#ifndef SERVER_H
#define SERVER_H

#include "File.h"
#include <boost/asio.hpp>
using boost::asio::ip::tcp;
class Server
{
public:
    Server();
    ~Server() = default;
    static string address;
	tcp::acceptor acceptor;
	tcp::socket socket;

	Server(boost::asio::io_service & io_service);

	static void initializeManager();
    static void receiveFiles();
    static void sendFile(File file);
    static void synchronize();
	std::string readRequest(tcp::socket & socket);
	std::string sendResponse(tcp::socket & socket, const std::string& str);

private:
	void start_accept();
};



#endif // !SERVER_H
