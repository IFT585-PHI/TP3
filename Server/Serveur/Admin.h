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
    ~Admin() = default;

private:
    unsigned int userId;
    unsigned int groupId;

public:
    void SetUserId(unsigned int _userId);
    unsigned int getUserId();
};

#endif // !ADMIN_H