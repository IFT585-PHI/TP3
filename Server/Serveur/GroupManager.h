#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include "Group.h"

static class GroupManager
{
public:		
    ~GroupManager() = default;
	static GroupManager* getInstance();

private:
	GroupManager();
	static GroupManager* gm;

    map<unsigned int, Group> groups; // int = groupId

public:
    void initialize();
    bool addGroup(unsigned int groupId, Group group);

    bool addUserToGroup(unsigned int groupId, unsigned int userId);
    bool removeUserFromGroup(unsigned int groupId, unsigned int userId);

	bool removeUserPending(unsigned int groupId, unsigned int userId);

    bool setNewAdmin(unsigned int groupId, unsigned int userId);
    Group getGroupById(unsigned int groupId);

    bool doesGroupExists(unsigned int groupId);
	bool doesUserPendingExists(unsigned int groupId, unsigned int userId);
};

#endif // !GROUPMANAGER_H
