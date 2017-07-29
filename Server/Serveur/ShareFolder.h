#ifndef SHAREFOLDER_H
#define SHAREFOLDER_H

#include "Entity.h"
#include "File.h"
#include <map>

class ShareFolder : public Entity
{
public:
    ShareFolder() = default;
    ShareFolder(unsigned int _id, string _root);
    ShareFolder(unsigned int _id, string _root, map<File, unsigned int> _files);
    ~ShareFolder() = default;

private:
    string root;
    map<File, unsigned int> files; //int = version
    
public:
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !SHAREFOLDER_H
