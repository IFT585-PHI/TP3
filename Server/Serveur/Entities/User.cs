using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class User : Entity, ISynchronize
    {
        string _name;
        bool _isConnected = false;
        DateTime _lastSynchronized;

        Dictionary<int, ShareFolder> _groups;   //int = groupID
        List<int> _pendingInvitations;           //int = groupID

        public User(int id, string name)
            : base(id)
        {
            _name = name;
            _lastSynchronized = DateTime.Now;
        }

        public User(int id, string name, Dictionary<int, ShareFolder> groups, List<int> pendingInvitations)
            : base(id)
        {
            _name = name;
            _groups = groups;
            _pendingInvitations = pendingInvitations;
        }

        public void CreateGroup(string groupName, string description)
        {
            int lastIndex = _groups.Count;
            Group group = new Group(lastIndex, groupName, description, _id);
            Server.getGroupManager().AddGroup(lastIndex, group);
        }
        public void ApplyGroup(int idGroup)
        {
            Server.getGroupManager().GetGroupById(idGroup).AddPendingInvitation(_id);
        }

        public void AddPendingInvitation(int groupId)
        {
            _pendingInvitations.Add(groupId);
        }

        public bool AcceptInvitation(int groupId)
        {
            if (!DoesPendingInvitationExists(groupId))
                return false;

            Server.getGroupManager().AddUserToGroup(groupId, _id);
            _pendingInvitations.Remove(groupId);
            return true;
        }

        public bool DenyInvitation(int groupId)
        {
            if (!DoesPendingInvitationExists(groupId))
                return false;

            Server.getGroupManager().RemoveUserFromGroup(groupId, _id);
            _pendingInvitations.Remove(groupId);
            return true;
        }

        public void Synchronize()
        {
            // do something


            _lastSynchronized = DateTime.Now;
        }

        public List<int> getGroups()
        {
            return _groups.Keys.ToList();
        }

        public bool GetIsConnected()
        {
            return _isConnected;
        }

        public void SetIsConnected(bool isConnected)
        {
            _isConnected = isConnected;
        }

        public String GetName()
        {
            return _name;
        }

        private bool DoesPendingInvitationExists(int groupId)
        {
            return _pendingInvitations.Contains(groupId);
        }
    }
}
