#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "User.h"
#include <vector>

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
    bool sendInvitation(unsigned int userId, unsigned int groupId);
    set<unsigned int> getAllGroupIdFor(unsigned int id);
    void synchronize();
    vector<User> getListExistingUsers();
    map<unsigned int, User> getExistingUsers();
    User getUserByName(string userName);
    User getUserById(unsigned int userId);
    bool doesUserExists(unsigned int userId);
};

#endif // !USERMANAGER_H