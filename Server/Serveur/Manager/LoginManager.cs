using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class LoginManager
    {
        Dictionary<string, string> userAuthentification; // username and password
        List<User> connectedUsers;

        public LoginManager()
        {
            userAuthentification = new Dictionary<string, string>();
            connectedUsers= new List<User>();
        }

        public void Initialize()
        {
            // Build userAuthentification from file parser
        }

        public bool ValidateUserLogin(string userName, string password)
        {
            foreach (KeyValuePair<string, string> entry in userAuthentification)
            {
                if(entry.Key == userName && entry.Value == password)
                {
                    User user = Server.getUserManager().GetUserByName(userName);
                    user.SetIsConnected(true);
                    connectedUsers.Add(user);
                    return true;
                }
            }

            return false;
        }

        public List<User> GetConnectedUsers()
        {
            return connectedUsers;
        }

        public List<User> GetNotConnectedUsers()
        {
            List<User> notConnectedUsers = Server.getUserManager().GetListExistingUsers();

            foreach(User u in connectedUsers)
            {
                notConnectedUsers.Remove(u);
            }

            return notConnectedUsers;
        }

        public void AddUser(string userName, string password)
        {
            userAuthentification.Add(userName, password);

            Server.getUserManager().AddNewUser(userName);
        }

        //Synchronize connectedUsers
        public void Synchronize()
        {
            List<User> existingUsers = Server.getUserManager().GetListExistingUsers();

            foreach (User u in connectedUsers)
            {
                if (!u.GetIsConnected())
                {
                    u.SetIsConnected(false);
                    connectedUsers.Remove(u);
                }
            }
        }
    }
}
