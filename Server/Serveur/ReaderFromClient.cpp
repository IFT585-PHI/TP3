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
        status = "Success";
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
            user.setIsConnected(true);
            status = "Success";
        }
        else {
            errorMsg = "User already logged in";
            status = "Failed";
        }
    }
    else {
        errorMsg = "Invalid user";
        status = "Failed";
    }

    rapidjson::StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);

    writer.StartObject();
    writer.String("status");
    writer.String(status.c_str(), static_cast<SizeType>(status.length()));

    writer.String("errorInfo");
    writer.String(errorMsg.c_str(), static_cast<SizeType>(errorMsg.length()));

    if (status == "Success") {
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
        user.setIsConnected(false);
        status = "Success";
    }
    else {
        errorMsg = "User failed to logout";
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

/*string ReaderFromClient::getOnlineUsersResponse(MessageMap messages);
string ReaderFromClient::getGroupUsersResponse(MessageMap messages);
string ReaderFromClient::getGroupPendingUsersResponse(MessageMap messages);
string ReaderFromClient::getGroupsResponse(MessageMap messages);
string ReaderFromClient::getLeaveGroupResponse(MessageMap messages);
string ReaderFromClient::getJoinGroupsResponse(MessageMap messages);
string ReaderFromClient::getDeleteGroupResponse(MessageMap messages);
string ReaderFromClient::getKickUserResponse(MessageMap messages);
string ReaderFromClient::getPromoteUserResponse(MessageMap messages);
string ReaderFromClient::getInviteUserResponse(MessageMap messages);
string ReaderFromClient::getDeclineRequestResponse(MessageMap messages);
string ReaderFromClient::getApproveRequestResponse(MessageMap messages);*/