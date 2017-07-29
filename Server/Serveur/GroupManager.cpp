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

bool GroupManager::addGroup(string name, string description, unsigned int adminId) {
	//Should look if the name is already taken, but im lazy as fu
	unsigned int groupId = createNewGroupId();
	groups.insert(make_pair(groupId, Group{groupId, name, description, adminId}));

	return true;
}

bool GroupManager::removeGroup(unsigned int groupId) {
	if (!doesGroupExists(groupId))
		return false;

	groups.erase(groupId);
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

int GroupManager::createNewGroupId() {
	return groups.size();
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

vector<Group> GroupManager::getAllGroupsForUser(int userId)
{
	vector<Group> result;
	for (auto entry : groups) {
		if(entry.second.doesMemberExists(userId))
			result.push_back(entry.second);
	}
	return result;
}

vector<Group> GroupManager::getAllPendingGroupsForUser(int userId)
{
	vector<Group> result;
	for (auto entry : groups) {
		if (entry.second.doesPendingInvitationExists(userId))
			result.push_back(entry.second);
	}
	return result;
}

vector<Group> GroupManager::getAllOutGroupForUser(int userId)
{
	vector<Group> result;
	vector<Group> all = getAllGroups();
	vector<Group> toRemove = getAllGroupsForUser(userId);

	for (auto g : getAllPendingGroupsForUser(userId)) {
		toRemove.push_back(g);
	}

	for (auto it = all.begin(); it != all.end(); ++it) {
		if (!(std::find(toRemove.begin(), toRemove.end(), *it) != toRemove.end()))
			result.push_back(*it);
	}

	return result;
}

vector<Group> GroupManager::getAllGroups()
{
	vector<Group> result;
	for (auto entry : groups) {
		result.push_back(entry.second);
	}
	return result;
}