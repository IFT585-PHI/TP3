using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    class Entity
    {
        protected int _id;

        protected Entity(int id)
        {
            _id = id;
        }

        int getId()
        {
            return _id;
        }
    }
}
