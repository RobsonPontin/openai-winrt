using CommunityToolkit.Mvvm.ComponentModel;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.ViewModels;
using SampleApp.WinUI3.Views.Pages;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels
{
    internal partial class MainPageViewModel : BaseViewModel
    {
        public MainPageViewModel()
        {
            Title = "OpenAI Samples";
            NavigationViewItems = new ObservableCollection<NavigationViewItem>();

            // Create Image
            NavigationViewItems.Add(new NavigationViewItem
            {
                Icon = new SymbolIcon((Symbol)0xE158),
                Content = "Images",
                Tag = typeof(ImagesPage)
            });

            // Models
            NavigationViewItems.Add(new NavigationViewItem
            {
                Icon = new SymbolIcon((Symbol)0xE14C),
                Content = "Models",
                Tag = typeof(ModelsPage)
            });
        }

        [ObservableProperty]
        private string? title;

        [ObservableProperty]
        ObservableCollection<NavigationViewItem> navigationViewItems;
    }
}
