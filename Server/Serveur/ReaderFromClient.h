#ifndef READERFROMCLIENT_H
#define READERFROMCLIENT_H

#include "rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <vector>

using namespace rapidjson;
using namespace std;

enum ClientFunction { Register, LogIn, LogOut, GetOnlineUsers, GetGroupUsers, GetGroupPendingUsers, GetGroups, JoinGroup, LeaveGroup, DeleteGroup, KickUser, PromoteUser, InviteUser, DeclineRequest, ApproveRequest, GetFiles, Error };
static vector<string> ClientFunctionStrings{ "Register", "LogIn", "LogOut", "GetOnlineUsers", "GetGroupUsers", "GetGroupPendingUsers", "GetGroups", "JoinGroup", "LeaveGroup", "DeleteGroup", "KickUser", "PromoteUser", "InviteUser", "DeclineRequest", "ApproveRequest", "GetFiles" };

static ClientFunction getEnumFromText(string val) {
    for (int i = 0; i < ClientFunctionStrings.size(); ++i)
        if (ClientFunctionStrings[i] == val)
            return (ClientFunction)i;
}

typedef map<string, string> MessageMap;

struct MessageHandler
    : public BaseReaderHandler<UTF8<>, MessageHandler> {
    MessageHandler() : messages_(), state_(kExpectObjectStart), name_() {}

    bool StartObject() {
        switch (state_) {
        case kExpectObjectStart:
            state_ = kExpectNameOrObjectEnd;
            return true;
        default:
            return false;
        }
    }

    bool String(const char* str, SizeType length, bool) {
        switch (state_) {
        case kExpectNameOrObjectEnd:
            name_ = string(str, length);
            state_ = kExpectValue;
            return true;
        case kExpectValue:
            messages_.insert(MessageMap::value_type(name_, string(str, length)));
            state_ = kExpectNameOrObjectEnd;
            return true;
        default:
            return false;
        }
    }

    bool EndObject(SizeType) { return state_ == kExpectNameOrObjectEnd; }

    bool Default() { return false; } // All other events are invalid.

    MessageMap messages_;
    enum State {
        kExpectObjectStart,
        kExpectNameOrObjectEnd,
        kExpectValue
    }state_;
    std::string name_;
};


class ReaderFromClient
{
public:
    ReaderFromClient()=delete;
    ~ReaderFromClient()=delete;
    static void ParseMessages(const char* json, MessageMap& messages);
    static MessageMap getMessageMapFromJson(string json);
    static ClientFunction getFunctionId(MessageMap messages);
    static string getRegisterResponse(MessageMap& messages);
    static string getLogInResponse(MessageMap messages);
    static string getLogOutResponse(MessageMap messages);
    /*static string getOnlineUsersResponse(MessageMap messages);
    static string getGroupUsersResponse(MessageMap messages);
    static string getGroupPendingUsersResponse(MessageMap messages);
    static string getGroupsResponse(MessageMap messages);
    static string getJoinGroupsResponse(MessageMap messages);
    static string getLeaveGroupResponse(MessageMap messages);
    static string getDeleteGroupResponse(MessageMap messages);
    static string getKickUserResponse(MessageMap messages);
    static string getPromoteUserResponse(MessageMap messages);
    static string getInviteUserResponse(MessageMap messages);
    static string getDeclineRequestResponse(MessageMap messages);
    static string getApproveRequestResponse(MessageMap messages);*/
    //static string getGetFilesResponse(MessageMap messages);
    
        //status , errorInfo ==empty si pas d'erreur, retour 
};

#endif // !READERFROMCLIENT_H