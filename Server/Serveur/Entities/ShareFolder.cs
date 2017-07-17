using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class ShareFolder : Entity
    {
        String _root;

        Dictionary<File, int> files; // int = version

        public ShareFolder(int id, string root):
            base(id)
        {
            _root = root;
        }
    }
}
