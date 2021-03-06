
#include "UserManager.h"
#include "GroupManager.h"
#include "LoginManager.h"
#include "ReaderFromClient.h"
#include "Server.h"
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>

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
        loginMan->addUserAuthentification(username->second, password->second);
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
        writer.String("id");
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
        writer.String("users");
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
	LoginManager* loginMan = LoginManager::getInstance();
    UserManager* userMan = UserManager::getInstance();
    GroupManager* groupMan = GroupManager::getInstance();
    Group* group;
    string errorMsg{};
    string status{};

	int groupId = atoi(messages.find("groupId")->second.c_str());

    try
    {
        group = groupMan->getGroupById(groupId);
        status = SUCCESS;
    }
    catch (std::exception& e)
    {
        status = FAILED;
        errorMsg = e.what();
    }

    if (group->members.empty()) {
        status = FAILED;
        errorMsg = "No members in group";
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == SUCCESS) {
        writer.String("users");
        writer.StartArray();
        for (auto userId : group->members) {
            writer.StartObject();

            writer.String("userId");
            writer.Uint(userId);

			User user = userMan->getUserById(userId);
            writer.String("username");
            writer.String(user.getName().c_str(), static_cast<SizeType>(user.getName().length()));

			writer.String("isConnected");
			writer.Bool(loginMan->isUserConnected(userId));

            writer.EndObject();
        }
        writer.EndArray();
    }

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getGroupPendingUsersResponse(MessageMap messages) {
    UserManager* userMan = UserManager::getInstance();
    GroupManager* groupMan = GroupManager::getInstance();
    Group* group;
    string errorMsg{};
    string status{};

	int groupId = atoi(messages.find("groupId")->second.c_str());
    
    try
    {
        group = groupMan->getGroupById(groupId);
        status = SUCCESS;
    }
    catch (std::exception& e)
    {
        status = FAILED;
        errorMsg = e.what();
    }

    if (group->pendingInvitations.empty()) {
        status = FAILED;
        errorMsg = "No pending invitation in this group";
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == SUCCESS) {
        writer.String("users");
        writer.StartArray();
        for (auto userId : group->pendingInvitations) {
            writer.StartObject();

            writer.String("userId");
            writer.Uint(userId);

            writer.String("username");
            User user = userMan->getUserById(userId);
            writer.String(user.getName().c_str(), static_cast<SizeType>(user.getName().length()));

                writer.EndObject();
        }
        writer.EndArray();
    }

    writer.EndObject();

    puts(sb.GetString());

    return sb.GetString();
}

