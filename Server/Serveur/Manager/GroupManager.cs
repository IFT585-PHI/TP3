using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class GroupManager
    {
        Dictionary<int, Group> groups; // int = groupId

        public GroupManager()
        {
            groups = new Dictionary<int, Group>();
        }

        public void Initialize()
        {
            // Build groups from file parser
        }

        public bool AddGroup(int groupId, Group group)
        {
            if (groups.ContainsKey(groupId) || groups.ContainsValue(group))
                return false;

            groups.Add(groupId, group);
            return true;
        }

        public bool AddUserToGroup(int groupId, int userId)
        {
            if (DoesGroupExists(groupId))
                return false;

            groups[groupId].AddMember(userId);
            return true;
        }

        public bool RemoveUserFromGroup(int groupId, int userId)
        {
            if (!DoesGroupExists(groupId))
                return false;

            groups[groupId].RemoveMember(userId);
            return true;
        }

        public bool SetNewAdmin(int groupId, int userId)
        {
            if (!DoesGroupExists(groupId))
                return false;

            groups[groupId].SetAdmin(userId);
            return true;
        }

        public Group GetGroupById(int groupId)
        {
            if (!DoesGroupExists(groupId))
                return null;

            return groups[groupId];           
        }

        private bool DoesGroupExists(int groupId)
        {
            return groups.ContainsKey(groupId);
        }
    }
}
