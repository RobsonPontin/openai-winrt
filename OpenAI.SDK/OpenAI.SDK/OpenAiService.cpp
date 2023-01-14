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


namespace winrt::OpenAI_SDK::implementation
{
    /// <summary>
    /// Generates an images based off keywords
    /// </summary>
    /// <param name="keywords">A range of keywords</param>
    /// <param name="openAiKey">The OpenAI secret key</param>
    /// <returns></returns>
    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::GenerateDalleImageAsync(winrt::hstring const& keywords)
    {
        if (keywords == L"" || m_openAiSecretKey == L"")
        {
            co_return nullptr;
        }

        try
        {
            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", m_openAiSecretKey));

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

    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::FileToPngBufferAsync(WS::StorageFile const& file)
    {
        try
        {
            // Open and decode
            auto stream = co_await file.OpenReadAsync();
            auto decoder = co_await WGI::BitmapDecoder::CreateAsync(stream);
            auto pixels = co_await decoder.GetPixelDataAsync();
            auto imageBytes = pixels.DetachPixelData();
            auto dpiX = decoder.DpiX();
            auto dpiY = decoder.DpiY();
            auto pixelHeight = static_cast<uint32_t>(decoder.PixelHeight());
            auto pixelWidth = static_cast<uint32_t>(decoder.PixelWidth());

            // encode
            WS::Streams::InMemoryRandomAccessStream encoded{};
            auto encoder = co_await WGI::BitmapEncoder::CreateAsync(WGI::BitmapEncoder::PngEncoderId(), encoded);
            encoder.SetPixelData(WGI::BitmapPixelFormat::Bgra8, WGI::BitmapAlphaMode::Straight, pixelWidth, pixelHeight, dpiX, dpiY, imageBytes);
            co_await encoder.FlushAsync();
            encoded.Seek(0);

            auto size = static_cast<uint32_t>(encoded.Size());
            WS::Streams::Buffer buffer{ size };
            co_await encoded.ReadAsync(buffer, size, WS::Streams::InputStreamOptions::None);

            co_return buffer;
        }
        catch (...)
        {
            // Sigh...
        }

        co_return nullptr;
    }

    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::GenerateDalleVariantAsync(WS::StorageFile const& file)
    {
        if (file == nullptr || m_openAiSecretKey == L"")
        {
            co_return nullptr;
        }

        try
        {
            auto localFile = file;

            auto imgBuffer = co_await FileToPngBufferAsync(localFile);
            if (imgBuffer == nullptr)
            {
                co_return nullptr;
            }

            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", m_openAiSecretKey));

            // Set up the API endpoint and parameters
            WWH::HttpRequestMessage request(
                WWH::HttpMethod::Post(),
                WF::Uri(L"https://api.openai.com/v1/images/variations"));

            WWH::HttpBufferContent bContent{ imgBuffer };
            bContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"image/png"));

            WWH::HttpMultipartFormDataContent multipartContent{};

            auto fileName = localFile.Name() == L"" ? L"ai_image" : localFile.Name();
            multipartContent.Add(bContent, L"image", fileName);

            winrt::hstring boundary = L"my_boundary_string_rpp"; // TODO: need to generate unique id
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