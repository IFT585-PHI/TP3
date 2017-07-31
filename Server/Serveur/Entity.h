#ifndef ENTITY_H
#define ENTITY_H

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <cstdio>
#include <string>
#include <iostream>

using namespace std;
using namespace rapidjson;

class Entity
{
public:
    Entity() = default;
    ~Entity() = default;
    unsigned int getId();

protected:
    unsigned int id;
    Entity(unsigned int _id);
};

#endif // !ENTITY_H