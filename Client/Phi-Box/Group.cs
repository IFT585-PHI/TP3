using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class Group
    {
        private int id { get; }

        public string name;
        public string description;
        public int status;

        public Group() { }

        public Group(int i, string n, string d, int s)
        {
            id = i;
            name = n;
            description = d;
            status = s;
        }

        //TEMPORARY (Real one is the one with ID)
        public Group(string n, string d, int s)
        {
            name = n;
            description = d;
            status = s;
        }

    }
}
