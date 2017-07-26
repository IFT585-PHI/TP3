using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class Client
    {
        User connectedUser;        

        public Client()
        {
            //TODO: Create Connection to Server

            //Get users

            //Get groups
        }


        public bool Connect(string username, string password)
        {
            bool connIsSuccesfull = false;

            //Connection user in server

            return connIsSuccesfull;
        }

        public void Disconnect()
        {
            //Tell server User is disconnecting

            //Go back to login page
        }


        public void RegisterUser(string username, string password)
        {
            bool connIsSuccesfull = false;
            //Send User to Server

            while (!connIsSuccesfull)
            {
                //Request to server
                connIsSuccesfull = true;

                //Deal with Error message

            }
            
            int id = 0; //Temporary, normallly, get all informations from results
            connectedUser = new User(id, username);
        }
        public Group AddGroup(string name, string description)
        {
            bool connIsSuccesfull = true;
            //Send Group to Server

            //Wait confirmation  

            if (connIsSuccesfull)
            {
                int id = 0; //Temporary, normallly, get all informations from results
                return new Group(id, name, description, (int)GroupStatus.IN);
            }
            else
                return new Group();
        }



        public List<User> GetOnlineUsers()
        {
            //Request Online Users from server

            return new List<User>();
        }

        public List<User> GetUsers(Group g)
        {
            //Request the users that are in a group

            return new List<User>();
        }
        public List<User> GetGroupUsers(Group g)
        {
            //Request the users that are in a group

            return new List<User>();
        }
        public List<User> GetGroupPendingUsers()
        {
            //Request the users that are pending

            return new List<User>();
        }

        public List<Group> GetGroups()
        {
            //Request Groups from server

            return new List<Group>();
        }





    }
}
