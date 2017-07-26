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

        private MainWindow mainWindow;

        public GroupView(MainWindow m)
        {
            mainWindow = m;
            InitializeComponent();
        }



        private void AddUser(User user)
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
