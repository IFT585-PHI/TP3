#include "Group.h"
#include "Server.h"
#include "UserManager.h"
#include <boost/filesystem/operations.hpp>

Group::Group(unsigned int _id, string _name, string _description, unsigned int _userId)
    : Entity(_id), name{ _name }, description{ _description }
{
    admin = Admin(_userId, _id);
    members = set<unsigned int>();
	members.insert(_userId);
	members.insert(UserManager::getInstance()->getUserByName("TEST").getId());
    pendingInvitations = set<unsigned int>();
    files = map<unsigned int, File*>();

	if (boost::filesystem::exists(Server::root))
		boost::filesystem::create_directory(Server::root);
}

Group::Group(unsigned int _id, string _name, string _description, Admin _admin, set<unsigned int> _members, set<unsigned int> _pendingInvitations, map<unsigned int, File*> _files)
    : Entity(_id), name{ _name }, description{ _description }, admin{ _admin }, members{ _members }, pendingInvitations{ _pendingInvitations }, files{ _files }
{
}

bool Group::addMember(unsigned int userId) {
    if (doesMemberExists(userId))
        return false;

    members.insert(userId);
	removePendingInvitation(userId);
    return true;
}

bool Group::removeMember(unsigned int userId) {
    if (!doesMemberExists(userId))
        return false;

    members.erase(userId);
    return true;
}

bool Group::addFile(File* file) {
    if (doesFileExists(file->getId()))
        return false;

    files.insert(make_pair(file->getId(), file));
    return true;
}

bool Group::updateFile(File* file) {
	if (!doesFileExists(file->getId()))
        return false;

	files[file->getId()] = file;
	files[file->getId()]->incVersion();
    return true;
}

bool Group::removeFile(File* file) {
	if (!doesFileExists(file->getId()))
        return false;

    files.erase(file->getId());
    return true;
}

bool Group::renameFile(File * file, string newName) {
    if (!doesFileExists(file->getId()))
        return false;

    file->setName(newName);
    return true;
}

bool Group::addPendingInvitation(unsigned int userId) {
    if (doesPendingInvitationExists(userId))
        return false;

    pendingInvitations.insert(userId);
    return true;
}

bool Group::removePendingInvitation(unsigned int userId) {
    if (!doesPendingInvitationExists(userId))
        return false;

    pendingInvitations.erase(userId);
    return true;
}

bool Group::doesMemberExists(unsigned int userId) {
    return members.count(userId);
}

bool Group::doesFileExists(unsigned int fileId) {
    return files.count(fileId);
}

bool Group::doesPendingInvitationExists(unsigned int userId) {
    return pendingInvitations.count(userId);
}

void Group::setAdmin(unsigned int userId) {
    admin.SetUserId(userId);
}

string Group::getName() {
	return name;
}

File* Group::getFileFromName(string fileName) {
    for (auto file : files) {
        if (file.second->getName() == fileName)
            return file.second;
    }

    return nullptr;
}



/*
void Group::serialize(PrettyWriter<StringBuffer>& writer) const {
    writer.StartObject();

    Entity::serialize(writer);

    writer.String("Name");
    writer.String(name.c_str(), static_cast<SizeType>(name.length()));
    writer.String("Description");
    writer.String(description.c_str(), static_cast<SizeType>(description.length()));
    
    writer.String("Admin");
    admin.serialize(writer);

    writer.String("Members");
    writer.StartArray();
    if (!members.empty()) {
        for (auto member : members)
            writer.Uint(member);
    }
    else
        writer.Null();

    writer.EndArray();

    writer.String("PerndingInvitations");
    writer.StartArray();
    if (!pendingInvitations.empty()) {
        for (auto pending : pendingInvitations)
            writer.Uint(pending);
    }
    else
        writer.Null();

    writer.EndArray();

    writer.String("FilesVersion");
    writer.StartArray();
    if (!filesVersion.empty()) {
        for (auto file : filesVersion) {
            writer.StartObject();
            writer.String("FileId");
            writer.Uint(file.first);
            writer.String("FileVersion");
            writer.Uint(file.second);
            writer.EndObject();
        }
    }
    else
        writer.Null();

    writer.EndArray();

    writer.EndObject();
}*/