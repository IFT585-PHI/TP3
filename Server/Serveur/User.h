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
    ~User() = default;

private:
    string name;

public:
    bool operator == (const User &u) {
        return id == u.id;
    }

    string getName();
};

#endif // !USER_H