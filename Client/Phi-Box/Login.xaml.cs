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
            int password = passwordBox.GetHashCode();

            if (username.Length == 0)
            {
                errormessage.Text = "Enter a username.";
                textBoxUsername.Focus();
                return;
            }
            else if (passwordBox.Password.Length == 0)
            {
                errormessage.Text = "Enter password.";
                passwordBox.Focus();
                return;
            }


            if (IsLogging)
            {
                errormessage.Text = "User is logging";
                //Logging process here
            }
            else
            {
                errormessage.Text = "User is registering";
                //Register process here
            }
            mainWindow.Navigate(new Dashboard(mainWindow));
        }

        private void ResetField()
        {
            textBoxUsername.Text = "";
            passwordBox.Password = "";
            errormessage.Text = "";
        }
    }
}
