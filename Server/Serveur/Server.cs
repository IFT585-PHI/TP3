using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Serveur
{
    static class Server
    {
        static public string address = "127.0.0.1";

        static GroupManager groupManager;
        static LoginManager loginManager;
        static UserManager userManager;
        
        static public void InitializeManager()
        {
            groupManager = new GroupManager();
            loginManager = new LoginManager();
            userManager = new UserManager();

            groupManager.Initialize();
            loginManager.Initialize();
            userManager.Initialize();
        }

        static public void ReceiveFiles()
        {

        }

        static public void SendFile(File file)
        {

        }

        static public void Synchronize()
        {
            userManager.Synchronize();
            loginManager.Synchronize();
            //...
        }

        static public GroupManager GetGroupManager()
        {
            return groupManager;
        }

        static public LoginManager GetLoginManager()
        {
            return loginManager;
        }

        static public UserManager GetUserManager()
        {
            return userManager;
        }
    }
}
