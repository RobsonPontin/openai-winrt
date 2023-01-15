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
}