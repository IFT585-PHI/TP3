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
            Server.GetUserManager().SendInvitation(_groupId, userId);
        }

        public void Remove(int userId)
        {
            Server.GetGroupManager().RemoveUserFromGroup(_groupId, userId);
        }

        public void Delegate(int userId)
        {
            Server.GetGroupManager().SetNewAdmin(_groupId, userId);
        }

        public void Accept(int userId)
        {
            Server.GetGroupManager().GetGroupById(_groupId).AddMember(userId);
        }

        public void Deny(int userId)
        {
            Server.GetGroupManager().GetGroupById(_groupId).RemovePendingInvitation(userId);
        }

        public void SetUserId(int userId)
        {
            _userId = userId;
        }
    }
}
