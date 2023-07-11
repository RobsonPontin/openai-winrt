using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using OpenAI.Chat;
using SampleApp.WinUI3.Services;
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;

namespace SampleApp.WinUI3.ViewModels
{
    internal class UiChatMessage
    {
        public string Username { get; set; }
        public string Message { get; set; }
    }

    internal partial class ChatPageViewModel : BaseViewModel
    {
        public ChatPageViewModel() 
        {
            m_openAiService = ServiceProvider.Instance.OpenAiService;

            SendMessageCommand = new AsyncRelayCommand(SendMessageAsync);
        }

        [ObservableProperty]
        private string messageToSend = string.Empty;

        public ObservableCollection<UiChatMessage> Messages { get; set; } = new ObservableCollection<UiChatMessage>();

        public IAsyncRelayCommand SendMessageCommand { get; set; }

        private async Task SendMessageAsync()
        {
            if (string.IsNullOrWhiteSpace(MessageToSend))
            {
                return;
            }

            // Add user msg to listview
            Messages.Add(new UiChatMessage 
            { 
                Message = MessageToSend, 
                Username = "Me" 
            });

            if (!m_openAiService.IsRunning)
            {
                ChatRequest chatRequest = new ChatRequest();
                OpenAI.Chat.ChatMessage chatMessage = new("user", MessageToSend);
                chatRequest.Messages.Add(chatMessage);

                var result = await m_openAiService.RunRequestAsync(chatRequest);
                if (result.IsResponseSuccess)
                {
                    var response = result.Choices.First();
                    Messages.Add(new UiChatMessage 
                    { 
                        Message = response.Message.Content,
                        Username = "A.I." 
                    });

                    // clear msg
                    MessageToSend = string.Empty;
                }
                else
                {
                    // failed to reply
                    Messages.Add(new UiChatMessage 
                    { 
                        Message = "Fail to reply. Error: " + result.Error.Message, 
                        Username = "A.I." 
                    });
                }
            }
        }
    }
}
