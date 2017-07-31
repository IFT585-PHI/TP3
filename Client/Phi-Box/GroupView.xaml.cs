using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for GroupView.xaml
    /// </summary>
    public partial class GroupView : Page
    {

        private uint groupId;
        private bool isAdmin;

        private MainWindow mainWindow;

        public GroupView(MainWindow m, uint id, uint adminId)
        {
            mainWindow = m;
            groupId = id;
            isAdmin = adminId == mainWindow.client.connectedUser.id;

            InitializeComponent();

            DisplayGroupUsers();
            DisplayFiles();

            if (isAdmin)
                DisplayAdminView();
            else
                DisplayUserView();

        }

        private void DisplayAdminView()
        {
            DisplayPendingUsers();
        }
        private void DisplayUserView()
        {
            Users.Children.Remove(PendingSection);
            UsersScroll.Height = 535;
            delete.Click += new RoutedEventHandler(Leave_Click);
            delete.Click -= new RoutedEventHandler(Delete_Click);
            delete.Content = "Leave Group";
        }
        

        private void AddGroupUser(User user)
        {
            //<Grid Height="55" Width="300" VerticalAlignment="Top">
            Grid grid = new Grid();
            grid.Height = 55; grid.Width = 300; grid.VerticalAlignment = VerticalAlignment.Top;

            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });

            //<Ellipse Fill="#FF17FF00" Stroke="Black" Margin="10,12,0,11" Height="32" VerticalAlignment="Center" HorizontalAlignment="Left" Width="32"/>
            Ellipse ell = new Ellipse();
            ell.Fill = (user.isConnected) ? Brushes.Lime : Brushes.Red; ell.Margin = new Thickness(10, 0, 0, 10); ell.Height = 25; ell.Width = 25;
            ell.VerticalAlignment = VerticalAlignment.Bottom; ell.HorizontalAlignment = HorizontalAlignment.Left;
            ell.Stroke = Brushes.Black;
            grid.Children.Add(ell);

            //<Label x:Name="label6" Content="Username 1" HorizontalAlignment="Left" Margin="49,0,0,0" Width="251" FontSize="16" VerticalAlignment="Top"/>
            Label label = new Label();
            label.Content = user.username; label.Height = 30; label.Width = 229; label.FontSize = 16;
            label.HorizontalAlignment = HorizontalAlignment.Left; label.VerticalAlignment = VerticalAlignment.Top;
            label.Margin = new Thickness(49, 0, 0, 0); label.Name = "label";
            grid.Children.Add(label);

            if (isAdmin)
            {
                //<Button x:Name="Prmote" Content="Promote" Margin="56,31,160,3" Click="Promote_Click"/>
                Button button = new Button();
                button.Name = "Promote"; button.Content = "Promote"; button.Margin = new Thickness(56, 31, 160, 3);
                button.Click += new RoutedEventHandler(Promote_Click); button.DataContext = user.id;
                grid.Children.Add(button);

                //<Button x:Name="Remove" Content="Remove" Margin="175,31,41,3" Click="Remove_Click"/>
                Button button2 = new Button();
                button2.Name = "Remove"; button2.Content = "Remove"; button2.Margin = new Thickness(175, 31, 41, 3);
                button2.Click += new RoutedEventHandler(Remove_Click); button2.DataContext = user.id;
                grid.Children.Add(button2);
            }

            users_list.Children.Add(grid);
        }
        private void AddPendingUser(User user)
        {

            //<Grid Height="55" Width="300" VerticalAlignment="Top">
            Grid grid = new Grid();
            grid.Height = 55; grid.Width = 290; grid.VerticalAlignment = VerticalAlignment.Top;
            grid.HorizontalAlignment = HorizontalAlignment.Left;

            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });            

            //<Label x:Name="label6" Content="Username 1" HorizontalAlignment="Left" Margin="49,0,0,0" Width="251" FontSize="16" VerticalAlignment="Top"/>
            Label label = new Label();
            label.Content = user.username; label.Height = 30; label.Width = 229; label.FontSize = 16;
            label.HorizontalAlignment = HorizontalAlignment.Left; label.VerticalAlignment = VerticalAlignment.Top;
            label.Margin = new Thickness(10, 0, 0, 0); label.Name = "label";
            grid.Children.Add(label);

            //<Button x:Name="Prmote" Content="Promote" Margin="56,31,160,3" Click="Promote_Click"/>
            Button button = new Button();
            button.Name = "Approve"; button.Content = "Approve"; button.Margin = new Thickness(56, 31, 160, 3);
            button.Click += new RoutedEventHandler(Approve_Click); button.DataContext = user.id;
            grid.Children.Add(button);

            //<Button x:Name="Remove" Content="Remove" Margin="175,31,41,3" Click="Remove_Click"/>
            Button button2 = new Button();
            button2.Name = "Decline"; button2.Content = "Decline"; button2.Margin = new Thickness(175, 31, 41, 3);
            button2.Click += new RoutedEventHandler(Decline_Click); button2.DataContext = user.id;
            grid.Children.Add(button2);

            pending_list.Children.Add(grid);
        }
        private void AddFile(PhiFile file)
        {
            //<Grid Height="68" Width="718">
            Grid grid = new Grid();
            grid.Height = 68; grid.Width = 718; grid.VerticalAlignment = VerticalAlignment.Top;

            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });

            //<Label Content="File name 1.txt" HorizontalAlignment="Left" Margin="52,0,0,0" VerticalAlignment="Center" FontSize="22"/>
            Label label = new Label();
            label.Content = file.name; label.FontSize = 22;
            label.HorizontalAlignment = HorizontalAlignment.Left; label.VerticalAlignment = VerticalAlignment.Center;
            label.Margin = new Thickness(52, 0, 0, 0);
            grid.Children.Add(label);

            files_list.Children.Add(grid);
        }

        private void DisplayGroupUsers()
        {
            users_list.Children.Clear();
            foreach (User u in mainWindow.client.GetGroupUsers(groupId))
            {
                if (u.id != mainWindow.client.connectedUser.id)
                    AddGroupUser(u);
            }
        }
        private void DisplayPendingUsers()
        {
            pending_list.Children.Clear();
            foreach (User u in mainWindow.client.GetGroupPendingUsers(groupId))
            {
                if (u.id != mainWindow.client.connectedUser.id)
                    AddPendingUser(u);
            }
        }
        private void DisplayFiles()
        {
            files_list.Children.Clear();
            Console.WriteLine(Directory.GetCurrentDirectory());
            foreach (PhiFile u in mainWindow.client.GetFiles(groupId))
            {
                AddFile(u);
            }
        }

        private void Promote_Click(object sender, RoutedEventArgs e)
        {
            uint userId = (uint)((Button)sender).DataContext;
            mainWindow.client.PromoteUser(groupId, userId);
            mainWindow.Navigate(new GroupView(mainWindow, groupId, userId));
        }
        private void Remove_Click(object sender, RoutedEventArgs e)
        {
            uint userId = (uint)((Button)sender).DataContext;
            mainWindow.client.KickUser(groupId, userId);
            DisplayGroupUsers();
        }
        private void Approve_Click(object sender, RoutedEventArgs e)
        {
            uint userId = (uint)((Button)sender).DataContext;
            mainWindow.client.ApproveRequest(groupId, userId);
            DisplayGroupUsers();
            DisplayPendingUsers();
        }
        private void Decline_Click(object sender, RoutedEventArgs e)
        {
            uint userId = (uint)((Button)sender).DataContext;
            mainWindow.client.DeclineRequest(groupId, userId);
            DisplayGroupUsers();
            DisplayPendingUsers();
        }

        private void Add_Member_Click(object sender, RoutedEventArgs e)
        {
            string username = AddMember.Text;
            mainWindow.client.InviteUser(groupId, username);
            DisplayGroupUsers();
            DisplayPendingUsers();
        }

        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            mainWindow.client.LogOut();
            mainWindow.Navigate(new Login(mainWindow));
        }

        private void Prev_Click(object sender, RoutedEventArgs e)
        {
            mainWindow.Navigate(new Dashboard(mainWindow));
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            mainWindow.client.DeleteGroup(groupId);
            mainWindow.Navigate(new Dashboard(mainWindow));
        }
        private void Leave_Click(object sender, RoutedEventArgs e)
        {
            mainWindow.client.LeaveGroup(groupId);
            mainWindow.Navigate(new Dashboard(mainWindow));
        }
    }
}
