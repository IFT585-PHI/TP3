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
    /// Interaction logic for Dashboard.xaml
    /// </summary>
    /// 

    public partial class Dashboard : Page
    {
        private MainWindow mainWindow;

        public Dashboard(MainWindow m)
        {
            mainWindow = m;
            InitializeComponent();
            Update_Dashboard();
        }

        public void Update_Dashboard()
        {
            groups_list.Children.Clear();
            foreach (Group g in mainWindow.Groups)
                Add_Group(g);

            users_list.Children.Clear();
            foreach (User u in mainWindow.Users)
            {
                if(!mainWindow.LoggedInUser.Equals(u))
                    Add_User(u);
            }
        }

        private void Create_Group(object sender, RoutedEventArgs e)
        {
            string groupName = NewGroupName.Text;
            string description = NewGroupDescription.Text;

            if (groupName.Length == 0)
            {
                NewGroupName.Focus();
                return;
            }

            Group group = new Group(groupName, description, (int)GroupStatus.IN);

            mainWindow.AddGroup(group);
            Update_Dashboard();
        }

        private void Add_Group(Group group)
        {
            //<Grid Height="85" Width="720">
            Grid grid = new Grid ();
            grid.Height = 85; grid.Width = 720;


            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });

            // <Label x:Name="label5" Content="Group 1 Name" HorizontalAlignment="Left" Margin="10,10,0,0" VerticalAlignment="Top" Height="38" Width="225" FontSize="20" FontWeight="Bold"/>
            Label label = new Label();
            label.Content = group.name; label.Height = 38; label.Width = 225; label.FontSize = 20;
            label.FontWeight = FontWeights.Bold; label.HorizontalAlignment = HorizontalAlignment.Left;
            label.Margin = new Thickness(10, 10, 0, 0); label.VerticalAlignment = VerticalAlignment.Top;
            grid.Children.Add(label);

            if(group.status != (int)GroupStatus.IN)
            {
                //<Button x:Name="Join_group" Content="Join" Margin="504,14,126,37" FontSize="18"/>
                Button button = new Button();
                button.Name = "Join_group"; button.Content = "Join"; button.Margin = new Thickness(504, 14, 126, 37);
                button.FontSize = 18;

                if (group.status == (int)GroupStatus.PENDING)
                    button.IsEnabled = false;

                grid.Children.Add(button);
            }            

            //<Button x:Name="See_group" Content="Details" Margin="610,14,20,37" FontSize="18"/>
            Button button2 = new Button();
            button2.Name = "See_group"; button2.Content = "Details"; button2.Margin = new Thickness(610, 14, 20, 37);
            button2.FontSize = 18;
            grid.Children.Add(button2);

            //<TextBlock x:Name="textBlock" HorizontalAlignment="Left" Margin="15,54,0,0" TextWrapping="Wrap" Text="" VerticalAlignment="Top" Height="28" Width="685" FontSize="14" />
            TextBlock textBlock2 = new TextBlock();
            textBlock2.Name = "textBlock"; textBlock2.HorizontalAlignment = HorizontalAlignment.Left;
            textBlock2.Text = group.description; textBlock2.FontSize = 14; textBlock2.Height = 28; textBlock2.Width = 685;
            textBlock2.Margin = new Thickness(15, 54, 0, 0); textBlock2.VerticalAlignment = VerticalAlignment.Top;
            grid.Children.Add(textBlock2);

            groups_list.Children.Add(grid);
        }
        private void Add_User(User user)
        {
            //<Grid Height="52">
            Grid grid = new Grid();
            grid.Height = 44; grid.Width = 300;

            //<Border BorderBrush="Gray" BorderThickness="1"/>
            grid.Children.Add(new Border { BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1) });

            //<Ellipse Fill="#FF17FF00" Stroke="Black" Margin="10,0,0,10" Height="32" VerticalAlignment="Bottom" HorizontalAlignment="Left" Width="32"/>
            Ellipse ell = new Ellipse();
            ell.Fill = Brushes.Lime; ell.Margin = new Thickness(10, 0, 0, 10); ell.Height = 25; ell.Width = 25;
            ell.VerticalAlignment = VerticalAlignment.Bottom; ell.HorizontalAlignment = HorizontalAlignment.Left;
            ell.Stroke = Brushes.Black;
            grid.Children.Add(ell);

            //<Label x:Name="label6" Content="Username 1" HorizontalAlignment="Left" Margin="61,10,0,10" Width="229" VerticalContentAlignment="Center" FontSize="14"/>
            Label label = new Label();
            label.Content = user.username; label.Height = 30; label.Width = 229; label.FontSize = 16;
            label.HorizontalAlignment = HorizontalAlignment.Left; label.VerticalAlignment = VerticalAlignment.Center;
            label.Margin = new Thickness(50, 10, 0, 10); label.Name = "label";
            grid.Children.Add(label);

            users_list.Children.Add(grid);
        }
    }
}
