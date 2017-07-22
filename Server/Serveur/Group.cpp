#include "Group.h"

Group::Group(unsigned int _id, string _name, string _description, unsigned int _userId)
    : Entity(_id), name{ _name }, description{ _description }
{
    admin = Admin(_id, _userId);
    members = set<unsigned int>();
    pendingInvitations = set<unsigned int>();
    filesVersion = map<unsigned int, unsigned int>();
}

Group::Group(unsigned int _id, string _name, string _description, Admin _admin, set<unsigned int> _members, set<unsigned int> _pendingInvitations, map<unsigned int, unsigned int> _filesVersion)
    : Entity(_id), name{ _name }, description{ _description }, admin{ _admin }, members{ _members }, pendingInvitations{ _pendingInvitations }, filesVersion{ _filesVersion }
{
}

bool Group::addMember(unsigned int userId) {
    if (doesMemberExists(userId) || !doesPendingInvitationExists(userId))
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

bool Group::addFile(unsigned int fileId) {
    if (doesFileExists(fileId))
        return false;

    filesVersion.insert(make_pair(fileId, 1));
    return true;
}

bool Group::updateFile(unsigned int fileId) {
    if (!doesFileExists(fileId))
        return false;

    //do something with file content?
    filesVersion[fileId] += 1;
    return true;
}

bool Group::removeFile(unsigned int fileId) {
    if (!doesFileExists(fileId))
        return false;

    filesVersion.erase(fileId);
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
    return filesVersion.count(fileId);
}

bool Group::doesPendingInvitationExists(unsigned int userId) {
    return pendingInvitations.count(userId);
}

void Group::setAdmin(unsigned int userId) {
    admin.SetUserId(userId);
}