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

bool GroupManager::removeUserPending(unsigned int groupId, unsigned int userId) {
	if (!doesGroupExists(groupId) && !doesUserPendingExists(groupId, userId))
		return false;

	getGroupById(groupId).removePendingInvitation(userId);
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

bool GroupManager::doesUserPendingExists(unsigned int groupId, unsigned int userId) {
	if (!doesGroupExists(groupId))
		throw exception{ "No group exist for this id" };

	for (int id : getGroupById(groupId).pendingInvitations)
	{
		if (id == userId)
			return true;
	}

	return false;
}