#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>

using namespace std;

class Entity
{
public:
    Entity() = default;
    ~Entity() = default;

private:

protected:
    unsigned int id;
    Entity(unsigned int _id);
    unsigned int getId();
};

#endif // !ENTITY_H