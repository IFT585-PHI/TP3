#include "UserManager.h"

UserManager* UserManager::um = nullptr;

UserManager::UserManager()
{
    existingUsers = map<unsigned int, User>();
}

UserManager* UserManager::getInstance() {
	if (UserManager::um == nullptr)
		UserManager::um = new UserManager();

	return UserManager::um;
}

bool UserManager::addNewUser(string userName) {
    for(auto u : getListExistingUsers())
    {
        if (u.getName() == userName)
            return false;
    }

    existingUsers.insert(make_pair(existingUsers.size(), User{ existingUsers.size(), userName }));
    return true;
}

vector<User> UserManager::getListExistingUsers() {
    vector<User> users;

    for (auto u : existingUsers) {
        users.push_back(u.second);
    }

    return users;
}

User UserManager::getUserByName(string userName) {
    User user;

    for(auto u : getListExistingUsers())
    {
        if (u.getName() == userName)
        {
            user = u;
            break;
        }
    }

    return user;
}

User UserManager::getUserById(unsigned int userId) {
    if (!doesUserExists(userId))
        throw exception{"No user exist for this id"};

    return existingUsers[userId];
}

bool UserManager::doesUserExists(unsigned int userId) {
    return existingUsers.count(userId);
}