using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System;
using System.Net.Sockets;
using System.IO;
using System.Text;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net;
using System.Net.Cache;
using Newtonsoft.Json;
using System.Collections.Generic;

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
            textBoxUsername.Focus();
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

        private void Submit_Click_Transfert_Test(object sender, RoutedEventArgs e)
        {
            string path = "C:/Users/Shocky/devoir_4 (1).pdf";
            byte[] data = File.ReadAllBytes(path);
            int length = data.Length;

            Client client = new Client();
            string fileName = GetFileName(path);
            long maxLength = 100 / sizeof(byte);
            long transmitted = 0;
            client.CreateFile(fileName);

            while (transmitted != length)
            {
                if(length - transmitted < maxLength - 1)
                {
                    maxLength = length - transmitted;
                }
                byte[] bytesToTransmit = new byte[maxLength];
                Array.Copy(data, transmitted, bytesToTransmit, 0, maxLength);
                transmitted += maxLength;
                client.SendFile(fileName, bytesToTransmit);
            }
            client.SendFileTransferComplete(fileName, 123);
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
        

    }
}
