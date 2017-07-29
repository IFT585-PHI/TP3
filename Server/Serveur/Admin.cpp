#include "Admin.h"
#include "Server.h"
#include "GroupManager.h"
#include "UserManager.h"

Admin::Admin(unsigned int _userId, unsigned int _groupId)
    : userId{ _userId }, groupId{ _groupId }
{
    pendingInvitations = set<unsigned int>();
}

Admin::Admin(unsigned int _userId, unsigned int _groupId, set<unsigned int> _pendingInvitations)
    : userId{ _userId }, groupId{ _groupId }, pendingInvitations{ _pendingInvitations }
{
}

void Admin::Invite(unsigned int _userId) {
    GroupManager::getInstance()->addUserToGroup(groupId, _userId);
}

void Admin::Remove(unsigned int _userId) {
	GroupManager::getInstance()->removeUserFromGroup(groupId, _userId);
}

void Admin::Delegate(unsigned int _userId) {
	GroupManager::getInstance()->setNewAdmin(groupId, _userId);
}

void Admin::Accept(unsigned int _userId) {
	GroupManager::getInstance()->getGroupById(groupId).addMember(_userId);
}

void Admin::Deny(unsigned int _userId) {
	GroupManager::getInstance()->getGroupById(groupId).removePendingInvitation(_userId);
}

void Admin::SetUserId(unsigned int _userId) {
    userId = _userId;
}

unsigned int Admin::getUserId() {
    return userId;
}

void Admin::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    writer.String("UserId");
    writer.Uint(userId);
    writer.String("GroupId");
    writer.Uint(groupId);

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