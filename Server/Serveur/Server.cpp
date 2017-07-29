
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



	//TEST
	LoginManager::getInstance()->addUser("TEST", "test");
	UserManager::getInstance()->addNewUser("TEST");
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
		//JSON PARSE ^

		std::cout << "Resquest received: " << std::endl;
		std::cout << json << std::endl;
		//sendResponse(socket, json);

		//TEST
		sendResponse(socket, LookUpClientFunction(json));

		std::cout << "Response Sent." << std::endl;
	}
}

string Server::LookUpClientFunction(string json) {
	string result;

    MessageMap messages = ReaderFromClient::getMessageMapFromJson(json);
    ClientFunction cf = ReaderFromClient::getFunctionId(messages);

	switch (cf) 
	{
		case ClientFunction::Register:
		{
            return ReaderFromClient::getRegisterResponse(messages);
            break;
		}
		case ClientFunction::LogIn:
		{
            return ReaderFromClient::getLogInResponse(messages);
            break;
		}
		case ClientFunction::LogOut:
		{
            return ReaderFromClient::getLogOutResponse(messages);
            break;
		}
		case ClientFunction::GetOnlineUsers:
		{
            return ReaderFromClient::getOnlineUsersResponse(messages);
            break;
		}
		case ClientFunction::GetGroupUsers:
		{
            return ReaderFromClient::getGroupUsersResponse(messages);
			break;
		}
		case ClientFunction::GetGroupPendingUsers:
		{
            return ReaderFromClient::getGroupPendingUsersResponse(messages);
			break;
		}
		case ClientFunction::GetGroups:
		{
            return ReaderFromClient::getGroupsResponse(messages);
			break;
		}
        case ClientFunction::CreateGroup:
        {
            return ReaderFromClient::getCreateGroupResponse(messages);
            break;
        }
		case ClientFunction::JoinGroup:
		{
            return ReaderFromClient::getJoinGroupResponse(messages);
			break;
		}
		case ClientFunction::LeaveGroup:
		{
            return ReaderFromClient::getLeaveGroupResponse(messages);
			break;
		}
		case ClientFunction::DeleteGroup:
		{
            return ReaderFromClient::getDeleteGroupResponse(messages);
			break;
		}
		case ClientFunction::KickUser:
		{
            return ReaderFromClient::getKickUserResponse(messages);
			break;
		}
		case ClientFunction::PromoteUser:
		{
            return ReaderFromClient::getPromoteUserResponse(messages);
			break;
		}
		case ClientFunction::InviteUser:
		{
            return ReaderFromClient::getInviteUserResponse(messages);
			break;
		}
		case ClientFunction::DeclineRequest:
		{
            return ReaderFromClient::getDeclineRequestResponse(messages);
			break;
		}
		case ClientFunction::ApproveRequest:
		{
            return ReaderFromClient::getApproveRequestResponse(messages);
			break;
		}
		case ClientFunction::GetFiles:
		{
			break;
		}
        case ClientFunction::Error:
        {
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