using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class Admin
    {
        int _userId;
        int _groupId;

        List<int> _pendingInvitations; // int = userId

        public Admin(int userId, int groupId)
        {
            _userId = userId;
            _groupId = groupId;
        }

        public Admin(int userId, int groupId, List<int> pendingInvitations)
        {
            _userId = userId;
            _groupId = groupId;
            _pendingInvitations = pendingInvitations;
        }

        public void Invite(int userId)
        {
            Server.getUserManager().SendInvitation(_groupId, userId);
        }

        public void Remove(int userId)
        {
            Server.getGroupManager().RemoveUserFromGroup(_groupId, userId);
        }

        public void Delegate(int userId)
        {
            Server.getGroupManager().SetNewAdmin(_groupId, userId);
        }

        public void Accept(int userId)
        {
            Group group = Server.getGroupManager().GetGroupById(_groupId);
            group.AddMember(userId);
            group.RemovePendingInvitation(userId);
        }

        public void Deny(int userId)
        {
            Server.getGroupManager().GetGroupById(_groupId).RemovePendingInvitation(userId);
        }

        public void SetUserId(int userId)
        {
            _userId = userId;
        }
    }
}
