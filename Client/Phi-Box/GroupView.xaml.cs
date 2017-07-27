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
    /// Interaction logic for GroupView.xaml
    /// </summary>
    public partial class GroupView : Page
    {
        private int groupId;
        private bool isAdmin;

        private MainWindow mainWindow;

        public GroupView(MainWindow m, int id)
        {
            mainWindow = m;
            groupId = id;
            InitializeComponent();
            isAdmin = true;
            DisplayGroupUsers();
            DisplayPendingUsers();
        }



        private void AddGroupUser(User user)
        {
            /*
            <Grid Height="55" Width="300" VerticalAlignment="Top">
                <Border BorderBrush="Gray" BorderThickness="1" />
                <Ellipse Fill="#FF17FF00" Stroke="Black" Margin="10,12,0,11" Height="32" VerticalAlignment="Center" HorizontalAlignment="Left" Width="32"/>
                <Label x:Name="label6" Content="Username 1" HorizontalAlignment="Left" Margin="49,0,0,0" Width="251" FontSize="16" VerticalAlignment="Top"/>
                <Button x:Name="Prmote" Content="Promote" Margin="56,31,160,3" Click="Promote_Click"/>
                <Button x:Name="Kick" Content="Remove" Margin="175,31,41,3" Click="Remove_Click"/>
            </Grid>

            */

            //<Grid Height="55" Width="300" VerticalAlignment="Top">
            Grid grid = new Grid();
            grid.Height = 55; grid.Width = 300; grid.VerticalAlignment = VerticalAlignment.Top;

            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });

            //<Ellipse Fill="#FF17FF00" Stroke="Black" Margin="10,12,0,11" Height="32" VerticalAlignment="Center" HorizontalAlignment="Left" Width="32"/>
            Ellipse ell = new Ellipse();
            ell.Fill = Brushes.Lime; ell.Margin = new Thickness(10, 0, 0, 10); ell.Height = 25; ell.Width = 25;
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
                button.Click += new RoutedEventHandler(Promote_Click);
                grid.Children.Add(button);

                //<Button x:Name="Remove" Content="Remove" Margin="175,31,41,3" Click="Remove_Click"/>
                Button button2 = new Button();
                button2.Name = "Remove"; button2.Content = "Remove"; button2.Margin = new Thickness(175, 31, 41, 3);
                button2.Click += new RoutedEventHandler(Remove_Click);
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
            button.Click += new RoutedEventHandler(Approve_Click);
            grid.Children.Add(button);

            //<Button x:Name="Remove" Content="Remove" Margin="175,31,41,3" Click="Remove_Click"/>
            Button button2 = new Button();
            button2.Name = "Decline"; button2.Content = "Decline"; button2.Margin = new Thickness(175, 31, 41, 3);
            button2.Click += new RoutedEventHandler(Decline_Click);
            grid.Children.Add(button2);

            pending_list.Children.Add(grid);
        }

        private void DisplayGroupUsers()
        {
            foreach (User g in mainWindow.client.GetGroupUsers(groupId))
            {
                AddGroupUser(g);
            }
        }
        private void DisplayPendingUsers()
        {
            foreach (User g in mainWindow.client.GetGroupPendingUsers(groupId))
            {
                AddPendingUser(g);
            }
        }




        private void DisplayFiles()
        {

        }

        private void Promote_Click(object sender, RoutedEventArgs e)
        {

        }
        private void Remove_Click(object sender, RoutedEventArgs e)
        {

        }
        private void Approve_Click(object sender, RoutedEventArgs e)
        {

        }
        private void Decline_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
