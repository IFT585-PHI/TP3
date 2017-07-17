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

        Dictionary<File, int> _files; // int = version

        public ShareFolder(int id, string root)
            : base(id)
        {
            _root = root;
            _files = new Dictionary<File, int>();
        }

        public ShareFolder(int id, string root, Dictionary<File, int> files)
            : base(id)
        {
            _root = root;
            _files = files;
        }
    }
}
