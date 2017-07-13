﻿using System;
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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            //Start a login page
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
    }
}
