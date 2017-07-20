using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Navigation;
using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;
using Newtonsoft.Json.Linq;

namespace Phi_Box
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    enum GroupStatus : int
    {
        OUT,
        PENDING,
        IN
    }

    public partial class MainWindow : Window
    {

        public List<Group> Groups;
        public List<User> Users;

        public User LoggedInUser;


        public MainWindow()
        {
            Update_Lists();

            InitializeComponent();

            //Start at login page
            Navigate(new Login(this));
        }

        public void Navigate(Page page)
        {
            frame.NavigationService.Navigate(page);
        }

        public void Update_Lists()
        {
            //Update Groups list
            GetGroups();

            //Update Users list
            GetUsers();

        }


        //Don't mind that, need it to change pages
        private void frame_Navigated(object sender, NavigationEventArgs e)
        {

        }

        public void GetGroups()
        {
            Groups = new List<Group>();
            JArray a = JArray.Parse(File.ReadAllText("../../groups.json"));
            foreach (JObject o in a.Children<JObject>())
            {
                string name = o.First.Last.ToString();
                string description = o.First.Next.Last.ToString();
                int status = (int)o.Last.Last;
                Groups.Add(new Group(name, description, status));
            }
        }
        public void GetUsers()
        {
            Users = new List<User>();
            JArray a =  JArray.Parse(File.ReadAllText("../../users.json"));
            foreach (JObject o in a.Children<JObject>())
            {
                string name = o.First.Last.ToString();
                string password = o.Last.Last.ToString();
                Users.Add(new User(name, password));
            }
        }
        public void AddGroup(Group group)
        {
            //TODO: SEND GROUP TO SERVER
            GetGroups();
            //TEMPORARY ADD IT HERE, USUALLY GetGroups will have the new one
            Groups.Add(group);
            string json = JsonConvert.SerializeObject(Groups.ToArray());
            System.IO.File.WriteAllText("../../groups.json", json);
        }
        public void AddUser(User user)
        {
            //TODO: SEND Users TO SERVER
            GetUsers();
            //TEMPORARY ADD IT HERE, USUALLY GetUsers will have the new one
            Users.Add(user);
            string json = JsonConvert.SerializeObject(Users.ToArray());
            System.IO.File.WriteAllText("../../users.json", json);
        }


    }
}
