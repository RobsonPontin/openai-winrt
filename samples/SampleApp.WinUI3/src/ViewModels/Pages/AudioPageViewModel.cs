using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using SampleApp.WinUI3.Services;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage.Pickers;
using Windows.Storage;
using Microsoft.UI.Xaml;
using OpenAI.Audio;

namespace SampleApp.WinUI3.ViewModels
{
    internal partial class AudioPageViewModel : BaseViewModel
    {
        internal AudioPageViewModel()
        {
            OpenAudioFileCommand = new AsyncRelayCommand(OpenAudioFileAsync);
        }

        [ObservableProperty]
        private string audioText = string.Empty;

        [ObservableProperty]
        private string audiofileName = string.Empty;

        public IAsyncRelayCommand OpenAudioFileCommand { get; set; }

        private async Task OpenAudioFileAsync()
        {
            var filePicker = new FileOpenPicker();

            // Get the current window's HWND by passing in the Window object
            var hwnd = WinRT.Interop.WindowNative.GetWindowHandle((Application.Current as App).MainWindow);

            // Associate the HWND with the file picker
            WinRT.Interop.InitializeWithWindow.Initialize(filePicker, hwnd);

            filePicker.FileTypeFilter.Add(".wav");
            filePicker.FileTypeFilter.Add(".mp3");
            filePicker.FileTypeFilter.Add(".mpga");
            filePicker.FileTypeFilter.Add(".m4a");
            filePicker.FileTypeFilter.Add(".mp4");
            filePicker.FileTypeFilter.Add(".mpeg");
            filePicker.FileTypeFilter.Add(".webm");

            var file = await filePicker.PickSingleFileAsync();
            if (file != null) 
            {
                AudiofileName = file.Name;

                AudioRequest audioRequest = new();
                await audioRequest.SetAudioFileAsync(file);

                var result = await m_openAiService.RunRequestAsync(audioRequest);
                if (result.IsResponseSuccess)
                {
                    AudioText = result.Text;
                }
                else
                {
                    // TODO: Error
                }
            }
        }
    }
}
