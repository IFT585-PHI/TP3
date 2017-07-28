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
using System.Runtime.Serialization;

namespace Phi_Box 
{
    public class Client
    {
        private User connectedUser;

        public Client()
        {
        }

        public void Submit_TCP_Request()
        {
            try
            {
                TcpClient client = new TcpClient();
                Console.WriteLine("Connection started ...");

                client.Connect("192.168.0.100", 13);
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
                Console.WriteLine("ERROR: " + ex.Message);
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
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.LogIn.ToString());
            dict.Add("username", username);
            dict.Add("password", password);
            string json = JsonConvert.SerializeObject(dict);
            int userId = -1;

            Parser.IdResponse res = JsonConvert.DeserializeObject<Parser.IdResponse>(RequestToServer(json));          

            if (res.status == Status.Success)
                userId = res.id;
            else
            {
                Console.WriteLine("ERROR: " + res.errorInfo);
                return false;
            }

            connectedUser = new User(userId, username, true);
    
            return true;
        }

        /// <summary>
        /// Register a new user
        /// </summary>
        /// <param name="username"></param>
        /// <param name="password"></param>
        /// <returns></returns>
        public bool Register(string username, string password)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.Register.ToString());
            dict.Add("username", username);
            dict.Add("password", password);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            {}
            else
            {
                Console.WriteLine("ERROR: " + res.errorInfo);
                return false;
            }

            return LogIn(username, password);
        }

        /// <summary>
        /// Log out the current loggedIn user
        /// </summary>
        public void LogOut()
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.LogOut.ToString());
            dict.Add("userId", connectedUser.id.ToString());
            string json = JsonConvert.SerializeObject(dict);

            RequestToServer(json);
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
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetOnlineUsers.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<User> users = new List<User>();

            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(RequestToServer(json));

            if (res.status == Status.Success)
                users = res.userList;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            return users;
        }        

        /// <summary>
        /// Get the list of the users present in a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupUsers(int groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroupUsers.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<User> users = new List<User>();

            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(RequestToServer(json));

            if (res.status == Status.Success)
                users = res.userList;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            return users;
        }

        /// <summary>
        /// Get the list of the pending users for a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupPendingUsers(int groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroupPendingUsers.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<User> users = new List<User>();

            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(RequestToServer(json));

            if (res.status == Status.Success)
                users = res.userList;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            return users;
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
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroupPendingUsers.ToString());
            dict.Add("name", name);
            dict.Add("description", description);
            dict.Add("adminId", connectedUser.id.ToString());
            string json = JsonConvert.SerializeObject(dict);
            int groupId = -1;

            Parser.IdResponse res = JsonConvert.DeserializeObject<Parser.IdResponse>(RequestToServer(json));

            if (res.status == Status.Success)
                groupId = res.id;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            Group group = new Group(groupId, name, description, connectedUser.id, GroupStatus.IN);

            List<Group> groups = GetGroups();
            groups.Add(group);
            
            return group;
        }

        /// <summary>
        /// Get the list of groups
        /// </summary>
        /// <returns></returns>
        public List<Group> GetGroups()
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroups.ToString());
            dict.Add("userId", connectedUser.id.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<Group> groups = new List<Group>();

            Parser.TripleListsGroupsForUserResponse res = JsonConvert.DeserializeObject<Parser.TripleListsGroupsForUserResponse>(RequestToServer(json));

            if (res.status == Status.Success)
            {
                foreach (Group g in res.inList)
                    groups.Add(new Group(g.id, g.name, g.description, g.adminId, GroupStatus.IN));

                foreach (Group g in res.pendingList)
                    groups.Add(new Group(g.id, g.name, g.description, g.adminId, GroupStatus.PENDING));

                foreach (Group g in res.outList)
                    groups.Add(new Group(g.id, g.name, g.description, g.adminId, GroupStatus.OUT));
            }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            return groups;
        }

        /// <summary>
        /// The loggedIn user ask to join a group
        /// </summary>
        /// <param name="groupId"></param>
        public void JoinGroup(int groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.JoinGroup.ToString());
            dict.Add("userId", connectedUser.id.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            {}
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// The loggedIn user leave a group
        /// </summary>
        /// <param name="groupId"></param>
        public void LeaveGroup(int groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.LeaveGroup.ToString());
            dict.Add("userId", connectedUser.id.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Delete the group
        /// </summary>
        /// <param name="groupId"></param>
        public void DeleteGroup(int groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.DeleteGroup.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }        

        /// <summary>
        /// Kick a user from the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void KickUser(int groupId, int userId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.KickUser.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("userId", userId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Promote a user to be admin of the group instead of the current user
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void PromoteUser(int groupId, int userId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.PromoteUser.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("userId", userId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Invite User to the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="username"></param>
        public void InviteUser(int groupId, string username)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.InviteUser.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("userId", username);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Refuse a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void DeclineRequest(int groupId, int userId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.DeclineRequest.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("userId", userId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Approve a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void ApproveRequest(int groupId, int userId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.ApproveRequest.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("userId", userId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        string RequestToServer(string json)
        {
            string response = string.Empty;

            try
            {
                TcpClient client = new TcpClient();
                Console.WriteLine("Connection started...");

                client.Connect("192.168.0.100", 13);
                Console.WriteLine("Connected");

                NetworkStream ns = client.GetStream();

                Console.WriteLine("Transmition of the request...");
                StreamWriter sw = new StreamWriter(ns);
                sw.Write(json + ".");
                sw.Flush();

                Console.WriteLine("Reception of the request...");
                StreamReader sr = new StreamReader(ns);
                response = sr.ReadLine();

                client.Close();
                Console.WriteLine("Connection closed");
            }
            catch (Exception ex)
            {
                Console.WriteLine("ERROR: " + ex.Message);
            }

            return response;
        }
    }
}
