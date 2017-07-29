using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Phi_Box
{
    public class Group
    {
        public uint id;
        public string name;
        public string description;
        public uint adminId;
        public GroupStatus status;

        public Group() { }

        public Group(uint _id, string _name, string _description, uint _adminId, GroupStatus _status)
        {
            id = _id;
            name = _name;
            description = _description;
            adminId = _adminId;
            status = _status;
        }
    }
}
