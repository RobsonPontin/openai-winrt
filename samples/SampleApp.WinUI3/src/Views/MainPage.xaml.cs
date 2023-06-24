// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.ViewModels;
using System;

namespace SampleApp.WinUI3.Views
{
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            this.DataContext = new MainPageViewModel();
        }

        private void navigationView_ItemInvoked(NavigationView sender, NavigationViewItemInvokedEventArgs args)
        {
            var item = args?.InvokedItemContainer;
            if (item != null) 
            {
                var pageType = item.Tag as Type;

                if (navigationFrame.CurrentSourcePageType == pageType)
                {
                    return;
                }

                navigationFrame.Navigate(pageType);
            }
        }
    }
}
