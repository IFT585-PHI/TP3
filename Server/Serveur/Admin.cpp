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
    UserManager::getInstance()->sendInvitation(groupId, _userId);
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