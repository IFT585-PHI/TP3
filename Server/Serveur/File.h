#ifndef FILE_H
#define FILE_H

#include "Entity.h"

struct FileName {
    string name;
    string extension;

    string getFileName() {
        return name + "." + extension;
    }
};

class File : Entity
{
public:
    File() = default;
    File(string _name, string _extension, string _path, unsigned int _id);
    ~File() = default;

private:
    FileName fileName;
    string path;
    unsigned int version = 1;

public:
    unsigned int getVersion();
    void setVersion(unsigned int _version);
};

#endif // !FILE_H
