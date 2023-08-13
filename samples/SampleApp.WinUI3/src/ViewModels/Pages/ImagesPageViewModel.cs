using CommunityToolkit.Mvvm.ComponentModel;

namespace SampleApp.WinUI3.ViewModels
{
    internal partial class ImagesPageViewModel : BaseViewModel
    {
        [ObservableProperty]
        private string? imageName;

        public ImagesPageViewModel(OpenAI.OpenAiService openAiService)
            : base(openAiService)
        {
        }
    }
}
