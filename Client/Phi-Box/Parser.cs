﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public enum ClientFunction { Register, LogIn, LogOut, GetOnlineUsers, GetGroupUsers, GetGroupPendingUsers, GetGroups, CreateGroup, JoinGroup, LeaveGroup, DeleteGroup, KickUser, PromoteUser, InviteUser, DeclineRequest, ApproveRequest, GetFiles, SendFile, CreatePendingFile, FileTransferComplete, AddedFile, RenamedFile, UpdatedFile, DeletedFile };
    public enum Status { Success, Failed };

    class Parser
    {
        public class Response
        {
            public Status status {get; set;}
            public string errorInfo { get; set; }
        }
        public class IdResponse : Response
        {
            public string id { get; set; }
        }

        public class ListUsersResponse : Response
        {
            public List<User> users { get; set; }
        }

        public class ListGroupsResponse : Response
        {
            public List<User> members { get; set; }
        }

        public class TripleListsGroupsForUserResponse : Response
        {
            public List<Group> inList { get; set; }
            public List<Group> pendingList { get; set; }
            public List<Group> outList { get; set; }
        }
    }
}
