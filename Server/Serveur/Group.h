#ifndef GROUP_H
#define GROUP_H

#include "Entity.h"
#include "Admin.h"
#include "File.h"
#include <set>
#include <map>
#include <vector>

class Group : public Entity
{
public:
    Group() = default;
    Group(unsigned int _id, string _name, string _description, unsigned int _userId);
    ~Group() = default;
 
    string name;
    string description;
    Admin admin;
    set<unsigned int> members; //int = userId
    set<unsigned int> pendingInvitations; //int = userId
    map<unsigned int, File*> files; //int = idFile

public:
	bool operator ==(const Group &g) {
		return id == g.id;
	}

    bool addMember(unsigned int userId);
    bool removeMember(unsigned int userId);

    bool addFile(File* file);
    bool updateFile(File* file);
    bool removeFile(File* file);
    bool renameFile(File* file, string newName);

    File* getFileFromName(string fileName);

    bool addPendingInvitation(unsigned int userId);
    bool removePendingInvitation(unsigned int userId);
	vector<File> getFiles();

    bool doesMemberExists(unsigned int userId);
    bool doesFileExists(unsigned int fileId);
    bool doesPendingInvitationExists(unsigned int userId);
    void setAdmin(unsigned int userId);

	unsigned int createNewFileId();

	string getName();
};

#endif // !GROUP_H
