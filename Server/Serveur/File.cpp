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

/*template <typename Writer>*/
void File::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    Entity::serialize(writer);

    writer.String("File name");
    writer.String(fileName.getFileName().c_str(), static_cast<SizeType>(fileName.getFileName().length()));
    writer.String("Extension");
    writer.String(fileName.extension.c_str(), static_cast<SizeType>(fileName.extension.length()));
    writer.String("Path");
    writer.String(path.c_str(), static_cast<SizeType>(path.length()));
    writer.String("Version");
    writer.Uint(version);

    writer.EndObject();
}