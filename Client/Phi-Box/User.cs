﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class User
    {
        public string username;
        public string password;

        public User(string u, string p)
        {
            username = u;
            password = p;
        }
        
        public bool Equals(User user)
        {
            if (this.username == user.username && this.password == user.password)
                return true;
            return false;
        }


    }
}
