using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Linq;
using System.Net.Sockets;
using System.Net;
using System.Net.Cache;
using System.Windows;

namespace Phi_Box
{
    public class Client
    {

        private User connectedUser;        

        public Client()
        {
            //TODO: Create Connection to Server
            
        }

        public void Submit_TCP_Request()
        {
            try
            {
                TcpClient client = new TcpClient();
                Console.WriteLine("Connection started ...");

                client.Connect("192.168.0.171", 13);
                Console.WriteLine("Connected");

                Console.WriteLine("Transmition of the request : Test.");

                string str = "Test.";
                NetworkStream ns = client.GetStream();
                StreamWriter sw = new StreamWriter(ns);

                sw.Write(str);
                sw.Flush();

                StreamReader sr = new StreamReader(ns);
                string response = sr.ReadLine();
                Console.WriteLine("The reponse from the server :" + response);
                sr.Close();

                client.Close();
                Console.WriteLine("Connection closed");

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

       

        /********************************************
         *          TEMPORARY SECTION 
         ********************************************/
        private JArray ReadJSONFile(string fileName)
        {
            return JArray.Parse(File.ReadAllText("../../" + fileName));
        }


        /********************************************
         *          CONNECTION REQUESTS SECTION 
         ********************************************/

        /// <summary>
        /// Connect user to Phi-Box
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool LogIn(string username, string password)
        {
            bool isSuccesfull = true;

            //Connection user in server


            int id = 0; //Temporary, normallly, get all informations from results
            connectedUser = new User(id, username);

            return isSuccesfull;
        }

        /// <summary>
        /// Register a new user
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool Register(string username, string password)
        {
            bool isSuccesfull = true;

            //Send User to Server

            int id = 0; //Temporary, normallly, get all informations from results
            connectedUser = new User(id, username);

            return isSuccesfull;
        }

        /// <summary>
        /// Log out the current loggedIn user
        /// </summary>
        public void LogOut()
        {
            //Tell server User is disconnecting

            //Go back to login page
        }



        /********************************************
         *          USERS REQUESTS SECTION 
         ********************************************/

        /// <summary>
        /// Get the list of online users
        /// </summary>
        /// <returns></returns>
        public List<User> GetOnlineUsers()
        {
            //Request Online Users from server

            List<User> users = new List<User>();

            foreach (JObject o in ReadJSONFile("users.json").Children<JObject>())
            {
                var elem = o.ToObject<Dictionary<string, object>>();

                int id = Int32.Parse(elem["id"].ToString());
                string username = elem["username"].ToString();

                //Usually would compare with id
                if(username != connectedUser.username)
                    users.Add(new User(id, username));
            }

            return users;
        }        

        /// <summary>
        /// Get the list of the users present in a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupUsers(int groupId)
        {
            //Request the users that are in a group
            return GetOnlineUsers();
            //return new List<User>();
        }

        /// <summary>
        /// Get the list of the pending users for a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupPendingUsers(int groupId)
        {
            //Request the users that are pending

            return GetOnlineUsers();
            //return new List<User>();
        }



        /********************************************
         *          GROUPS REQUESTS SECTION 
         ********************************************/

        /// <summary>
        /// Create a group with a name and description
        /// </summary>
        /// <param name="name"></param>
        /// <param name="description"></param>
        /// <returns></returns>
        public Group CreateGroup(string name, string description)
        {
            //Send Group to Server

            int id = 0; //Temporary, normallly, get all informations from results
            Group group = new Group(id, name, description, (int)GroupStatus.IN);
            List<Group> groups = GetGroups();
            groups.Add(group);
            string json = JsonConvert.SerializeObject(groups.ToArray());
            System.IO.File.WriteAllText("../../groups.json", json);
            
            return group;
        }

        /// <summary>
        /// Get the list of groups
        /// </summary>
        /// <returns></returns>
        public List<Group> GetGroups()
        {
            //Request Groups from server

            List<Group> groups = new List<Group>();
            foreach (JObject o in ReadJSONFile("groups.json").Children<JObject>())
            {
                var elem = o.ToObject<Dictionary<string, object>>();                
                groups.Add(new Group(Int32.Parse(elem["id"].ToString()), elem["name"].ToString(), elem["description"].ToString(), Int32.Parse(elem["status"].ToString())));
            }

            return groups;
        }

        /// <summary>
        /// The loggedIn user ask to join a group
        /// </summary>
        /// <param name="groupId"></param>
        public void JoinGroup(int groupId)
        {
            Console.WriteLine("Asking to join the group " + groupId);
        }

        /// <summary>
        /// The loggedIn user leave a group
        /// </summary>
        /// <param name="groupId"></param>
        public void LeaveGroup(int groupId)
        {
            Console.WriteLine("Leaving the group " + groupId);
        }

        /// <summary>
        /// Delete the group
        /// </summary>
        /// <param name="groupId"></param>
        public void DeleteGroup(int groupId)
        {
            Console.WriteLine("Deleting the group " + groupId);
        }        

        /// <summary>
        /// Kick a user from the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void KickUser(int groupId, int userId)
        {
            Console.WriteLine("Kicking the userId " + userId + " in the group:" + groupId);
        }

        /// <summary>
        /// Promote a user to be admin of the group instead of the current user
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void PromoteUser(int groupId, int userId)
        {
            Console.WriteLine("Promoting the userId " + userId + " in the group:" + groupId);
        }

        /// <summary>
        /// Invite User to the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="username"></param>
        public void InviteUser(int groupId, string username)
        {
            Console.WriteLine("Inviting " + username + " in the group:" + groupId);
        }

        /// <summary>
        /// Refuse a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void DeclineRequest(int groupId, int userId)
        {
            Console.WriteLine("Declining request for userId:" + userId + " in the group:" + groupId);
        }

        /// <summary>
        /// Approve a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void ApproveRequest(int groupId, int userId)
        {
            Console.WriteLine("Approving request for userId:" + userId + " in the group:" + groupId);
        }

        

        /// <summary>
        /// Get the list of groups
        /// </summary>
        /// <returns></returns>
        public void GetFiles(int groupId)
        {
            //How files are going to look like????
        }
    }
}
