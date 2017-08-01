#include "File.h"

File::File(string _name, unsigned int _id)
    : Entity(_id)
{
    fileName = _name;
}

unsigned int File::getVersion() {
    return version;
}

void File::incVersion() {
	++version;
}

string File::getName() {
    return fileName;
}

void File::setName(string newName) {
    fileName = newName;
}