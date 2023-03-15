// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Media.Imaging;
using System;
using System.Linq;
using System.Threading.Tasks;
using Windows.Storage.Streams;

namespace SampleApp.WinUI3
{
    public sealed partial class MainWindow : Window
    {
        OpenAI.OpenAiService m_openAiService;

        public MainWindow()
        {
            this.InitializeComponent();
        }

        private async void myButton_Click(object sender, RoutedEventArgs e)
        {
            if (m_openAiService == null)
            {
                var options = new OpenAI.OpenAiOptions();
                options.SetOpenAiKeyFromEnvironmentVar("MY_OPEN_AI_API_KEY");

                m_openAiService = new OpenAI.OpenAiService(options);
            }

            await RequestModelsAsync();

            return;

            var imgRequest = new OpenAI.Image.ImageCreateRequest();
            // Change to something else (or move it to a text box in the XAML)
            imgRequest.Prompt = "A robot bird flying above a green lake";

            var result = await m_openAiService.RunRequestAsync(imgRequest);
            if (result.IsResponseSuccess)
            {
                // Handle image received
                var imgBuffer = result.Images.First();

                var stream = new InMemoryRandomAccessStream();
                var outputStream = stream.GetOutputStreamAt(0);
                var datawriter = new DataWriter(outputStream);
                datawriter.WriteBuffer(imgBuffer);
                await datawriter.StoreAsync();
                await outputStream.FlushAsync();

                if (stream != null)
                {
                    var img = new BitmapImage();
                    img.SetSource(stream);
                    image.Source = img;
                }

                textBlock.Visibility = Visibility.Collapsed;
                image.Visibility = Visibility.Visible;
            }
            else
            {                
                if (result.Error!= null) 
                {
                    textBlock.Text = result.Error.Message;

                    textBlock.Visibility = Visibility.Visible;
                    image.Visibility = Visibility.Collapsed;
                }
            }
        }

        private async Task RequestChatAsync()
        {
            if (m_openAiService == null)
            {
                return;
            }

            var chatReq = new OpenAI.Chat.ChatRequest();
            var message = new OpenAI.Chat.ChatMessage(
                "user",
                "can we fly?"
            );

            chatReq.Messages.Add(message);

            var result = await m_openAiService.RunRequestAsync(chatReq);
            if (result.IsResponseSuccess)
            {
                // TODO: handle chat response
            }
        }

        public async Task RequestModelsAsync()
        {
            var result = await m_openAiService.RunRequestAsync(new OpenAI.ModelRequest());
            if (result.IsResponseSuccess)
            {
                // TODO: handle model response
            }
        }
    }
}
