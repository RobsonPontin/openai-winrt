using CommunityToolkit.Mvvm.ComponentModel;
using SampleApp.WinUI3.Services;
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels
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

            try
            {
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
            catch(Exception) 
            {
                // TODO: ?
            }
        }
    }
}
