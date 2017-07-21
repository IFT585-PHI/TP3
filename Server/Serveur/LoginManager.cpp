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

void LoginManager::initialize() {
    // Build userAuthentification from file parser
}

bool LoginManager::validateUserLogin(string userName, string password) {
    for (auto entry : userAuthentification) {
        if (entry.first == userName && entry.second == password)
        {
            User user = UserManager::getInstance()->getUserByName(userName);
            user.setIsConnected(true);
            connectedUsers.push_back(user);
            return true;
        }
    }

    return false;
}

vector<User> LoginManager::getConnectedUsers() {
    return connectedUsers;
}

vector<User> LoginManager::getNotConnectedUsers() {
	vector<User> notConnectedUsers = UserManager::getInstance()->getListExistingUsers();

    for(auto u : connectedUsers)
    {
        std::vector<User>::iterator it = find(notConnectedUsers.begin(), notConnectedUsers.end(), u);
        if(it != notConnectedUsers.end())
            notConnectedUsers.erase(it);
    }

    return notConnectedUsers;
}

void LoginManager::addUser(string userName, string password) {
    userAuthentification.insert(make_pair(userName, password));

	UserManager::getInstance()->addNewUser(userName);
}

void LoginManager::synchronize() {
	vector<User> existingUsers = UserManager::getInstance()->getListExistingUsers();

    for(auto u : connectedUsers)
    {
        if (!u.getIsConnected())
        {
			u.setIsConnected(false);
            std::vector<User>::iterator it = find(connectedUsers.begin(), connectedUsers.end(), u);
            if (it != connectedUsers.end())
                connectedUsers.erase(it);
        }
    }
}