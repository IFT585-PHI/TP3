#include "User.h"
#include "Server.h"
#include "GroupManager.h"
#include <ctime>

User::User(unsigned int _id, string _name)
    : Entity(_id), name{ _name }
{
}

string User::getName() {
    return name;
}