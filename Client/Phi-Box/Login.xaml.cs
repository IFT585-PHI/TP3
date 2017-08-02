using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

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
            textBoxIP.Text = Client.ipAddresse;
            textBoxUsername.Focus();
            Task synchronizationTask = new Task(SynchronizeFields);
            synchronizationTask.Start();
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
            else if (textBoxIP.Text.Length == 0)
            {
                errormessage.Text = "Enter the server Ip address.";
                passwordBox.Focus();
                return;
            }

            Client.ipAddresse = textBoxIP.Text;

            if (IsLogging)
            {
                if (!mainWindow.client.LogIn(username, password)){
                    errormessage.Text = "Wrong password and/or username.";
                    return;
                }
            }
            else
            {
                if (!mainWindow.client.Register(username, password)){
                    errormessage.Text = "This username has already been taken.";
                    return;
                }
            }
            
            mainWindow.Navigate(new Dashboard(mainWindow));
        }

        private void ResetField()
        {
            textBoxUsername.Text = "";
            passwordBox.Password = "";
            errormessage.Text = "";
        }

        private string GetFileName(string path)
        {
            var words = path.Split('/','\\');
            return words[words.Length - 1];
        }

        private void SynchronizeFields()
        {
            while (true)
            {
                string root = Group.root;

                Dictionary<string, List<string>> filesList = new Dictionary<string, List<string>>();

                if (!Directory.Exists(root))
                {
                    Directory.CreateDirectory(root);
                }

                foreach (string directory in Directory.GetDirectories(root)){
                    List<string> filesInDirectory = new List<string>();
                    foreach (string file in Directory.GetFiles(directory)){
                        filesInDirectory.Add(file.Split('/').Last());
                    }
                   filesList.Add(directory.Split('/').Last(), filesInDirectory);
                }
                Client.SendCurrentFileListRequest(filesList);
                Thread.Sleep(15000);
            }
        }
    }
}
