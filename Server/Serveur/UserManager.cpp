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

void UserManager::initialize() {
    // Build existingUsers from file parser
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

bool UserManager::sendInvitation(unsigned int userId, unsigned int groupId) {
    if (!existingUsers.count(userId))
        return false;

    existingUsers[userId].addPendingInvitation(groupId);
    return true;
}

set<unsigned int> UserManager::getAllGroupIdFor(unsigned int id) {
    if (existingUsers.count(id))
        return existingUsers[id].getGroups();

    throw exception{ "No user exist for this id" };;
}

void UserManager::synchronize() {
    for (auto u : existingUsers) {
        u.second.synchronize();
    }
}

vector<User> UserManager::getListExistingUsers() {
    vector<User> users;

    for (auto u : existingUsers) {
        users.push_back(u.second);
    }

    return users;
}

map<unsigned int, User> UserManager::getExistingUsers() {
    return existingUsers;
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