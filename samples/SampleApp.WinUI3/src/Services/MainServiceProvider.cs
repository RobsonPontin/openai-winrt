using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Jab;

namespace SampleApp.WinUI3.Services
{
    [ServiceProvider]
    [Import(typeof(IViewModelsServiceProvider))]

    [Singleton(typeof(OpenAI.OpenAiService), Factory = nameof(GetOpenAiService))]
    public partial class MainServiceProvider
    {
    }
}
