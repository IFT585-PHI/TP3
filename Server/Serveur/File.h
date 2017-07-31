#ifndef FILE_H
#define FILE_H

#include "Entity.h"

class File : public Entity
{
public:
    File() = default;
    File(string _name, string _path, unsigned int _id);
    ~File() = default;

    bool operator==(const File &f) {
        return id == f.id;
    }
    
    bool operator<(const File &f) const {
        return (id < f.id);
    }

private:
    string fileName;
    string path;
    unsigned int version = 1;

public:
    unsigned int getVersion();
    string getName();
    void setName(string newName);
    //void serialize(PrettyWriter<StringBuffer>& writer) const;
	void incVersion();
};

#endif // !FILE_H
