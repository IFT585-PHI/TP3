using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    struct FileName
    {
        public string name;
        public string extension;

        public string GetFileName()
        {
            return name + "." + extension;
        }
    }
    class File : Entity
    {
        FileName _fileName;
        String _path;
        int _version = 1;

        public File(string name, string extension, string path, int id)
            : base(id)
        {
            _fileName.name = name;
            _fileName.extension = extension;
            _path = path;
        }
    }
}
