#include "Entity.h"

Entity::Entity(unsigned int _id) {
    id = _id;
}

unsigned int Entity::getId(){
    return id;
}

void Entity::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.String("id");
    writer.Uint(id);
}