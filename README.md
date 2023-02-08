# WinRT SDK for OpenAI DALL·E

A community-maintained (unofficial) OpenAI WinRT SDK that can be used with UWP (Universal Windows Platform) and/or WinUI 3 (Desktop) apps for both C++ and C# languages.

For more information visit https://openai.com/.

## OpenAI Service

It is necessary to initialize the service with a API Key, which can be obtained at https://openai.com/.

```cpp
OpenAI::OpenAiService m_openAiService{};
m_openAiService.SetOpenAiSecretKey(L"SECRET API KEY");
```

## GPT3 APIs

### Text Completion

With text compoetion you can input some text as a prompt, and the model will generate a text completion that attempts to match whatever context or pattern you gave it.

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

winrt::hstring prompt = L"remove the background from the image";

auto imageReq = OpenAI::Image::ImageEditRequest{};
imageReq.ImageName(file.Name());
imageReq.Prompt(prompt);
co_await imageReq.SetImageAsync(file);

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```
