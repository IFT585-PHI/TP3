using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Phi_Box
{
    public class User
    {
        [JsonProperty("userId")]
        public uint id;
        [JsonProperty("username")]
        public string username;
        [JsonProperty("isConnected")]
        public bool isConnected;

        public User(uint _id, string _username, bool _isConnected)
        {
            id = _id;
            username = _username;
            isConnected = _isConnected;
        }

    }
}
