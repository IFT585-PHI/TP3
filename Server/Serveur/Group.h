#ifndef GROUP_H
#define GROUP_H

#include "Entity.h"
#include "Admin.h"
#include <set>
#include <map>

class Group : public Entity
{
public:
    Group() = default;
    Group(unsigned int _id, string _name, string _description, unsigned int _userId);
    Group(unsigned int _id, string _name, string _description, Admin _admin, set<unsigned int> _members, set<unsigned int> _pendingInvitations, map<unsigned int, unsigned int> _filesVersion);
    ~Group() = default;
 
    string name;
    string description;
    Admin admin;
    set<unsigned int> members; //int = userId
    set<unsigned int> pendingInvitations; //int = userId
    map<unsigned int, unsigned int> filesVersion; //int = idFile, int = latestVersion

public:
	bool operator ==(const Group &g) {
		return id == g.id;
	}

    bool addMember(unsigned int userId);
    bool removeMember(unsigned int userId);

    bool addFile(unsigned int fileId);
    bool updateFile(unsigned int fileId);
    bool removeFile(unsigned int fileId);

    bool addPendingInvitation(unsigned int userId);
    bool removePendingInvitation(unsigned int userId);

    bool doesMemberExists(unsigned int userId);
    bool doesFileExists(unsigned int fileId);
    bool doesPendingInvitationExists(unsigned int userId);
    void setAdmin(unsigned int userId);
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !GROUP_H
