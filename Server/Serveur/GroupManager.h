#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include "UserManager.h"
#include "Group.h"
#include "vector"

#include <algorithm> 

static class GroupManager
{
public:		
    ~GroupManager() = default;
	static GroupManager* getInstance();

private:
	GroupManager();
	static GroupManager* gm;

    map<unsigned int, Group*> groups; // int = groupId

public:
    bool addGroup(string name, string description, unsigned int adminId);
	bool removeGroup(unsigned int groupId);

    bool addUserToGroup(unsigned int groupId, unsigned int userId);
    bool removeUserFromGroup(unsigned int groupId, unsigned int userId);

	bool removeUserPending(unsigned int groupId, unsigned int userId);

	unsigned int createNewGroupId();
    bool setNewAdmin(unsigned int groupId, unsigned int userId);
    Group *getGroupById(unsigned int groupId);

    bool doesGroupExists(unsigned int groupId);
	bool doesUserPendingExists(unsigned int groupId, unsigned int userId);
	bool doesGroupNameExists(string groupName);

	vector<Group> getAllGroups();
	vector<Group> getAllGroupsForUser(int userId);
	vector<Group> getAllPendingGroupsForUser(int userId);
	vector<Group> getAllOutGroupForUser(int userId);

};

#endif // !GROUPMANAGER_H
