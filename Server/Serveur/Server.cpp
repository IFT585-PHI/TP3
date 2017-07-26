#include "GroupManager.h"
#include "LoginManager.h"
#include "UserManager.h"
#include "Server.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
string Server::address = "127.0.0.1";
Server::Server(boost::asio::io_service& io_service):acceptor(io_service, tcp::endpoint(tcp::v4(), 13)), socket(io_service) {
}

void Server::initializeManager() {
	GroupManager::getInstance()->initialize();
	LoginManager::getInstance()->initialize();
	UserManager::getInstance()->initialize();
}

void Server::receiveFiles() {

}

void Server::sendFile(File file) {
    
}

void Server::synchronize() {
	UserManager::getInstance()->synchronize();
	LoginManager::getInstance()->synchronize();
    //...
}

void Server::start_accept() {
	acceptor.accept(socket);
	while (true) {
		std::string str = readRequest(socket);
		std::cout << str << std::endl;

		sendResponse(socket, "Test R�ussi");
		std::cout << "sent Test R�ussi" << std::endl;
	}
}

std::string Server::readRequest(tcp::socket & socket) {
	boost::asio::streambuf buf;
	boost::asio::read_until(socket, buf, "\n");

	std::string data = boost::asio::buffer_cast<const char*>(buf.data());
	data.erase(--data.end());
	return data;
}

void Server::sendResponse(tcp::socket & socket, const std::string& str) {
	const std::string msg = str + "\n";
	boost::asio::write(socket, boost::asio::buffer(msg));
}