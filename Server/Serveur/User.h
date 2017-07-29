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
    User(unsigned int _id, string _name, system_clock::time_point _lastSynchronized, map<unsigned int, ShareFolder> _groups);
    ~User() = default;

private:
    string name;
    string root;
    system_clock::time_point lastSynchronized;
	bool isConnected;

    map<unsigned int, ShareFolder> groups;   //int = groupId

public:
    bool operator == (const User &u) {
        return id == u.id;
    }

    void synchronize() override;
    set<unsigned int> getGroups();
    string getRoot();
    string getName();
    void serialize(PrettyWriter<StringBuffer>& writer) const;

	void setIsConnected(bool val);
	bool getIsConnected();
};

#endif // !USER_H