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

	if (boost::filesystem::exists(Server::ROOT))
		boost::filesystem::create_directory(Server::ROOT);
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
	file->incVersion();
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

unsigned int Group::createNewFileId() {
	return files.size();
}