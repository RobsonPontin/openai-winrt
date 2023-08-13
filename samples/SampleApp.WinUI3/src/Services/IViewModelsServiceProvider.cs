using SampleApp.WinUI3.ViewModels;
using Jab;

namespace SampleApp.WinUI3.Services
{
    [ServiceProviderModule]
    [Transient(typeof(MainPageViewModel))]
    [Transient(typeof(ImagesPageViewModel))]
    [Transient(typeof(ChatPageViewModel))]
    [Transient(typeof(AudioPageViewModel))]
    [Transient(typeof(ModelsPageViewModel))]
    public partial interface IViewModelsServiceProvider
    {
    }
}
