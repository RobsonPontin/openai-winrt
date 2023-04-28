using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.Services
{
    internal class ServiceProvider
    {
        static ServiceProvider spInstance;
        OpenAI.OpenAiService openAiService;

        private ServiceProvider()
        { 
        }

        public static ServiceProvider Instance
        {
            get
            {
                if (spInstance == null)
                {
                    spInstance = new ServiceProvider();
                }

                return spInstance;
            }
        }

        public OpenAI.OpenAiService OpenAiService
        {
            get
            {
                if (openAiService == null)
                {
                    var options = new OpenAI.OpenAiOptions();
                    options.SetOpenAiKeyFromEnvironmentVar("MY_OPEN_AI_API_KEY");

                    openAiService = new OpenAI.OpenAiService(options);
                }

                return openAiService;
            }
        }

    }
}
