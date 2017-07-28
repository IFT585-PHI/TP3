#include "User.h"
#include "Server.h"
#include "GroupManager.h"
#include <ctime>

User::User(unsigned int _id, string _name)
    : Entity(_id), name{ _name }
{
    lastSynchronized = system_clock::now();
    groups = map<unsigned int, ShareFolder>();
    pendingInvitations = set<unsigned int>();
}

User::User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized, map<unsigned int, ShareFolder> _groups, set<unsigned int> _pendingInvitations)
    : Entity(_id), name{ _name }, lastSynchronized{ _lastSynchronized }, groups{ _groups }, pendingInvitations{ _pendingInvitations }
{
}

void User::createGroup(string groupName, string description) {
	GroupManager::getInstance()->addGroup(groups.size(), Group(groups.size(), groupName, description, id));
}

void User::applyGroup(unsigned int groupId) {
	GroupManager::getInstance()->getGroupById(groupId).addPendingInvitation(id);
}

void User::addPendingInvitation(unsigned int groupId){
    pendingInvitations.insert(groupId);
}

bool User::acceptInvitation(unsigned int groupId) {
    if (!doesPendingInvitationExists(groupId))
        return false;

	GroupManager::getInstance()->addUserToGroup(groupId, id);
    pendingInvitations.erase(groupId);
    return true;
}

bool User::denyInvitation(unsigned int groupId) {
    if (!doesPendingInvitationExists(groupId))
        return false;

	GroupManager::getInstance()->removeUserFromGroup(groupId, id);
    pendingInvitations.erase(groupId);
    return true;
}

void User::synchronize() {
    // do something


    lastSynchronized = system_clock::now();
}

bool User::doesPendingInvitationExists(unsigned int groupId) {
    return pendingInvitations.count(groupId);
}

set<unsigned int> User::getGroups() {
    set<unsigned int> groupsList{};

    for (auto group : groups) {
        groupsList.insert(group.first);
    }

    return groupsList;
}

bool User::getIsConnected() {
    return isConnected;
}

void User::setIsConnected(bool _isConnected) {
    isConnected = _isConnected;
}

string User::getName() {
    return name;
}

void User::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    writer.String("UserName");
    writer.String(name.c_str(), static_cast<SizeType>(name.length()));
    writer.String("isConnected");
    writer.Bool(isConnected);
    writer.String("LastSynchronized");
    
    std::time_t lastSynchronized_c = std::chrono::system_clock::to_time_t(lastSynchronized);
    std::string lastSynchronized_str = ctime(&lastSynchronized_c);

    writer.String(lastSynchronized_str.c_str(), static_cast<SizeType>(lastSynchronized_str.length()));

    writer.String("Groups");
    writer.StartArray();
    if (!groups.empty()) {
        for (auto group : groups) {
            writer.StartObject();

            writer.String("GroupId");
            writer.Uint(group.first);

            writer.String("ShareFolder");
            group.second.serialize(writer);

            writer.EndObject();
        }
    }
    else
        writer.Null();

    writer.EndArray();

    writer.String("PerndingInvitations");
    writer.StartArray();
    if (!pendingInvitations.empty()) {
        for (std::set<unsigned int>::iterator pendingItr = pendingInvitations.begin(); pendingItr != pendingInvitations.end(); ++pendingItr)
            writer.Uint(*pendingItr);
    }
    else
        writer.Null();

    writer.EndArray();

    writer.EndObject();
}
