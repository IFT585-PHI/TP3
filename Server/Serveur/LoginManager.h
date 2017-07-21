#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "User.h"
#include <vector>

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
    void initialize();
    bool validateUserLogin(string userName, string password);
    vector<User> getConnectedUsers();
    vector<User> getNotConnectedUsers();
    void addUser(string userName, string password);
    void synchronize();
};

#endif // !LOGINMANAGER_H