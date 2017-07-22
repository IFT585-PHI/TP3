#include "GroupManager.h"
#include "LoginManager.h"
#include "UserManager.h"
#include "Server.h"

string Server::address = "127.0.0.1";

Server::Server() {
    
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