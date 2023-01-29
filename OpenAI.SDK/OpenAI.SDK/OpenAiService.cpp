#include "pch.h"

#include "OpenAiService.h"
#if __has_include("OpenAiService.g.cpp")
#include "OpenAiService.g.cpp"
#endif

#include <winrt/Windows.Web.Http.h>
#include <winrt/Windows.Web.Http.Headers.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.UI.Xaml.Controls.h>

#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Networking.BackgroundTransfer.h>

#include "OpenAiOptions.h"
#include "ImageUtils.h"
#include "BaseRequest.h"

namespace winrt::OpenAI::implementation
{
    // TODO: all request Image, Completion, etc... should derive from an interface for IsValid, BuildHttp, Prompt, Model, etc...

    WF::IAsyncOperation<OpenAI::Image::ImageResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Image::ImageRequest const& imageRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(imageRequest);
        if (response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
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
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(completionRequest);
        if (response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
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
        }

        co_return winrt::make<OpenAI::Completion::implementation::CompletionResponse>();        
    }

    WF::IAsyncOperation<OpenAI::Embedding::EmbeddingResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Embedding::EmbeddingRequest const& embeddingRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(embeddingRequest);
        if (response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Parse JSON response
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
        }

        co_return winrt::make<OpenAI::Embedding::implementation::EmbeddingResponse>();        
    }

    WF::IAsyncOperation<OpenAI::Moderation::ModerationResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Moderation::ModerationRequest const& moderationRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(moderationRequest);
        if (response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
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
        }

        co_return winrt::make<OpenAI::Moderation::implementation::ModerationResponse>();
    }

    WF::IAsyncOperation<WWH::HttpResponseMessage> OpenAiService::PerformHttpRequestAsync(OpenAI::BaseRequest const& request)
    {
        if (m_openAiOptions != nullptr && request.IsValid())
        {
            // TODO: cache this value to avoid casting
            auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
            auto openAiKey = options_impl->OpenAiKey();

            auto httpRequest = request.BuildHttpRequest();

            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(
                WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", openAiKey));

            // Send the request and retrieve the response           
            WWH::HttpResponseMessage response = co_await httpClient.SendRequestAsync(httpRequest);
            co_return response;
        }

        co_return nullptr;
    }

    WF::IAsyncOperation<WDJ::JsonObject> OpenAiService::ParseHttpMsgToJsonAsync(WWH::HttpResponseMessage const& msg)
    {
        WDJ::JsonObject jsonObject = nullptr;

        try
        {
            auto stringResult = co_await msg.Content().ReadAsStringAsync();
            
            if (WDJ::JsonObject::TryParse(stringResult, jsonObject) == false)
            {
                // TODO: log error
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            winrt::hresult hr = ex.code(); // HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND).
            winrt::hstring message = ex.message(); // The system cannot find the file specified.

            // TODO: log error for result msg?
        }

        co_return jsonObject;
    }
}