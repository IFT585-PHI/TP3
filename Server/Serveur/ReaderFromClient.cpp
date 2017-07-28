#include "UserManager.h"
#include "GroupManager.h"
#include "LoginManager.h"
#include "ReaderFromClient.h"

void ReaderFromClient::ParseMessages(const char* json, MessageMap& messages) {
    Reader reader;
    MessageHandler handler;
    StringStream ss(json);
    if (reader.Parse(ss, handler))
        messages.swap(handler.messages_);   // Only change it if success.
    else {
        ParseErrorCode e = reader.GetParseErrorCode();
        size_t o = reader.GetErrorOffset();
    }
}

MessageMap ReaderFromClient::getMessageMapFromJson(string json) {
    MessageMap messages;
    Reader reader;
    ParseMessages(json.c_str(), messages);

    return messages;
}

ClientFunction ReaderFromClient::getFunctionId(MessageMap messages) {
    MessageMap::iterator function = messages.find("function");

    if (function != messages.end()) {
        return getEnumFromText(function->second);
    }

    return ClientFunction::Error;
}

string ReaderFromClient::getRegisterResponse(MessageMap& messages) {
    UserManager* userMan = UserManager::getInstance();
    LoginManager* loginMan = LoginManager::getInstance();
    string errorMsg{};
    string status{};

    MessageMap::iterator username = messages.find("username");
    MessageMap::iterator password = messages.find("password");;

    if (!loginMan->doesUsernameExists(username->second)) {
        loginMan->addUser(username->second, password->second);
        userMan->addNewUser(username->second);
        status = SUCCESS;
    }
    else {
        errorMsg = "User already exists";
        status = "Failed";
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));
    
    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getLogInResponse(MessageMap messages) {
    LoginManager* loginMan = LoginManager::getInstance();
    UserManager* userMan = UserManager::getInstance();
    string errorMsg{};
    string status{};
    User user;

    MessageMap::iterator username = messages.find("username");
    MessageMap::iterator password = messages.find("password");;

    if (loginMan->validateUserLogin(username->second, password->second)) {
        user = userMan->getUserByName(username->second);

        if (loginMan->addConnectedUser(user)) {
            status = SUCCESS;
        }
        else {
            errorMsg = "User already logged in";
            status = FAILED;
        }
    }
    else {
        errorMsg = "Invalid user";
        status = FAILED;
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == SUCCESS) {
        writer.String("userId");
        writer.Uint(user.getId());
    }

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getLogOutResponse(MessageMap messages) {
    LoginManager* loginMan = LoginManager::getInstance();
    UserManager* userMan = UserManager::getInstance();
    string errorMsg{};
    string status{};
    User user;

    MessageMap::iterator userId = messages.find("userId");

    user = userMan->getUserById(atoi(userId->second.c_str()));

    if (loginMan->removeConnectedUser(user)) {
        status = SUCCESS;
    }
    else {
        errorMsg = "User failed to logout";
        status = FAILED;
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getOnlineUsersResponse(MessageMap messages) {
    LoginManager* loginMan = LoginManager::getInstance();
    UserManager* userMan = UserManager::getInstance();
    string errorMsg{};
    string status{};

    vector<User>connectedUsers = loginMan->getConnectedUsers();

    if (!connectedUsers.empty()) {
        status = SUCCESS;
    }
    else {
        errorMsg = "No user connected";
        status = FAILED;
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == SUCCESS) {
        writer.StartArray();
        for (auto user : connectedUsers) {
            writer.StartObject();

            writer.String("userId");
            writer.Uint(user.getId());
            
            writer.String("username");
            writer.String(user.getName().c_str(), static_cast<SizeType>(user.getName().length()));

            writer.EndObject();
        }
        writer.EndArray();
    }

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getGroupUsersResponse(MessageMap messages) {
    UserManager* userMan = UserManager::getInstance();
    GroupManager* groupMan = GroupManager::getInstance();
    string errorMsg{};
    string status{};

    MessageMap::iterator groupId = messages.find("groupId");

    Group group = groupMan->getGroupById(atoi(groupId->second.c_str()));

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == SUCCESS) {
        writer.StartArray();
        for (auto userId : group.members) {
            writer.StartObject();

            writer.String("userId");
            writer.Uint(userId);

            /*writer.String("username");

            writer.String(user.getName().c_str(), static_cast<SizeType>(user.getName().length()));*/

            writer.EndObject();
        }
        writer.EndArray();
    }

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

/*string ReaderFromClient::getGroupPendingUsersResponse(MessageMap messages);
string ReaderFromClient::getGroupsResponse(MessageMap messages);*/

string ReaderFromClient::getJoinGroupResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));
	int userId = (atoi(messages.find("userId")->second.c_str()));

	if (groupMan->getGroupById(groupId).addMember(userId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user wasn't in the group";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getLeaveGroupResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));
	int userId = (atoi(messages.find("userId")->second.c_str()));

	if (groupMan->getGroupById(groupId).removeMember(userId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user wasn't in the group";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getCreateGroupResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	string name = messages.find("name")->second.c_str();
	string description = messages.find("description")->second.c_str();
	int userId = atoi(messages.find("adminId")->second.c_str());
	int groupId = groupMan->createNewGroupId();

	Group group = Group(groupId, name, description, userId);

	if (groupMan->addGroup(groupMan->createNewGroupId(), group))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "GroupID already exists";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getDeleteGroupResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->removeGroup(groupId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "Group doesn't exist";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getKickUserResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = (atoi(messages.find("userId")->second.c_str()));
	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->removeUserFromGroup(groupId, userId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user wasn't in the group";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getPromoteUserResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = (atoi(messages.find("userId")->second.c_str()));
	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->setNewAdmin(groupId, userId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "User/Group doesn't exist";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getInviteUserResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = (atoi(messages.find("userId")->second.c_str()));
	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->addUserToGroup(groupId, userId))
	{
		status = "Success";
	}
	else {
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user was already in the group";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getDeclineRequestResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = (atoi(messages.find("userId")->second.c_str()));
	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->removeUserPending(groupId, userId))
	{
		status = "Success";
	}
	else
	{
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user was not in the group pending invitation";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getApproveRequestResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = (atoi(messages.find("userId")->second.c_str()));
	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->removeUserPending(groupId, userId))
	{
		groupMan->addUserToGroup(groupId, userId);
		status = "Success";
	}
	else
	{
		status = "Failed";
		errorMsg = "User/Group doesn't exist or the user was not in the group pending invitation";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}