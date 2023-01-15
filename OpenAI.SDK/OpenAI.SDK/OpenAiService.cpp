#include "pch.h"

#include "OpenAiService.h"
#if __has_include("OpenAiService.g.cpp")
#include "OpenAiService.g.cpp"
#endif

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Networking.BackgroundTransfer.h>

#include "OpenAiOptions.h"
#include "ImageUtils.h"

namespace winrt::OpenAI::implementation
{
    WF::IAsyncOperation<OpenAI::Image::ImageResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Image::ImageRequest const& imageRequest)
    {
        if (m_openAiOptions != nullptr && imageRequest.IsValid())
        {
            auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
            auto openAiKey = options_impl->OpenAiKey();

            auto request_impl = winrt::get_self<OpenAI::Image::implementation::ImageRequest>(imageRequest);
            auto httpRequest = request_impl->BuildHttpRequest();

            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(
                WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", openAiKey));
                        
            // Send the request and retrieve the response           
            WWH::HttpResponseMessage response = co_await httpClient.SendRequestAsync(httpRequest);
            auto stringResult = co_await response.Content().ReadAsStringAsync();
            if (response.IsSuccessStatusCode())
            {
                auto json = WDJ::JsonObject::Parse(stringResult);

                // Extract the image data from the JSON response
                auto data = json.GetNamedValue(L"data");

                // It will return an array of images, in this case we are only getting the first one
                auto jValue = data.GetArray().GetAt(0).GetObject();
                auto stringUri = jValue.GetNamedString(L"url");

                WF::Uri aiImage{ stringUri };

                // Result will come in PNG format
                WWH::HttpClient httpClientGet;
                auto result = co_await httpClientGet.GetAsync(aiImage);
                auto imageBuffer = co_await result.Content().ReadAsBufferAsync();

                std::vector<WS::Streams::IBuffer> images;
                images.push_back(imageBuffer);

                co_return winrt::make<OpenAI::Image::implementation::ImageResponse>(images);
            }
        }

        // TODO: Handle error and response result
        co_return winrt::make<OpenAI::Image::implementation::ImageResponse>();
    }

    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::GenerateDalleImageAsync(winrt::hstring const& keywords)
    {
        if (keywords == L"")
        {
            co_return nullptr;
        }

        try
        {
            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", L""));

            // Set up the API endpoint and parameters
            WWH::HttpRequestMessage request(
                WWH::HttpMethod::Post(),
                WF::Uri(L"https://api.openai.com/v1/images/generations"));

            winrt::hstring response_format = L"url"; // Either "url" or "b64_json"

            auto prompt = L"{\"model\": \"image-alpha-001\", \"prompt\": \"" + keywords + L"\", \"num_images\":1, \"size\":\"1024x1024\", \"response_format\": \"" + response_format + L"\"}";
            WWH::HttpStringContent content(prompt, winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8);
            content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
            request.Content(content);

            // Send the request and retrieve the response           
            WWH::HttpResponseMessage response = co_await httpClient.SendRequestAsync(request);

            if (response.IsSuccessStatusCode())
            {
                auto ui_thread = winrt::apartment_context();
                co_await winrt::resume_background();
                auto stringResult = response.Content().ReadAsStringAsync().get();
                co_await ui_thread;

                auto json = WDJ::JsonObject::Parse(stringResult);

                // Extract the image data from the JSON response
                auto data = json.GetNamedValue(L"data");

                // It will return an array of images, in this case we are only getting the first one
                auto jValue = data.GetArray().GetAt(0).GetObject();
                auto stringUri = jValue.GetNamedString(L"url");

                WF::Uri aiImage{ stringUri };

                // Result will come in PNG format
                WWH::HttpClient httpClientGet;
                auto result = co_await httpClientGet.GetAsync(aiImage);
                auto imageBuffer = co_await result.Content().ReadAsBufferAsync();

                co_return imageBuffer;
            }

        }
        catch (...)
        {
            // sigh.....
        }

        co_return nullptr;
    }

    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::GenerateDalleVariantAsync(WS::StorageFile const& file)
    {
        if (file == nullptr)
        {
            co_return nullptr;
        }

        try
        {
            auto localFile = file; // Create a local copy

            auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(localFile);
            if (imgBuffer == nullptr)
            {
                co_return nullptr;
            }

            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", L""));

            // Set up the API endpoint and parameters
            WWH::HttpRequestMessage request(
                WWH::HttpMethod::Post(),
                WF::Uri(L"https://api.openai.com/v1/images/variations"));

            WWH::HttpBufferContent bContent{ imgBuffer };
            bContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"image/png"));

            WWH::HttpMultipartFormDataContent multipartContent{};

            auto fileName = localFile.Name() == L"" ? L"untitled" : localFile.Name();
            multipartContent.Add(bContent, L"image", fileName);

            winrt::hstring boundary = winrt::to_hstring(Windows::Foundation::GuidHelper::CreateNewGuid()); // create unique ID
            multipartContent.Headers().TryAppendWithoutValidation(L"Content-Type", L"multipart/form-data; boundary=" + boundary);

            request.Content(multipartContent);

            // Send the request and retrieve the response           
            WWH::HttpResponseMessage response = co_await httpClient.SendRequestAsync(request);

            if (response.IsSuccessStatusCode())
            {
                auto ui_thread = winrt::apartment_context();
                co_await winrt::resume_background();
                auto stringResult = response.Content().ReadAsStringAsync().get();
                co_await ui_thread;

                auto json = WDJ::JsonObject::Parse(stringResult);

                // Extract the image data from the JSON response
                auto data = json.GetNamedValue(L"data");

                // It will return an array of images, in this case we are only getting the first one
                auto jValue = data.GetArray().GetAt(0).GetObject();
                auto stringUri = jValue.GetNamedString(L"url");

                WF::Uri aiImage{ stringUri };

                // Result will come in PNG format
                WWH::HttpClient httpClientGet;
                auto result = co_await httpClientGet.GetAsync(aiImage);
                co_return co_await result.Content().ReadAsBufferAsync();
            }
        }
        catch (...)
        {
            // Sigh....
        }

        co_return nullptr;
    }
}