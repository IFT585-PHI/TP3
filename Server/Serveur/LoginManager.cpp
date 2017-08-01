#include "LoginManager.h"
#include "UserManager.h"

LoginManager* LoginManager::lm = nullptr;

LoginManager::LoginManager() {
    userAuthentification = map<string, string>();
    connectedUsers = vector<User>();
}

LoginManager* LoginManager::getInstance() {
	if (LoginManager::lm == nullptr)
		LoginManager::lm = new LoginManager();

	return LoginManager::lm;
}

bool LoginManager::validateUserLogin(string userName, string password) {
    for (auto entry : userAuthentification) {
        if (entry.first == userName && entry.second == password)
        {
            return true;
        }
    }

    return false;
}

vector<User> LoginManager::getConnectedUsers() {
    return connectedUsers;
}

void LoginManager::addUserAuthentification(string userName, string password) {
    userAuthentification.insert(make_pair(userName, password));

	UserManager::getInstance()->addNewUser(userName);
}

bool LoginManager::isUserConnected(unsigned int userId) {
	for (auto entry : connectedUsers) {
		if (entry.getId() == userId)
		{
			return true;
		}
	}
	return false;
}

bool LoginManager::doesUsernameExists(string userName) {
    for (auto entry : userAuthentification) {
        if (entry.first == userName)
        {
            return true;
        }
    }
    return false;
}

bool LoginManager::addConnectedUser(User newUser) {
    for (auto user : connectedUsers) {
        if (newUser == user) {
            return false;
        }
    }

    connectedUsers.push_back(newUser);
    return true;
}

bool LoginManager::removeConnectedUser(User user) {
    for (auto u : connectedUsers) {
        if (user == u) {
            std::vector<User>::iterator it = find(connectedUsers.begin(), connectedUsers.end(), u);
			if (it != connectedUsers.end()) {
                connectedUsers.erase(it);
			}
            return true;
        }
    }
    return false;
}