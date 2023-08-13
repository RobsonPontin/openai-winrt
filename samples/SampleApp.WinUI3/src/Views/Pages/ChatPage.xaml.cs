using CommunityToolkit.Mvvm.DependencyInjection;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.ViewModels;

namespace SampleApp.WinUI3.Views
{
    internal sealed partial class ChatPage : Page
    {
        public ChatPage()
        {
            this.InitializeComponent();

            this.DataContext = Ioc.Default.GetRequiredService<ChatPageViewModel>();
        }
    }
}
