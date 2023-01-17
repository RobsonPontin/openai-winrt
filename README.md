# WinRT SDK for OpenAI DALL·E

A WinRT SDK that can be used with UWP (Universal Windows Platform) apps.

Visit https://openai.com/ to get your API key. Also documentation with more detail is avaliable there.

## DALL·E Sample

### Image Generation

Generate images from a command prompt.

```cpp
winrt::hstring prompt = L"a robot duck walking on the beach";

auto imageReq = OpenAI::Image::ImageCreateRequest{};
imageReq.ImageName(L"ai image");
imageReq.Prompt(prompt);

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```

### Image Variant

Create image variants from a source image file.

```cpp
Windows::Storage::StorageFile file = GetFileFromPicker();

auto imageReq = OpenAI::Image::ImageVariationRequest{};
imageReq.ImageName(file.Name());
co_await imageReq.SetImageAsync(file);

auto response = co_await m_openAiService.RunRequestAsync(imageReq);
```
