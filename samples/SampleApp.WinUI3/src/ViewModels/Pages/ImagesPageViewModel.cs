using CommunityToolkit.Mvvm.ComponentModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels.Pages
{
    internal partial class ImagesPageViewModel : BaseViewModel
    {
        [ObservableProperty]
        private string? imageName;
    }
}
