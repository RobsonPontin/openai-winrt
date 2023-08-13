// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using CommunityToolkit.Mvvm.DependencyInjection;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.Services;
using SampleApp.WinUI3.Views;

namespace SampleApp.WinUI3
{
    /// <summary>
    /// Provides application-specific behavior to supplement the default Application class.
    /// </summary>
    public partial class App : Application
    {
        private Window m_window;

        /// <summary>
        /// Initializes the singleton application object.  This is the first line of authored code
        /// executed, and as such is the logical equivalent of main() or WinMain().
        /// </summary>
        public App()
        {
            this.InitializeComponent();

            // Dependency Injection: Configure all registered services.
            Ioc.Default.ConfigureServices(new MainServiceProvider());
        }

        public Window MainWindow { get { return m_window; } }

        /// <summary>
        /// Invoked when the application is launched.
        /// </summary>
        /// <param name="args">Details about the launch request and process.</param>
        protected override void OnLaunched(Microsoft.UI.Xaml.LaunchActivatedEventArgs args)
        {
            m_window = new Window();
         
            Frame rootFrame = new Frame();
            m_window.Content = rootFrame;
            
            rootFrame.Navigate(typeof(MainPage));
            m_window.Activate();
        }
    }
}
