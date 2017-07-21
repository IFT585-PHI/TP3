#ifndef SHAREFOLDER_H
#define SHAREFOLDER_H

#include "Entity.h"
#include "File.h"
#include <map>

class ShareFolder : Entity
{
public:
    ShareFolder() = default;
    ShareFolder(unsigned int _id, string _root);
    ShareFolder(unsigned int _id, string _root, map<File, unsigned int> _files);
    ~ShareFolder() = default;

private:
    string root;
    map<File, unsigned int> files; // int = version
};

#endif // !SHAREFOLDER_H
