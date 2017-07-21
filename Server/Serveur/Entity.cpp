#include "Entity.h"

Entity::Entity(unsigned int _id) {
    id = _id;
}

unsigned int Entity::getId(){
    return id;
}