using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Phi_Box
{
    public class Group
    {
        [JsonProperty("groupId")]
        public uint id;
        [JsonProperty("name")]
        public string name;
        [JsonProperty("description")]
        public string description;
        [JsonProperty("adminId")]
        public uint adminId;
        public GroupStatus status;

        FileSystemWatcher fsw;
        string root = "C:/Phibox/";

        public Group() { }

        public Group(uint _id, string _name, string _description, uint _adminId, GroupStatus _status)
        {
            id = _id;
            name = _name;
            description = _description;
            adminId = _adminId;
            status = _status;
            adminId = _adminId;

            initFileWatcher();
        }

        private void initFileWatcher()
        {
            if(!Directory.Exists(root + name))
            {
                if (!Directory.Exists(root))
                {
                    Directory.CreateDirectory(root);
                }

                Directory.CreateDirectory(root + name);
            }

            fsw = new FileSystemWatcher();
            fsw.Path = root + name;
            fsw.NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.FileName;
            fsw.Filter = "*.*";
            fsw.Created += new FileSystemEventHandler(OnCreated);
            fsw.Changed += new FileSystemEventHandler(OnChanged);
            fsw.Deleted += new FileSystemEventHandler(OnDeleted);
            fsw.Renamed += new RenamedEventHandler(OnRenamed);
            fsw.EnableRaisingEvents = true;
        }

        private void OnChanged(object source, FileSystemEventArgs e)
        {
            Client.AddedFileRequest(id, e.FullPath, e.Name);
        }

        private void OnCreated(object source, FileSystemEventArgs e)
        {
            Client.AddedFileRequest(id, e.FullPath, e.Name);
        }

        private void OnDeleted(object source, FileSystemEventArgs e)
        {
            Client.DeletedFileRequest(id, e.Name);
        }

        private void OnRenamed(object source, RenamedEventArgs e)
        {
            Client.RenamedFileRequest(id, e.OldName, e.Name);
        }

        public bool IsValid()
        {
            return name != null;
        }
    }
}
