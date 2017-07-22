#include "GroupManager.h"

GroupManager* GroupManager::gm = nullptr;

GroupManager::GroupManager() {
    groups = map<unsigned int, Group>();
}

GroupManager* GroupManager::getInstance() {
	if (GroupManager::gm == nullptr)
		GroupManager::gm = new GroupManager();

	return GroupManager::gm;
}

void GroupManager::initialize() {
    // Build groups from file parser
}

bool GroupManager::addGroup(unsigned int groupId, Group group) {
    if (groups.count(groupId))
        return false;

    groups.insert(make_pair(groupId, group));
    return true;
}

bool GroupManager::addUserToGroup(unsigned int groupId, unsigned int userId) {
    if (doesGroupExists(groupId))
        return false;

    groups[groupId].addMember(userId);
    return true;
}

bool GroupManager::removeUserFromGroup(unsigned int groupId, unsigned int userId) {
    if (!doesGroupExists(groupId))
        return false;

    groups[groupId].removeMember(userId);
    return true;
}

bool GroupManager::setNewAdmin(unsigned int groupId, unsigned int userId) {
    if (!doesGroupExists(groupId))
        return false;

    groups[groupId].setAdmin(userId);
    return true;
}

Group GroupManager::getGroupById(unsigned int groupId) {
    if (!doesGroupExists(groupId))
        throw exception{ "No group exist for this id" };

    return groups[groupId];
}

bool GroupManager::doesGroupExists(unsigned int groupId) {
    return groups.count(groupId);
}