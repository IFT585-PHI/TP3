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

void ShareFolder::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    writer.String("Root");
    writer.String(root.c_str());

    writer.String("Files");
    writer.StartArray();
    if (!files.empty()) {
        for (auto file : files) {
            writer.StartObject();
            
            writer.String("File");
            file.first.serialize(writer);

            writer.String("Version");
            writer.Uint(file.second);

            writer.EndObject();
        }
    }
    else
        writer.Null();

    writer.EndArray();

    writer.EndObject();
}