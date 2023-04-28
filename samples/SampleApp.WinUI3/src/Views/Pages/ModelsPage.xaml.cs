// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Controls.Primitives;
using Microsoft.UI.Xaml.Data;
using Microsoft.UI.Xaml.Input;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Navigation;
using SampleApp.WinUI3.Services;
using SampleApp.WinUI3.ViewModels.Pages;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;

namespace SampleApp.WinUI3.Views.Pages
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ModelsPage : Page
    {
        public ModelsPage()
        {
            this.InitializeComponent();

            this.DataContext = ViewModel = new ModelsPageViewModel();
        }

        private ModelsPageViewModel ViewModel { get; set; }

        private async void btnGetModels_Click(object sender, RoutedEventArgs e)
        {
            await ViewModel.RequestModelsAsync();
        }
    }
}
