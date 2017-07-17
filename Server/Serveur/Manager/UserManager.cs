using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class UserManager
    {
        Dictionary<int, User> existingUsers; // int = userId

        public UserManager()
        {
            existingUsers = new Dictionary<int, User>();
        }

        public void Initialize()
        {
            // Build existingUsers from file parser
        }

        public bool AddNewUser(String userName)
        {
            foreach(User u in GetListExistingUsers())
            {
                if (u.GetName() == userName)
                    return false;
            }

            existingUsers.Add(existingUsers.Count, new User(existingUsers.Count, userName));
            return true;
        }

        public bool SendInvitation(int userId, int groupId)
        {
            if (!existingUsers.ContainsKey(userId))
                return false;

            existingUsers[userId].AddPendingInvitation(groupId);
            return true;
        }

        public List<int> GetAllGroupIdFor(int id)
        {
            if (existingUsers.ContainsKey(id))
                return existingUsers[id].getGroups();

            return null;
        }

        public void Synchronize()
        {
            foreach (User u in existingUsers.Values.ToList())
            {
                u.Synchronize();
            }
        }

        public List<User> GetListExistingUsers()
        {
            return existingUsers.Values.ToList();
        }

        public Dictionary<int, User> GetExistingUsers()
        {
            return existingUsers;
        }

        public User GetUserByName(string userName)
        {
            User user = null;

            foreach(User u in GetListExistingUsers())
            {
                if(u.GetName() == userName)
                {
                    user = u;
                    break;
                }
            }

            return user;
        }

        public User GetUserById(int userId)
        {
            if (!DoesUserExists(userId))
                return null;

            return existingUsers[userId];
        }

        private bool DoesUserExists(int userId)
        {
            return existingUsers.ContainsKey(userId);
        }
    }
}
