#include "User.h"
#include "Server.h"
#include "GroupManager.h"
#include <ctime>

User::User(unsigned int _id, string _name)
    : Entity(_id), name{ _name }, root{}
{
    lastSynchronized = system_clock::now();
    groups = map<unsigned int, ShareFolder>();
}

User::User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized, map<unsigned int, ShareFolder> _groups)
    : Entity(_id), name{ _name }, root{}, lastSynchronized{ _lastSynchronized }, groups{ _groups }
{
}

void User::synchronize() {
    // do something


    lastSynchronized = system_clock::now();
}

set<unsigned int> User::getGroups() {
    set<unsigned int> groupsList{};

    for (auto group : groups) {
        groupsList.insert(group.first);
    }

    return groupsList;
}

string User::getName() {
    return name;
}

string User::getRoot() {
    return root;
}

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
