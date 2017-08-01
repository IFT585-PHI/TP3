#include "Admin.h"
#include "Server.h"
#include "GroupManager.h"
#include "UserManager.h"

Admin::Admin(unsigned int _userId, unsigned int _groupId)
    : userId{ _userId }, groupId{ _groupId }
{
}

void Admin::SetUserId(unsigned int _userId) {
    userId = _userId;
}

unsigned int Admin::getUserId() {
    return userId;
}