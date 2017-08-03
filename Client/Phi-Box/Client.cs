using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Net.Sockets;
using System.Windows;
using System.Linq;
using System.Text;
using System.Net;
using System.Threading.Tasks;
using System.Threading;

namespace Phi_Box 
{
    public class Client
    {

        public User connectedUser;
        public Window popup;
        static public string ipAddresse;

        public Client()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    ipAddresse = ip.ToString();
                }
            }
        }

        /********************************************
         *          UTILITY REQUESTS SECTION 
         ********************************************/

        private void DisplayError(string message)
        {
            MessageBox.Show(message);
        }

        static string RequestToServer(string json)
        {
            string response = string.Empty;

            try
            {
                TcpClient client = new TcpClient();
                Console.WriteLine("Connection started...");

                client.Connect(ipAddresse, 1313);
                Console.WriteLine("Connected");

                NetworkStream ns = client.GetStream();

                Console.WriteLine("Transmition of the request...");
                StreamWriter sw = new StreamWriter(ns);
                sw.Write(json + ".");
                sw.Flush();

                Console.WriteLine("Reception of the request...");
                StreamReader sr = new StreamReader(ns, Encoding.Default);
                response = sr.ReadToEnd();
                Console.WriteLine("Reponse : " + response);

                client.Close();
                sw.Close();
                sr.Close();
                ns.Close();
                Console.WriteLine("Connection closed");
            }
            catch (Exception ex)
            {
                Console.WriteLine("ERROR: " + ex.Message);
            }

            return response;
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

            Parser.IdResponse res = JsonConvert.DeserializeObject<Parser.IdResponse>(RequestToServer(json)); 
            if (res.status == Status.Success)
            {
                uint userId = uint.Parse(res.id);
                connectedUser = new User(userId, username, true);
                Task synchronizationTask = new Task(SynchronizeFields);
                synchronizationTask.Start();
            }
            else
            {
                Console.WriteLine("ERROR: " + res.errorInfo);
                return false;
            }

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
            connectedUser = null;
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

            string s = RequestToServer(json);
            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(s);
            if (res.status == Status.Success)
                users = res.users;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
            
            return users;
        }        

        /// <summary>
        /// Get the list of the users present in a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupUsers(uint groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroupUsers.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<User> users = new List<User>();

            string s = RequestToServer(json);
            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(s);

            if (res.status == Status.Success)
                users = res.users;
            else
                Console.WriteLine("ERROR: " + res.errorInfo);

            return users;

        }

        /// <summary>
        /// Get the list of the pending users for a group
        /// </summary>
        /// <param name="groupId"></param>
        /// <returns></returns>
        public List<User> GetGroupPendingUsers(uint groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetGroupPendingUsers.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);
            List<User> users = new List<User>();

            Parser.ListUsersResponse res = JsonConvert.DeserializeObject<Parser.ListUsersResponse>(RequestToServer(json));
            
            if (res.status == Status.Success)
                users = res.users;
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
            dict.Add("function", ClientFunction.CreateGroup.ToString());
            dict.Add("name", name);
            dict.Add("description", description);
            dict.Add("adminId", connectedUser.id.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.IdResponse res = JsonConvert.DeserializeObject<Parser.IdResponse>(RequestToServer(json));
            Group group = new Group();

            if (res.status == Status.Success)
            {
                uint groupId = uint.Parse(res.id);
                group = new Group(groupId, name, description, connectedUser.id, GroupStatus.IN);

                List<Group> groups = GetGroups();
                groups.Add(group);
            }
            else
                DisplayError("ERROR: " + res.errorInfo);
            
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
        public void JoinGroup(uint groupId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// The loggedIn user leave a group
        /// </summary>
        /// <param name="groupId"></param>
        public void LeaveGroup(uint groupId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Delete the group
        /// </summary>
        /// <param name="groupId"></param>
        public void DeleteGroup(uint groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.DeleteGroup.ToString());
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                DisplayError("ERROR: " + res.errorInfo);
        }        

        /// <summary>
        /// Kick a user from the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void KickUser(uint groupId, uint userId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Promote a user to be admin of the group instead of the current user
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void PromoteUser(uint groupId, uint userId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Invite User to the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="username"></param>
        public void InviteUser(uint groupId, string username)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.InviteUser.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("username", username);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Refuse a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void DeclineRequest(uint groupId, uint userId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Approve a request from a user to join the group
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="userId"></param>
        public void ApproveRequest(uint groupId, uint userId)
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
                DisplayError("ERROR: " + res.errorInfo);
        }


        /********************************************
         *          Files REQUESTS SECTION 
         ********************************************/

        /// <summary>
        /// Create the pending file in the server.
        /// </summary>
        /// <param name="fileName"></param>public static void AddedFileRequest(uint groupId, string filePath, string fileName)
        public static void CreateFile(string fileName)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.CreatePendingFile.ToString());
            dict.Add("name", fileName);

            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Send a file to the server.
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="content"></param>
        public static void SendFile(string fileName, byte[] content)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.SendFile.ToString());
            dict.Add("name", fileName);
            dict.Add("content", string.Join(",", content));
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Tell the server that a certain file was fully transfered.
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="groupId"></param>
        public static void SendFileTransferComplete(string fileName, uint groupId)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.FileTransferComplete.ToString());
            dict.Add("name", fileName);
            dict.Add("groupId", groupId.ToString());
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Tell the server to add a file to the mentionned directory.
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="filePath"></param>
        /// <param name="fileName"></param>
        public static void AddedFileRequest(uint groupId, string filePath, string fileName)
        {
            byte[] data = File.ReadAllBytes(filePath);
            int length = data.Length;
            long maxLength = 100 / sizeof(byte);
            long transmitted = 0;
            CreateFile(fileName);
            while (transmitted != length)
            {
                if (length - transmitted < maxLength - 1)
                {
                    maxLength = length - transmitted;
                }
                byte[] bytesToTransmit = new byte[maxLength];
                Array.Copy(data, transmitted, bytesToTransmit, 0, maxLength);
                transmitted += maxLength;
                SendFile(fileName, bytesToTransmit);
            }
            SendFileTransferComplete(fileName, groupId);
        }

        /// <summary>
        /// Tell the server to rename a file in the mentionned directory.
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="oldFileName"></param>
        /// <param name="newFileName"></param>
        public static void RenamedFileRequest(uint groupId, string oldFileName, string newFileName)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.RenamedFile.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("oldFileName", oldFileName);
            dict.Add("newFileName", newFileName);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Tell the server to delete a file in the mentionned directory.
        /// </summary>
        /// <param name="groupId"></param>
        /// <param name="fileName"></param>
        public static void DeletedFileRequest(uint groupId, string fileName)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.DeletedFile.ToString());
            dict.Add("groupId", groupId.ToString());
            dict.Add("fileName", fileName);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            { }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        public List<string> GetFiles(uint groupId)
        {
            string[] filesName = new string[100];
            List<string> result = new List<string>();

            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.GetFileNames.ToString());
            dict.Add("groupId", groupId.ToString());

            string json = JsonConvert.SerializeObject(dict);

            Parser.ArrayStringResponse res = JsonConvert.DeserializeObject<Parser.ArrayStringResponse>(RequestToServer(json));

            if (res.status == Status.Success)
            {
                filesName = res.names;
            }
            else
                DisplayError("ERROR: " + res.errorInfo);

            foreach (string s in filesName)
                result.Add(s);

            return result;
        }


        /// <summary>
        /// Ask the server the list of missing files.
        /// </summary>
        /// <param name="fileList"></param>
        public static void SendCurrentFileListRequest(Dictionary<string, List<string>> fileList)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.SendCurrentFileListRequest.ToString());
            dict.Add("fileList", string.Join(";", fileList.Select(x => x.Key + "d" + "=" + string.Join(",", x.Value.ToArray())).ToArray()));

            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            {
                List<string> missingFiles = res.missingFiles.Split(',').ToList();
                string groupId = "";
                foreach (string file in missingFiles)
                {
                    if (!string.IsNullOrEmpty(file))
                    {
                        if (file[file.Length - 1] != 'd')
                        {
                            string missingFilePath = groupId + "/" + file;
                            string lastMessage = "start";
                            string fileContent = "";
                            var fs = new FileStream(Group.root + missingFilePath, FileMode.Create, FileAccess.Write);
                            while (true)
                            {
                                lastMessage = DownloadFileRequest(missingFilePath);
                                if(lastMessage == "EndOfFile")
                                {
                                    break;
                                }
                                fileContent += lastMessage;
                            }
                            byte[] toBytes = Encoding.Default.GetBytes(fileContent);
                            Encoding.Convert(Encoding.Default, Encoding.UTF8, toBytes);
                            fs.Write(toBytes, 0, toBytes.Length);
                            fs.Close();
                        }
                        else
                        {
                            groupId = file.Substring(0, file.Length - 1);
                        }
                    }
                    
                }

            }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
        }

        /// <summary>
        /// Ask the server to send the next part of the mentionned file.
        /// </summary>
        /// <param name="filePath"></param>
        public static string DownloadFileRequest(string filePath)
        {
            Dictionary<string, string> dict = new Dictionary<string, string>();
            dict.Add("function", ClientFunction.DownloadFileRequest.ToString());
            dict.Add("filePath", filePath);
            string json = JsonConvert.SerializeObject(dict);

            Parser.Response res = JsonConvert.DeserializeObject<Parser.Response>(RequestToServer(json));

            if (res.status == Status.Success)
            {
                return res.fileParts;
            }
            else
                Console.WriteLine("ERROR: " + res.errorInfo);
                return "";  
        }

        private void SynchronizeFields()
        {
            while (true)
            {
                string root = Group.root;

                Dictionary<string, List<string>> filesList = new Dictionary<string, List<string>>();

                if (!Directory.Exists(root))
                {
                    Directory.CreateDirectory(root);
                }

                foreach (string directory in Directory.GetDirectories(root))
                {
                    List<string> filesInDirectory = new List<string>();
                    foreach (string file in Directory.GetFiles(directory))
                    {
                        filesInDirectory.Add(file.Split('\\').Last());
                    }
                    filesList.Add(directory.Split('/').Last(), filesInDirectory);
                }
                Client.SendCurrentFileListRequest(filesList);
                Thread.Sleep(15000);
            }
        }
    }
}
