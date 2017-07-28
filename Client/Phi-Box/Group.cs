using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class Group
    {
        public int id;
        public string name;
        public string description;
        public int adminId;
        public GroupStatus status;

        public Group() { }

        public Group(int _id, string _name, string _description, int _adminId, GroupStatus _status)
        {
            id = _id;
            name = _name;
            description = _description;
            adminId = _adminId;
            status = _status;
        }
    }
}
