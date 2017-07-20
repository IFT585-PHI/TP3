using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net;
using System.Net.Cache;
using System.IO;

namespace Phi_Box
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    public partial class Login : Page
    {

        private MainWindow mainWindow;

        private bool IsLogging = true;

        public Login(MainWindow m)
        {
            mainWindow = m;
            InitializeComponent();
        }
        

        private void Enter_Shortcut(object sender, RoutedEventArgs e)
        {
            if (Keyboard.IsKeyDown(Key.Enter))
                Submit_Click(sender, e);
        }

        private void Register_Click(object sender, RoutedEventArgs e)
        {
            ResetField();

            if (IsLogging)
            {
                IsLogging = false;
                Submit.Content = "Register";
                RegisterLink.Content = "Login";
            }
            else
            {
                IsLogging = true;
                Submit.Content = "Login";
                RegisterLink.Content = "Register";
            }
        }

        private void Submit_Click(object sender, RoutedEventArgs e)
        {
            errormessage.Text = "";

            string username = textBoxUsername.Text;
            string password = passwordBox.Password;

            if (username.Length == 0)
            {
                errormessage.Text = "Enter a username.";
                textBoxUsername.Focus();
                return;
            }
            else if (passwordBox.Password.Length == 0)
            {
                errormessage.Text = "Enter a password.";
                passwordBox.Focus();
                return;
            }

            User currUser = new User(username, password);

            if (IsLogging)
            {
                if (!Log_In_User(currUser)) return;
                //TODO: Tell server user is loggedIn
            }
            else
            {
                if (!Register_User(currUser)) return;
                mainWindow.AddUser(currUser);
                //TODO: Tell server there's a new user, and he's loggedIn
            }

            mainWindow.LoggedInUser = currUser;
            mainWindow.Navigate(new Dashboard(mainWindow));
        }

        private bool Log_In_User(User user)
        {
            bool userIsPresent = false;
            bool success = false;

            foreach (User u in mainWindow.Users)
            {
                if(u.username == user.username)
                {
                    userIsPresent = true;
                    errormessage.Text = "Wrong password!";

                    if (u.password == user.password)
                        success = true;

                    break;
                }
            }

            if(!userIsPresent)
                errormessage.Text = "User doesn't exist!";

            return success;
        }
        private bool Register_User(User user)
        {
            bool userExist = false;
            bool success = true;

            foreach (User u in mainWindow.Users)
            {
                if (u.username == user.username)
                {
                    userExist = true;
                    success = false;
                    errormessage.Text = "Username already exist!";

                    break;
                }
            }

            if (!userExist)
                mainWindow.Users.Add(user); 

            return success;
        }

        private void ResetField()
        {
            textBoxUsername.Text = "";
            passwordBox.Password = "";
            errormessage.Text = "";
        }
        

    }
}
