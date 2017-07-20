using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class Group
    {

        public string name;
        public string description;
        public int status;

        public Group(string n, string d, int s)
        {
            name = n;
            description = d;
            status = s;
        }
        
    }
}
