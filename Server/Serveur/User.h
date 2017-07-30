#ifndef USER_H
#define USER_H

#include "ShareFolder.h"
#include <chrono>
#include <set>

using namespace std::chrono;

class Server;

class User : public Entity
{
public:
    User() = default;
    User(unsigned int _id, string _name);
    User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized, map<unsigned int, ShareFolder> _groups);
    ~User() = default;

private:
    string name;
    string root;
    system_clock::time_point lastSynchronized;

    map<unsigned int, ShareFolder> groups;   //int = groupId

public:
    bool operator == (const User &u) {
        return id == u.id;
    }

    void synchronize();
    set<unsigned int> getGroups();
    string getRoot();
    string getName();
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !USER_H