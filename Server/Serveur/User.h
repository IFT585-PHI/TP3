#ifndef USER_H
#define USER_H

#include <chrono>
#include <set>

#include "Entity.h"

using namespace std::chrono;

class User : public Entity
{
public:
    User() = default;
    User(unsigned int _id, string _name);
    User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized);
    ~User() = default;

private:
    string name;
    system_clock::time_point lastSynchronized;

public:
    bool operator == (const User &u) {
        return id == u.id;
    }

    string getName();
   // void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !USER_H