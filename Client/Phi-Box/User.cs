using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class User
    {
        public int id;
        public string username;
        public bool isOnline;      //Status: 0 Offline, 1 Online

        public User(int _id, string _username)
        {
            id = _id;
            username = _username;
        }

        public User(int _id, string _username, bool _status)
        {
            id = _id;
            username = _username;
            isOnline = _status;
        }

    }
}
