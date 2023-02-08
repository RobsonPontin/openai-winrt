// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

using Microsoft.UI.Xaml;
using System;


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
                m_openAiService = new OpenAI.OpenAiService();
                m_openAiService.SetOpenAiSecretKey("YOU_OPEN_AI_KEY");
            }

            var imgRequest = new OpenAI.Image.ImageCreateRequest();
            imgRequest.Prompt = "A robot bird flying above a green lake, abstract, art";

            var result = await m_openAiService.RunRequestAsync(imgRequest);
            if (result.IsResponseSuccess)
            {
                // Handle image received
                //var img = result.Images.First();
            }
        }
    }
}
