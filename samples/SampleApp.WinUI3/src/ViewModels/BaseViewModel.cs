using CommunityToolkit.Mvvm.ComponentModel;
using SampleApp.WinUI3.Services;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels
{
    internal class BaseViewModel : ObservableObject
    {
        internal OpenAI.OpenAiService m_openAiService;

        internal BaseViewModel()
        {
            m_openAiService = ServiceProvider.Instance.OpenAiService;
        }
    }
}
