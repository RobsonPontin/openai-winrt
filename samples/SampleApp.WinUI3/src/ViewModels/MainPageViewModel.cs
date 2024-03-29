﻿using CommunityToolkit.Mvvm.ComponentModel;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.Views;
using System.Collections.ObjectModel;

namespace SampleApp.WinUI3.ViewModels
{
    internal partial class MainPageViewModel : BaseViewModel
    {
        public MainPageViewModel(OpenAI.OpenAiService openAiService)
            : base(openAiService)
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
                },

                // Audio
                new NavigationViewItem
                {
                    Icon = new SymbolIcon((Symbol)0xE189),
                    Content = "Audio",
                    Tag = typeof(AudioPage)
                },

                // PDF
                new NavigationViewItem
                {
                    Icon = new SymbolIcon((Symbol)0xE130),
                    Content = "PDF",
                    Tag = typeof(PdfPage)
                }
            };
        }

        [ObservableProperty]
        private string? title;

        [ObservableProperty]
        ObservableCollection<NavigationViewItem> navigationViewItems;
    }
}
