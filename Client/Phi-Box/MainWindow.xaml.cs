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

    public enum GroupStatus : int
    {
        OUT,
        PENDING,
        IN
    }
    

    public partial class MainWindow : Window
    {

        public Client client;

        public MainWindow()
        {
            client = new Client();

            InitializeComponent();
            
            //Start at login page
            Navigate(new Login(this));
        }

        public void Navigate(Page page)
        {
            frame.NavigationService.Navigate(page);            
        }
        
        //Don't mind that, need it to change pages
        private void frame_Navigated(object sender, NavigationEventArgs e)
        {

        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if(client.connectedUser != null)
            {
                client.LogOut();
                Navigate(new Login(this));
            }
        }

        
    }
}
