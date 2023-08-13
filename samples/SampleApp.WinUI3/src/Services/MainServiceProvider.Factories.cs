using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.Services
{
    public partial class MainServiceProvider
    {
        OpenAI.OpenAiService GetOpenAiService()
        {
            /* 
             * The assumption here is that "MY_OPEN_AI_API_KEY" is registered
             * as an environment variable which should contain the OpenAI key.
             */
            var options = new OpenAI.OpenAiOptions();
            options.SetOpenAiKeyFromEnvironmentVar("MY_OPEN_AI_API_KEY");

            return new OpenAI.OpenAiService(options);
        }
    }
}
