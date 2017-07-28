#ifndef USER_H
#define USER_H

#include "ShareFolder.h"
#include "ISynchronize.h"
#include <chrono>
#include <set>

using namespace std::chrono;

class Server;

class User : public Entity, ISynchronize
{
public:
    User() = default;
    User(unsigned int _id, string _name);
    User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized, map<unsigned int, ShareFolder> _groups, set<unsigned int> _pendingInvitations);
    ~User() = default;

private:
    string name;
    bool isConnected = false;
    system_clock::time_point lastSynchronized;

    map<unsigned int, ShareFolder> groups;   //int = groupId
    set<unsigned int> pendingInvitations; //int = groupId

public:
    bool operator == (const User &u) {
        return id == u.id;
    }

    void createGroup(string groupName, string description);
    void applyGroup(unsigned int idGroup);
    void addPendingInvitation(unsigned int groupId);
    bool acceptInvitation(unsigned int groupId);
    bool denyInvitation(unsigned int groupId);
    void synchronize() override;
    bool doesPendingInvitationExists(unsigned int groupId);
    set<unsigned int> getGroups();
    bool getIsConnected();
    void setIsConnected(bool isConnected);
    string getName();
    void serialize(PrettyWriter<StringBuffer>& writer) const;
};

#endif // !USER_H