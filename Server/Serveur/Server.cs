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

        static public void receiveFiles()
        {

        }

        static public void sendFile(File file)
        {

        }

        static public void Synchronize()
        {
            userManager.Synchronize();
            loginManager.Synchronize();
            //...
        }

        static public GroupManager getGroupManager()
        {
            return groupManager;
        }

        static public LoginManager getLoginManager()
        {
            return loginManager;
        }

        static public UserManager getUserManager()
        {
            return userManager;
        }
    }
}
