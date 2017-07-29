using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class User
    {
        public uint id;
        public string username;
        public bool isConnected;

        public User(uint _id, string _username, bool _isConnected)
        {
            id = _id;
            username = _username;
            isConnected = _isConnected;
        }

    }
}
