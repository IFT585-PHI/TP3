#include "User.h"
#include "Server.h"
#include "GroupManager.h"
#include <ctime>

User::User(unsigned int _id, string _name)
    : Entity(_id), name{ _name }
{
    lastSynchronized = system_clock::now();
}

User::User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized)
    : Entity(_id), name{ _name }, lastSynchronized{ _lastSynchronized }
{
}

string User::getName() {
    return name;
}

/*
void User::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    writer.String("UserName");
    writer.String(name.c_str(), static_cast<SizeType>(name.length()));
    writer.String("Root");
    writer.String(root.c_str(), static_cast<SizeType>(root.length()));

    writer.String("LastSynchronized");
    
    std::time_t lastSynchronized_c = std::chrono::system_clock::to_time_t(lastSynchronized);
    std::string lastSynchronized_str = ctime(&lastSynchronized_c);

    writer.String(lastSynchronized_str.c_str(), static_cast<SizeType>(lastSynchronized_str.length()));

    writer.String("Groups");
    writer.StartArray();
    if (!groups.empty()) {
        for (auto group : groups) {
            writer.StartObject();

            writer.String("GroupId");
            writer.Uint(group.first);

            writer.String("ShareFolder");
            group.second.serialize(writer);

            writer.EndObject();
        }
    }
    else
        writer.Null();

    writer.EndArray();

    writer.EndObject();
}
*/