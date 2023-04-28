using CommunityToolkit.Mvvm.ComponentModel;
using Microsoft.UI.Xaml;
using SampleApp.WinUI3.Services;
using SampleApp.WinUI3.ViewModels;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels.Pages
{
    internal partial class ModelsPageViewModel : BaseViewModel
    {
        public ModelsPageViewModel() 
        {
            ModelValues = new ObservableCollection<OpenAI.ModelValue>();
        }

        [ObservableProperty]
        private ObservableCollection<OpenAI.ModelValue> modelValues;

        public async Task RequestModelsAsync()
        {
            var openAiService = ServiceProvider.Instance.OpenAiService;

            var result = await openAiService.RunRequestAsync(new OpenAI.ModelRequest());
            if (result.IsResponseSuccess)
            {
                var models = result.Data.ToList<OpenAI.ModelValue>();
                foreach (OpenAI.ModelValue model in models)
                {
                    ModelValues.Add(model);
                }
            }
        }
    }
}
