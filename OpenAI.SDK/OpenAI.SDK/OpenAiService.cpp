#include "pch.h"
#include <OpenAiService.h>

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Networking.BackgroundTransfer.h>


namespace OpenAI_SDK
{
    /// <summary>
    /// Generates an images based off keywords
    /// </summary>
    /// <param name="keywords">A range of keywords</param>
    /// <param name="openAiKey">The OpenAI secret key</param>
    /// <returns></returns>
    WF::IAsyncOperation<WS::Streams::IBuffer> OpenAiService::GenerateDalleImageAsync(std::vector<winrt::hstring> keywords)
    {
        if (keywords.size() < 1 || m_openAiSecretKey == L"")
        {
            co_return nullptr;
        }

        // TODO:
        //co_await resume_background();

        // Concatenate keywords into a single query string
        winrt::hstring query;
        for (winrt::hstring keyword : keywords)
        {
            query = query + keyword + L" ";
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

            auto prompt = L"{\"model\": \"image-alpha-001\", \"prompt\": \"" + query + L"\", \"num_images\":1, \"size\":\"1024x1024\", \"response_format\": \"" + response_format + L"\"}";
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
        try
        {
            auto imgBuffer = co_await FileToPngBufferAsync(file);
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
            multipartContent.Add(bContent, L"image", file.Name());

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

    WF::IAsyncOperation<WS::StorageFile> OpenAiService::CreateFileFromImageAsync(WS::Streams::IBuffer buffer, bool saveToTempFolder)
    {
        WS::StorageFile file{ nullptr };
        if (!saveToTempFolder)
        {
            auto savePicker = WS::Pickers::FileSavePicker();
            savePicker.SuggestedStartLocation(WS::Pickers::PickerLocationId::PicturesLibrary);
            savePicker.FileTypeChoices().Insert(L"PNG", winrt::single_threaded_vector<winrt::hstring>({ L".png" }));
            // Default file name if the user does not type one in or select a file to replace
            savePicker.SuggestedFileName(L"AI image");

            file = co_await savePicker.PickSaveFileAsync();
        }
        else
        {
            auto tempFolder = WS::ApplicationData::Current().TemporaryFolder();
            file = co_await tempFolder.CreateFileAsync(L"ai_image", WS::CreationCollisionOption::GenerateUniqueName);
        }

        if (file != nullptr)
        {
            co_await WS::FileIO::WriteBufferAsync(file, buffer);
        }

        co_return file;
    }
}