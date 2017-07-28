
#include "GroupManager.h"
#include "LoginManager.h"
#include "UserManager.h"
#include "Server.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
string Server::address = "127.0.0.1";

Server::Server(boost::asio::io_service* io_service)
	:acceptor(*io_service, tcp::endpoint(tcp::v4(), 13)), socket(*io_service),
	 service(io_service)
{
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

void Server::run() {
	while (true) {
		tcp::socket socket(*service);
		acceptor.accept(socket);

		std::string json = readRequest(socket);
		json.pop_back();
		//JSON PARSE ^

		//TEST
		ClientFunction cf = ClientFunction::LogIn;

		string result = LookUpClientFunction(cf, json);

		std::cout << "Resquest received: " << std::endl;
		std::cout << json << std::endl;
		//sendResponse(socket, json);

		sendResponse(socket, result);
		std::cout << "Response Sent." << std::endl;
	}
}

string Server::LookUpClientFunction(ClientFunction cf, string json) {
	string result;

	switch (cf) 
	{
		case ClientFunction::LogIn:
		{
			int userId = LoginManager::getInstance()->validateUserLogin("", "");

			if (userId == -1) {
				//result = std::to_string(userId);
				result = std::to_string(1);
				//				JSON PARSE ^
				sendResponse(socket, result);
			}
			else {
				//ERROR
			}

			break;
		}
		default:
			break;
	}

	return result;
}

std::string Server::readRequest(tcp::socket & socket) {
	boost::asio::streambuf buf;
	// WILL STOP READING WHEN "." IS FOUND
	boost::asio::read_until(socket, buf, ".");

	std::string data = boost::asio::buffer_cast<const char*>(buf.data());
	data.erase(--data.end());
	return data;
}

void Server::sendResponse(tcp::socket & socket, const std::string& str) {
	const std::string msg = str + "\n";
	boost::asio::write(socket, boost::asio::buffer(msg));
}