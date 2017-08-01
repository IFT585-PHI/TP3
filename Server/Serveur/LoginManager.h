#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "User.h"
#include <vector>
#include <map>

class Server;

static class LoginManager
{
public:
	static LoginManager* getInstance();
    ~LoginManager() = default;

private:
	LoginManager();
	static LoginManager* lm;

    map<string, string> userAuthentification;
    vector<User> connectedUsers;

public:
    bool validateUserLogin(string userName, string password);
   
    bool addConnectedUser(User user);
    bool removeConnectedUser(User user);

    vector<User> getConnectedUsers();
    void addUserAuthentification(string userName, string password);

	bool isUserConnected(unsigned int userId);
	bool doesUsernameExists(string username);
};

#endif // !LOGINMANAGER_H