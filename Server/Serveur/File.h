#ifndef FILE_H
#define FILE_H

#include "Entity.h"

struct FileName {
    string name;
    string extension;

    string getFileName() {
        return name + "." + extension;
    }

    string getFileName() const{
        return name + "." + extension;
    }
};

class File : public Entity
{
public:
    File() = default;
    File(string _name, string _extension, string _path, unsigned int _id);
    ~File() = default;

    bool operator==(const File &f) {
        return id == f.id;
    }
    
    bool operator<(const File &f) const {
        return (id < f.id);
    }

private:
    FileName fileName;
    string path;
    unsigned int version = 1;

public:
    unsigned int getVersion();
    void setVersion(unsigned int _version);
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !FILE_H
