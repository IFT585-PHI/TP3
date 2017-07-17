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
        List<int> _pendingInvitations;          //int = groupID

        public User(int id, string name)
            : base(id)
        {
            _name = name;
            _lastSynchronized = DateTime.Now;
        }

        public User(int id, string name, DateTime lastSynchronized, Dictionary<int, ShareFolder> groups, List<int> pendingInvitations)
            : base(id)
        {
            _name = name;
            _lastSynchronized = lastSynchronized;
            _groups = groups;
            _pendingInvitations = pendingInvitations;
        }

        public void CreateGroup(string groupName, string description)
        {
            Server.GetGroupManager().AddGroup(_groups.Count, new Group(_groups.Count, groupName, description, _id));
        }
        public void ApplyGroup(int idGroup)
        {
            Server.GetGroupManager().GetGroupById(idGroup).AddPendingInvitation(_id);
        }

        public void AddPendingInvitation(int groupId)
        {
            _pendingInvitations.Add(groupId);
        }

        public bool AcceptInvitation(int groupId)
        {
            if (!DoesPendingInvitationExists(groupId))
                return false;

            Server.GetGroupManager().AddUserToGroup(groupId, _id);
            _pendingInvitations.Remove(groupId);
            return true;
        }

        public bool DenyInvitation(int groupId)
        {
            if (!DoesPendingInvitationExists(groupId))
                return false;

            Server.GetGroupManager().RemoveUserFromGroup(groupId, _id);
            _pendingInvitations.Remove(groupId);
            return true;
        }

        public void Synchronize()
        {
            // do something


            _lastSynchronized = DateTime.Now;
        }

        private bool DoesPendingInvitationExists(int groupId)
        {
            return _pendingInvitations.Contains(groupId);
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
    }
}