string ReaderFromClient::getGroupsResponse(MessageMap messages) {
	GroupManager* groupMan = GroupManager::getInstance();
	UserManager* userMan = UserManager::getInstance();
	string errorMsg{};
	string status{};

	int userId = atoi(messages.find("userId")->second.c_str());

	if (!groupMan->getAllGroups().empty()) {
		status = SUCCESS;
	}
	else {
		errorMsg = "No group exists";
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

		writer.String("inList");
		writer.StartArray();
		for (auto group : groupMan->getAllGroupsForUser(userId)) {
			writer.StartObject();

			writer.String("groupId");
			writer.Uint(group.getId());
                        
			writer.String("name");
			writer.String(group.name.c_str(), static_cast<SizeType>(group.name.length()));

			writer.String("description");
			writer.String(group.description.c_str(), static_cast<SizeType>(group.description.length()));

            writer.String("adminId");
            writer.Uint(group.admin.getUserId());

			writer.EndObject();
		}
		writer.EndArray();

		writer.String("pendingList");
		writer.StartArray();
		for (auto group : groupMan->getAllPendingGroupsForUser(userId)) {
			writer.StartObject();

			writer.String("groupId");
			writer.Uint(group.getId());

            writer.String("name");
			writer.String(group.name.c_str(), static_cast<SizeType>(group.name.length()));

			writer.String("description");
			writer.String(group.description.c_str(), static_cast<SizeType>(group.description.length()));

            writer.String("adminId");
            writer.Uint(group.admin.getUserId());

			writer.EndObject();
		}
		writer.EndArray();

		writer.String("outList");
		writer.StartArray();
		for (auto group : groupMan->getAllOutGroupForUser(userId)) {
			writer.StartObject();

			writer.String("groupId");
			writer.Uint(group.getId());

			writer.String("name");
			writer.String(group.name.c_str(), static_cast<SizeType>(group.name.length()));

			writer.String("description");
			writer.String(group.description.c_str(), static_cast<SizeType>(group.description.length()));

            writer.String("adminId");
            writer.Uint(group.admin.getUserId());

			writer.EndObject();
		}
		writer.EndArray();
	}
	else {
		writer.String("inList");
		writer.StartArray();
		writer.EndArray();

		writer.String("pendingList");
		writer.StartArray();
		writer.EndArray();

		writer.String("outList");
		writer.StartArray();
		writer.EndArray();
	}

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getJoinGroupResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));
	int userId = (atoi(messages.find("userId")->second.c_str()));

	if (groupMan->getGroupById(groupId)->addPendingInvitation(userId))
	{
		status = SUCCESS;
	}
	else {
		status = FAILED;
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

	if (groupMan->getGroupById(groupId)->removeMember(userId))
	{
		status = SUCCESS;
	}
	else {
		status = FAILED;
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

	string name = messages.find("name")->second;
	string description = messages.find("description")->second;
	int userId = atoi(messages.find("adminId")->second.c_str());
	int groupId = groupMan->createNewGroupId();

	if (groupMan->addGroup(name, description, userId))
	{
		status = SUCCESS;
	}
	else {
		status = FAILED;
		errorMsg = "Group name already exists";
	}

	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	if (status == SUCCESS) {
		writer.String("id");
		writer.Uint(groupId);
	}

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
		status = SUCCESS;
	}
	else {
		status = FAILED;
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
		status = SUCCESS;
	}
	else {
		status = FAILED;
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
		status = SUCCESS;
	}
	else {
		status = FAILED;
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
	UserManager* UserManager = UserManager::getInstance();
	string errorMsg{};
	string status{};

	string username = messages.find("username")->second;
	int groupId = (atoi(messages.find("groupId")->second.c_str()));
	User user = UserManager->getUserByName(username);

	if (groupMan->addUserToGroup(groupId, user.getId()))
	{
		status = SUCCESS;
	}
	else {
		status = FAILED;
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
		status = SUCCESS;
	}
	else
	{
		status = FAILED;
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
		status = SUCCESS;
	}
	else
	{
		status = FAILED;
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

string ReaderFromClient::getSendFileResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	string fileName = messages.find("name")->second.c_str();
	string fileContent = messages.find("content")->second.c_str();
	
	std::stringstream ss(fileContent);
	vector<char> vect;
	char i;
	std::string token;
	int last = 0;

	while (std::getline(ss, token, ',')) {
			vect.push_back(stoi(token));
			last = stoi(token);
	};
	try {
		vector<char> mergedVector;
		vector<char> currentBytes = PendingFiles[fileName];
		mergedVector.reserve(currentBytes.size() + vect.size());
		mergedVector.insert(mergedVector.end(), currentBytes.begin(), currentBytes.end());
		mergedVector.insert(mergedVector.end(), vect.begin(), vect.end());
		PendingFiles[fileName] = mergedVector;
		status = SUCCESS;
	}
	catch (exception e) {
		status = FAILED;
		errorMsg = "Could update the pending file" + fileName;
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

string ReaderFromClient::getCreateFileResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	string fileName = messages.find("name")->second.c_str();
	try {
		vector<char> byteToStore;
		PendingFiles.insert({ fileName , byteToStore });
		status = SUCCESS;
	}
	catch (exception e) {
		status = FAILED;
		errorMsg = "Could not create the file" + fileName;
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

string ReaderFromClient::getFileTransferCompleteResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));
	string fileName = messages.find("name")->second.c_str();
	string filePath = Server::ROOT + groupMan->getGroupById(groupId)->getName();
	boost::filesystem::path dir(filePath);

	if (!(boost::filesystem::exists(dir))) {

		if (boost::filesystem::create_directory(dir)) {
            
		}
		else {
			status = FAILED;
			errorMsg = "Could not create or reach the file directory for the group id" + groupId;
		}
	}

	try {
		File* file = new File(fileName, groupMan->getGroupById(groupId)->createNewFileId());
		if(groupMan->getGroupById(groupId)->getFileFromName(fileName) == nullptr) {
			groupMan->getGroupById(groupId)->addFile(file);
		}
		else {
			groupMan->getGroupById(groupId)->updateFile(file);
		}
		string path = filePath + "/" + fileName;
		std::ofstream(path, std::ios::binary).write(PendingFiles[fileName].data(), PendingFiles[fileName].size());
		status = SUCCESS;
	}
	catch (exception e) {
		status = FAILED;
		errorMsg = "Could not store the file" + fileName;
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

string ReaderFromClient::synchronizeFilesResponse(MessageMap messages)
{
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	string fileList = messages.find("fileList")->second.c_str();
	vector<string> files;
	vector<string> existingFiles;
	vector<string> missingFiles;
	string lastName;
	try {
		boost::split(files, fileList, boost::is_any_of("=;,"));
		string groupId = "";
		for (auto &name : files) {
			if(name != "" && name.back() == 'd'){
				lastName = name;
				if (groupId != "") {
					for (auto &fileName : existingFiles) {
						missingFiles.push_back(fileName);
					}
				}
				groupId = name;
				missingFiles.push_back(groupId);
				existingFiles = get_all_files_names_within_folder(Server::ROOT + '/' + groupId.substr(0, name.size() - 1) + "/*");
			}
			else {
				if (name == "") {
					for (auto &fileName : existingFiles) {
						missingFiles.push_back(fileName);
					}
				}
				else {
					auto itr = std::find(existingFiles.begin(), existingFiles.end(), name);
					if (itr != existingFiles.end()) existingFiles.erase(itr);
				}
			}
		}
		if (lastName != ""){
			for (auto &fileName : existingFiles) {
				missingFiles.push_back(fileName);
			}
		}		
		status = SUCCESS;
	}
	catch (exception e) {
		status = FAILED;
		errorMsg = "Error while searching for files";
	}
	string currentGroup = "";
	set<string> filesPresent;
	vector<string> uniqueMissingFiles;
	string last;
	for (auto const& s : missingFiles) {
		if (currentGroup == "") {
			currentGroup = s;
			uniqueMissingFiles.push_back(currentGroup);
		}
		else if (s != "" && s.back() == 'd') {
			currentGroup = s;
			std::copy(filesPresent.begin(), filesPresent.end(), std::back_inserter(uniqueMissingFiles));
			uniqueMissingFiles.push_back(currentGroup);
			filesPresent.clear();
		}
		else {
			filesPresent.insert(s);
		}
		last = s;
	}
	if (last != "" && last.back() != 'd') {
		std::copy(filesPresent.begin(), filesPresent.end(), std::back_inserter(uniqueMissingFiles));
	}
	std::string result;
	for (auto const& s : uniqueMissingFiles) { result += "," + s; }
	if (result.length() != 0) {
		result = result.substr(1, result.size());
	}
	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.String("missingFiles");
	writer.String(result.c_str(), static_cast<SizeType>(result.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getRenamedFileResponse(MessageMap messages) {
    GroupManager* groupMan = GroupManager::getInstance();
    string errorMsg{};
    string status{};

    int groupId = (atoi(messages.find("groupId")->second.c_str()));
    string oldFileName = messages.find("oldFileName")->second.c_str();
    string newFileName = messages.find("newFileName")->second.c_str();
    
    string filePath = Server::ROOT + groupMan->getGroupById(groupId)->getName() + "/";
    boost::filesystem::path dir(filePath);

    File* file = groupMan->getGroupById(groupId)->getFileFromName(oldFileName);

    if (file != nullptr) {
        boost::filesystem::path dir(filePath);

        if (boost::filesystem::exists(dir)) {
            string oldFileFullName = filePath + oldFileName;
            string newFileFullName = filePath + newFileName;
            if (rename(oldFileFullName.c_str(), newFileFullName.c_str()) != 0) {
                status = FAILED;
                errorMsg = "Could not rename file " + oldFileName + "from repository";
            }
            else {
                if (!groupMan->getGroupById(groupId)->renameFile(file, newFileName)) {
                    status = FAILED;
                    errorMsg = "Could not rename file " + oldFileName + "from datastructure";
                }
                else
                    status = SUCCESS;
            }
        }
        else {
            status = FAILED;
            errorMsg = "Could not find " + filePath;
        }
    }
    else {
        status = FAILED;
        errorMsg = "Could not find file " + oldFileName + "in datastructure";
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

string ReaderFromClient::getDeletedFileResponse(MessageMap messages) {
    GroupManager* groupMan = GroupManager::getInstance();
    string errorMsg{};
    string status{};

int groupId = (atoi(messages.find("groupId")->second.c_str()));
string fileName = messages.find("fileName")->second.c_str();

File* file = groupMan->getGroupById(groupId)->getFileFromName(fileName);

if (file != nullptr) {
	string filePath = Server::ROOT + groupMan->getGroupById(groupId)->getName() + "/";
	boost::filesystem::path dir(filePath);

	if (boost::filesystem::exists(dir)) {
		string fileFullName = filePath + fileName;
		if (remove(fileFullName.c_str()) != 0) {
			status = FAILED;
			errorMsg = "Could not delete file " + fileName + "from repository";
		}
		else {
			if (!groupMan->getGroupById(groupId)->removeFile(file)) {
				status = FAILED;
				errorMsg = "Could not delete file " + fileName + "from datastructure";
			}
			else
				status = SUCCESS;
		}
	}
	else {
		status = FAILED;
		errorMsg = "Could not find " + filePath;
	}
}
else {
	status = FAILED;
	errorMsg = "Could not find file " + fileName + "in datastructure";
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

string ReaderFromClient::sendNextFilePartResponse(MessageMap messages) {
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	string filePath = messages.find("filePath")->second.c_str();
	int const readingSize = 100;

	int start = 0;
	char* buffer = new char[readingSize];

	if (TransferingFiles.count(filePath)) {
		start = TransferingFiles[filePath];
	}
	else {
		TransferingFiles[filePath] = 0;
	}
	string data = "";
	int size = 0;
	try {

			std::ifstream infile(Server::ROOT + filePath, std::ios::binary);
			infile.seekg(0, infile.end);
			size_t length = infile.tellg();
			infile.seekg(start);
			if (TransferingFiles[filePath] == -1) {
				data = "EndOfFile";
				TransferingFiles[filePath] = 0;
			}
			else {
				if (TransferingFiles[filePath] + readingSize >= length) {
					size = length - TransferingFiles[filePath];
					infile.read(buffer, size);
					TransferingFiles[filePath] = -1;
				}
				else {
					infile.read(buffer, readingSize);
					TransferingFiles[filePath] += 99;
				}
				string idk;
				if (TransferingFiles[filePath] == -1) {
					string result(buffer,size);
					idk = result;
				}
				else {
					string result(buffer);
					idk = result;
				}
				data = idk;
				data = data.substr(0, readingSize - 1);
			}
		status = SUCCESS;
	}

	catch (exception e) {
		status = FAILED;
		errorMsg = "Could not parse " + filePath;
	}
	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);
	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("fileParts");
	writer.String(data.c_str(), static_cast<SizeType>(data.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

string ReaderFromClient::getFileNamesResponse(MessageMap messages) {
	GroupManager* groupMan = GroupManager::getInstance();
	string errorMsg{};
	string status{};

	int groupId = (atoi(messages.find("groupId")->second.c_str()));

	if (groupMan->doesGroupExists(groupId)) {
		status = SUCCESS;
	} else{
		status = FAILED;
		errorMsg = "The group doesnt exists";
	}
	rapidjson::StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	writer.StartObject();
	writer.String("status");
	writer.String(status.c_str(), static_cast<SizeType>(status.length()));

	writer.String("errorInfo");
	writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

	if (status == SUCCESS) {
		writer.String("names");
		writer.StartArray();

		for (auto f : groupMan->getGroupById(groupId)->getFiles()) {
			writer.String(f.getName().c_str(), static_cast<SizeType>(f.getName().length()));
		}
		writer.EndArray();
	}
	else {
		writer.String("names");
		writer.StartArray();
		writer.EndArray();
	}

	writer.EndObject();

	puts(sb.GetString());

	return sb.GetString();
}

vector<string>  ReaderFromClient::get_all_files_names_within_folder(string folder)
{
	vector<string> files;
	WIN32_FIND_DATA search_data;
	memset(&search_data, 0, sizeof(WIN32_FIND_DATA));
	std::wstring stemp = std::wstring(folder.begin(), folder.end());
	LPCWSTR path = stemp.c_str();
	HANDLE handle = FindFirstFile(path, &search_data);

	while (handle != INVALID_HANDLE_VALUE)
	{
		wstring ws(search_data.cFileName);
		// your new String
		string fileName(ws.begin(), ws.end());
		if (fileName != "." && fileName != "..")
		files.push_back(fileName);

		if (FindNextFile(handle, &search_data) == FALSE)
			break;
	}

	//Close the handle after use or memory/resource leak
	FindClose(handle);
	return files;
}


bool ReaderFromClient::isInteger(const std::string & s)
{
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char * p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}
