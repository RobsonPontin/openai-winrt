// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using CommunityToolkit.Mvvm.DependencyInjection;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.ViewModels;

namespace SampleApp.WinUI3.Views
{
    public sealed partial class ModelsPage : Page
    {
        public ModelsPage()
        {
            this.InitializeComponent();

            this.DataContext = ViewModel = Ioc.Default.GetRequiredService<ModelsPageViewModel>();
        }

        private ModelsPageViewModel ViewModel { get; set; }

        private async void btnGetModels_Click(object sender, RoutedEventArgs e)
        {
            await ViewModel.RequestModelsAsync();
        }
    }
}
