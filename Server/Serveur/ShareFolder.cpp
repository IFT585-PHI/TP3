#include "ShareFolder.h"

ShareFolder::ShareFolder(unsigned int _id, string _root)
    : Entity(_id), root{ _root }
{
    files = map<File, unsigned int>();
}

ShareFolder::ShareFolder(unsigned int _id, string _root, map<File, unsigned int> _files)
    : Entity(_id), root{ _root }, files{ _files }
{
}