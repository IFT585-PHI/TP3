#ifndef SERVER_H
#define SERVER_H

#include "File.h"
#include "ReaderFromClient.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
	static const string ROOT;

	Server(boost::asio::io_service* io_service);
    ~Server() = default;

	boost::asio::io_service* service;
	tcp::acceptor acceptor;
	tcp::socket socket;

	std::string readRequest(tcp::socket & socket);
	void sendResponse(tcp::socket & socket, const std::string& str);
	void run();
	static string LookUpClientFunction(string json);
};



#endif // !SERVER_H
