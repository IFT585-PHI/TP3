#ifndef ADMIN_H
#define ADMIN_H

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <set>

using namespace rapidjson;
using namespace std;

class Server;

class Admin
{
public:
    Admin() = default;
    Admin(unsigned int _userId, unsigned int _groupId);
    Admin(unsigned int _userId, unsigned int _groupId, set<unsigned int> _pendingInvitations);
    ~Admin() = default;

private:
    unsigned int userId;
    unsigned int groupId;
    set<unsigned int> pendingInvitations; // int = userId

public:
    void Invite(unsigned int _userId);
    void Remove(unsigned int _userId);
    void Delegate(unsigned int _userId);
    void Accept(unsigned int _userId);
    void Deny(unsigned int _userId);
    void SetUserId(unsigned int _userId);
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !ADMIN_H