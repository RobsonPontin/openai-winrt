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
    // TODO: all request Image, Completion, etc... should derive from an interface for IsValid, BuildHttp, Prompt, Model, etc...

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

    WF::IAsyncOperation<OpenAI::Completion::CompletionResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Completion::CompletionRequest const& completionRequest)
    {
        if (m_openAiOptions != nullptr && completionRequest.IsValid())
        {
            auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
            auto openAiKey = options_impl->OpenAiKey();

            auto request_impl = winrt::get_self<OpenAI::Completion::implementation::CompletionRequest>(completionRequest);
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

                // Extract the text completion data from the JSON response
                auto id = json.GetNamedString(L"id");
                auto object = json.GetNamedString(L"object");
                auto created = json.GetNamedNumber(L"created");
                auto model = json.GetNamedString(L"model");

                // Get choices vector
                auto choices = json.GetNamedValue(L"choices");
                auto firstChoice = choices.GetArray().GetAt(0).GetObject();
                auto text = firstChoice.GetNamedString(L"text");

                // TODO: Example of response to be parsed:
                /*
                     { "id":"cmpl-6Zx709yAFD1tRUubVOm3t4QXRAIqM",
                       "object":"text_completion",
                       "created":1674026726,
                       "model":"text-davinci-003",
                       "choices":[
                       {
                          "text":"\n\nQ: Did you hear about the restaurant on the moon?\nA",
                          "index":0,
                          "logprobs":null,
                          "finish_reason":"length"
                       }],"usage":{"prompt_tokens":6,"completion_tokens":16,"total_tokens":22}}
                */
                co_return winrt::make<OpenAI::Completion::implementation::CompletionResponse>(text);
            }

            co_return winrt::make<OpenAI::Completion::implementation::CompletionResponse>();
        }
    }

    WF::IAsyncOperation<OpenAI::Embedding::EmbeddingResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Embedding::EmbeddingRequest const& embeddingRequest)
    {
        if (m_openAiOptions != nullptr && embeddingRequest.IsValid())
        {
            auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
            auto openAiKey = options_impl->OpenAiKey();

            auto request_impl = winrt::get_self<OpenAI::Embedding::implementation::EmbeddingRequest>(embeddingRequest);
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
                auto data = json.GetNamedArray(L"data");
               
                WF::Collections::IVector<Embedding::EmbeddingValue> embeddingList{ single_threaded_vector<Embedding::EmbeddingValue>() };

                for (auto jsonValue : data)
                {
                    std::vector<double> embeddingValues{};
                    int32_t index = 0;

                    auto obj = jsonValue.GetObject();
                    index = static_cast<int32_t>(obj.GetNamedNumber(L"index"));
                    auto embeddingArray = obj.GetNamedArray(L"embedding");

                    for (uint32_t i = 0; i < embeddingArray.Size(); ++i)
                    {
                        double value = embeddingArray.GetNumberAt(i);
                        embeddingValues.push_back(value);
                    }

                    if (embeddingValues.size() > 0)
                    {
                        auto embeddingVal = winrt::make<OpenAI::Embedding::implementation::EmbeddingValue>(index, embeddingValues);
                        embeddingList.Append(embeddingVal);
                    }
                }

                if (embeddingList.Size() > 0)
                {
                    co_return winrt::make<OpenAI::Embedding::implementation::EmbeddingResponse>(embeddingList);
                }
            }

            co_return winrt::make<OpenAI::Embedding::implementation::EmbeddingResponse>();
        }
    }

    WF::IAsyncOperation<OpenAI::Moderation::ModerationResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Moderation::ModerationRequest const& moderationRequest)
    {
        if (m_openAiOptions != nullptr && moderationRequest.IsValid())
        {
            auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
            auto openAiKey = options_impl->OpenAiKey();

            auto request_impl = winrt::get_self<OpenAI::Moderation::implementation::ModerationRequest>(moderationRequest);
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

                auto id = json.GetNamedString(L"id");
                auto results = json.GetNamedArray(L"results");
                
                // Only evaluate and parse first data from array
                if (results.Size() == 1)
                {
                    std::vector<Moderation::IModerationValue> moderationValues;

                    auto jsonValue = results.GetAt(0);
                    auto categoryArray = jsonValue.GetObject();

                    // flags
                    auto categories = categoryArray.GetNamedObject(L"categories");                    
                    auto hate = categories.GetNamedBoolean(L"hate");
                    auto hateThreatening = categories.GetNamedBoolean(L"hate/threatening");
                    auto selfHarm = categories.GetNamedBoolean(L"self-harm");
                    auto sexual = categories.GetNamedBoolean(L"sexual");
                    auto sexualMinors = categories.GetNamedBoolean(L"sexual/minors");
                    auto violence = categories.GetNamedBoolean(L"violence");
                    auto violenceGraphic = categories.GetNamedBoolean(L"violence/graphic");

                    // scores
                    auto category_scores = categoryArray.GetNamedObject(L"category_scores");
                    auto dHate = category_scores.GetNamedNumber(L"hate");
                    auto dHateThreatening = category_scores.GetNamedNumber(L"hate/threatening");
                    auto dSelfHarm = category_scores.GetNamedNumber(L"self-harm");
                    auto dSexual = category_scores.GetNamedNumber(L"sexual");
                    auto dSexualMinors = category_scores.GetNamedNumber(L"sexual/minors");
                    auto dViolence = category_scores.GetNamedNumber(L"violence");
                    auto dViolenceGraphic = category_scores.GetNamedNumber(L"violence/graphic");

                    // TODO: proper parse data to a list, reuse code, make response
                    moderationValues.push_back(winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::Hate, hate, dHate));
                }
                                
                // TODO: parse data example
                /*
                 * {
                      "id": "modr-6dY1XYd6T2iOzsYRn7hckqrHcpp4U",
                      "model": "text-moderation-004",
                      "results": [
                        {
                          "categories": {
                            "hate": false,
                            "hate/threatening": false,
                            "self-harm": false,
                            "sexual": false,
                            "sexual/minors": false,
                            "violence": true,
                            "violence/graphic": false
                          },
                          "category_scores": {
                            "hate": 0.029734386131167412,
                            "hate/threatening": 0.00309771578758955,
                            "self-harm": 2.064793225287076e-09,
                            "sexual": 1.0425052323626005e-06,
                            "sexual/minors": 7.927656753281553e-09,
                            "violence": 0.9992383718490601,
                            "violence/graphic": 4.949376670992933e-05
                          },
                          "flagged": true
                        }
                      ]
                    } 
                 */
            }

            co_return winrt::make<OpenAI::Moderation::implementation::ModerationResponse>();
        }
    }
}