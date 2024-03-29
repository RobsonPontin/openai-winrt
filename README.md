# WinRT SDK for OpenAI DALL·E and GPT

A community-maintained (unofficial) OpenAI WinRT SDK that can be used with [UWP (Universal Windows Platform)](https://learn.microsoft.com/en-us/windows/uwp/get-started/universal-application-platform-guide) and/or [WinUI 3 (Desktop)](https://learn.microsoft.com/en-us/windows/apps/winui/winui3/) apps for both C++ and C# languages.

For more information visit https://openai.com/.

## OpenAI Service

It is necessary to initialize the service with an API Key, which can be obtained at https://openai.com/.

The example below show how to get the OpenAI Key by passing your environment variable which contains it.

```cs
var options = new OpenAI.OpenAiOptions();
options.SetOpenAiKeyFromEnvironmentVar("MY_OPEN_AI_API_KEY");

m_openAiService = new OpenAI.OpenAiService(options);
```

## GPT3 APIs

### Chat

Using chat you can go back and forth with the AI models about almost any subject

```cs
var chatRequest = new OpenAI.Chat.ChatRequest();
var chatMessage = new OpenAI.Chat.ChatMessage("user", "What is the most used math formula in the world?");
chatRequest.Messages.Add(chatMessage);

var response = await m_openAiService.RunRequestAsync(chatRequest);
```

### Text Completion

With text completion you can send some text as a prompt, and the model will generate a text completion that attempts to match whatever context or pattern you gave it.

```cpp
winrt::hstring prompt = L"tell me a very nice joke";

auto completionReq = OpenAI::Completion::CompletionRequest{};
completionReq.Prompt(prompt);
completionReq.Model(ModelType::text_davinci_003);

auto response = co_await m_openAiService.RunRequestAsync(completionReq);
```

## DALL·E

### Image Generation

Generate images from a command prompt.

```cpp
winrt::hstring prompt = L"a robot duck walking on the beach";

auto imageReq = OpenAI::Image::ImageCreateRequest{};
imageReq.ImageName(L"ai image");
imageReq.Prompt(prompt);

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```

### Image Variation

Create image variations from a source image file.

```cpp
Windows::Storage::StorageFile file = GetFileFromPicker();

auto imageReq = OpenAI::Image::ImageVariationRequest{};
imageReq.ImageName(file.Name());
co_await imageReq.SetImageAsync(file);

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```

### Image Editing

Create a new image from a source image file considering the prompt editing commands. 

```cpp
Windows::Storage::StorageFile file = GetFileFromPicker();
WindowS::Storage::StorageFile mask = GetFileFromPicker();

winrt::hstring prompt = L"add a small tiger with shoes";

auto imageReq = OpenAI::Image::ImageEditRequest{};
imageReq.ImageName(file.Name());
imageReq.Prompt(prompt);
co_await imageReq.SetImageAsync(file);
co_await imageReq.SetMaskAsync(mask); // Mask needed to declare which are of the image the editing should be applied

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```
