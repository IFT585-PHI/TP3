#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>
#include <map>

static class UserManager
{
public:
	static UserManager* getInstance();
    ~UserManager() = default;

private:
	UserManager();
	static UserManager* um;

    map<unsigned int, User> existingUsers; // int = userId

public:
    void initialize();
    bool addNewUser(string userName);
    vector<User> getListExistingUsers();
    User getUserByName(string userName);
    User getUserById(unsigned int userId);
    bool doesUserExists(unsigned int userId);
};

#endif // !USERMANAGER_H