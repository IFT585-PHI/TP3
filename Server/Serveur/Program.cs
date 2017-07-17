using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace Serveur
{
    class Program
    {
        static void Main(string[] args)
        {
            Server.InitializeManager();

            while (true)
            {
                Server.Synchronize();
                System.Threading.Thread.Sleep(15000);
            }
        }
    }
}
