using CommunityToolkit.Mvvm.ComponentModel;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.Views;
using System.Collections.ObjectModel;

namespace SampleApp.WinUI3.ViewModels
{
    internal partial class MainPageViewModel : BaseViewModel
    {
        public MainPageViewModel()
        {
            Title = "OpenAI Samples";
            NavigationViewItems = new ObservableCollection<NavigationViewItem>
            {
                // Create Image
                new NavigationViewItem
                {
                    Icon = new SymbolIcon((Symbol)0xE158),
                    Content = "Images",
                    Tag = typeof(ImagesPage)
                },

                // Models
                new NavigationViewItem
                {
                    Icon = new SymbolIcon((Symbol)0xE14C),
                    Content = "Models",
                    Tag = typeof(ModelsPage)
                },

                // Chat
                new NavigationViewItem
                {
                    Icon = new SymbolIcon((Symbol)0xE15F),
                    Content = "Chat",
                    Tag = typeof(ChatPage)
                }
            };
        }

        [ObservableProperty]
        private string? title;

        [ObservableProperty]
        ObservableCollection<NavigationViewItem> navigationViewItems;
    }
}
