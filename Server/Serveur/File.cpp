#include "File.h"

File::File(string _name, string _extension, string _path, unsigned int _id)
    : Entity(_id), path{_path}
{
    fileName.name = _name;
    fileName.extension = _extension;
}

unsigned int File::getVersion() {
    return version;
}

void File::setVersion(unsigned int _version) {
    version = _version;
}
