using CommunityToolkit.Mvvm.DependencyInjection;
using Microsoft.UI.Xaml.Controls;
using SampleApp.WinUI3.ViewModels;

namespace SampleApp.WinUI3.Views
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class AudioPage : Page
    {
        public AudioPage()
        {
            this.InitializeComponent();

            this.DataContext = Ioc.Default.GetRequiredService<AudioPageViewModel>();
        }
    }
}
