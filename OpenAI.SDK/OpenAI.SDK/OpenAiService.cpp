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
#include "ConnectionUtils.h"
#include "BaseRequest.h"
#include "ResponseError.h"

namespace winrt::OpenAI::implementation
{
    OpenAiService::OpenAiService()
    {
        SetOpenAiOptionsInternal(OpenAI::OpenAiOptions{});
    }

    OpenAiService::OpenAiService(OpenAI::OpenAiOptions const& options)
    {
        SetOpenAiOptionsInternal(options);
    }

    void OpenAiService::SetOpenAiOptionsInternal(OpenAI::OpenAiOptions const& options)
    {
        m_openAiOptions = options;

        auto options_impl = winrt::get_self<implementation::OpenAiOptions>(m_openAiOptions);
        m_openAiOptions_impl.copy_from(options_impl);
    }

    WF::IAsyncOperation<OpenAI::ResponseError> OpenAiService::GetErrorFromMessageAsync(WWH::HttpResponseMessage const httpMessage)
    {
        if (!m_isInternetAvailable)
        {
            co_return winrt::make<OpenAI::implementation::ResponseError>(L"No internet connection.");
        }

        if (httpMessage != nullptr)
        {
            auto json = co_await ParseHttpMsgToJsonAsync(httpMessage);
            if (json != nullptr)
            {
                try
                {
                    auto error = json.GetNamedObject(L"error");
                    auto code = error.GetNamedString(L"code");
                    auto message = error.GetNamedString(L"message");
                    auto type = error.GetNamedString(L"type");

                    co_return winrt::make<OpenAI::implementation::ResponseError>(code, message, type);
                }
                catch (winrt::hresult const&)
                {
                    // TODO: Parsing certain JSON data might throw
                }
            }
        }

        co_return winrt::make<OpenAI::implementation::ResponseError>();
    }

    WF::IAsyncOperation<OpenAI::Image::ImageResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Image::ImageRequest const imageRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(imageRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Extract the image data from the JSON response
                auto data = json.GetNamedValue(L"data");

                auto imageArray = data.GetArray();
                std::vector<WS::Streams::IBuffer> images;

                for (uint32_t i = 0; i < imageArray.Size(); ++i)
                {
                    // It will return an array of images, in this case we are only getting the first one
                    auto jValue = imageArray.GetAt(i).GetObject();
                    auto stringUri = jValue.GetNamedString(L"url");

                    WF::Uri aiImage{ stringUri };

                    // Result will come in PNG format
                    WWH::HttpClient httpClientGet;
                    auto result = co_await httpClientGet.GetAsync(aiImage);
                    auto imageBuffer = co_await result.Content().ReadAsBufferAsync();

                    images.push_back(imageBuffer);
                }

                co_return winrt::make<OpenAI::Image::implementation::ImageResponse>(images);
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Image::implementation::ImageResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::Completion::CompletionResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Completion::CompletionRequest const completionRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(completionRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
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
                auto choices = json.GetNamedArray(L"choices");

                std::vector<Completion::CompletionChoice> completionList{};

                for (auto jsonValue : choices)
                {
                    auto obj = jsonValue.GetObject();
                    auto text = obj.GetNamedString(L"text");
                    auto finishReason = obj.GetNamedString(L"finish_reason");
                    auto index = static_cast<uint32_t>(obj.GetNamedNumber(L"index"));
                    
                    /* TODO: parse Logprobs
                     * Example: "logprobs": {"tokens":[],"token_logprobs":[],"top_logprobs":[],"text_offset":[]} 
                     */
                    // auto ob = obj.GetNamedObject(L"logprobs");                    

                    completionList.push_back(
                        winrt::make<OpenAI::Completion::implementation::CompletionChoice>(text, finishReason, index));
                }

                auto usage = json.GetNamedObject(L"usage");
                
                uint32_t promptTokens = 0;
                if (usage.HasKey(L"prompt_tokens"))
                {
                    promptTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"prompt_tokens"));
                }

                uint32_t completionTokens = 0;
                if (usage.HasKey(L"completion_tokens"))
                {
                    completionTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"completion_tokens"));
                }

                uint32_t totalTokens = 0;
                if (usage.HasKey(L"total_tokens"))
                {
                    totalTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"total_tokens"));
                }

                auto completionUsage = winrt::make<OpenAI::Completion::implementation::CompletionUsage>(promptTokens, completionTokens, totalTokens);

                co_return winrt::make<OpenAI::Completion::implementation::CompletionResponse>(
                    id,
                    object,
                    created,
                    model,
                    completionList,
                    completionUsage);
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Completion::implementation::CompletionResponse>(error);
        }     
    }

    WF::IAsyncOperation<OpenAI::Embedding::EmbeddingResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Embedding::EmbeddingRequest const embeddingRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(embeddingRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Parse JSON response
                auto data = json.GetNamedArray(L"data");

                WFC::IVector<Embedding::EmbeddingValue> embeddingList{ single_threaded_vector<Embedding::EmbeddingValue>() };

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
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Embedding::implementation::EmbeddingResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::Moderation::ModerationResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Moderation::ModerationRequest const moderationRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(moderationRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                auto id = json.GetNamedString(L"id");
                auto results = json.GetNamedArray(L"results");

                // Only evaluate and parse first data from array
                if (results.Size() == 1)
                {
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

                    std::vector<Moderation::ModerationValue> moderationValues;
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::Hate, hate, dHate));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::HateThreatening, hateThreatening, dHateThreatening));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::selfHarm, selfHarm, dSelfHarm));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::Sexual, sexual, dSexual));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::SexualMinors, sexualMinors, dSexualMinors));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::Violence, violence, dViolence));
                    moderationValues.push_back(
                        winrt::make<Moderation::implementation::ModerationValue>(Moderation::ModerationCategory::ViolenceGraphic, violenceGraphic, dViolenceGraphic));

                    co_return winrt::make<OpenAI::Moderation::implementation::ModerationResponse>(id, moderationValues);
                }
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Moderation::implementation::ModerationResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::Edits::EditsResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Edits::EditsRequest const editRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(editRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Parse JSON response
                auto choices = json.GetNamedArray(L"choices");

                std::vector<Edits::EditsChoice> editsList{};

                for (auto jsonValue : choices)
                {
                    auto obj = jsonValue.GetObject();
                    auto text = obj.GetNamedString(L"text");
                    auto index = static_cast<uint32_t>(obj.GetNamedNumber(L"index"));

                    editsList.push_back(winrt::make<OpenAI::Edits::implementation::EditsChoice>(index, text));
                }
                
                auto usage = json.GetNamedObject(L"usage");
                auto promptTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"prompt_tokens"));
                auto completionTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"completion_tokens"));
                auto totalTokens = static_cast<uint32_t>(usage.GetNamedNumber(L"total_tokens"));

                auto editsUsage = winrt::make<OpenAI::Edits::implementation::EditsUsage>(promptTokens, completionTokens, totalTokens);

                co_return winrt::make<OpenAI::Edits::implementation::EditsResponse>(editsList, editsUsage);
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Edits::implementation::EditsResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::ModelResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::ModelRequest const modelRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await PerformHttpRequestAsync(modelRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Extract the text completion data from the JSON response
                auto object = json.GetNamedString(L"object");

                // Get choices vector
                auto data = json.GetNamedArray(L"data");
                std::vector<OpenAI::ModelValue> models{};

                for (auto jsonValue : data)
                {
                    auto obj = jsonValue.GetObject();
                    auto id = obj.GetNamedString(L"id");
                    auto model_object = obj.GetNamedString(L"object");
                    auto owned_by = obj.GetNamedString(L"owned_by");

                    models.push_back(winrt::make<OpenAI::implementation::ModelValue>(id, model_object, owned_by));
                }

                co_return winrt::make<OpenAI::implementation::ModelResponse>(models, object);
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::implementation::ModelResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::FilesResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::FilesRequest const filesRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await OpenAiService::PerformHttpRequestAsync(filesRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // TODO: json parsing and response
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::implementation::FilesResponse>(error);
        }
    }

    WF::IAsyncOperation<OpenAI::Chat::ChatResponse> OpenAiService::RunRequestAsync(winrt::OpenAI::Chat::ChatRequest const chatRequest)
    {
        // Send the request and retrieve the response  
        WWH::HttpResponseMessage response = co_await OpenAiService::PerformHttpRequestAsync(chatRequest);
        if (response != nullptr && response.IsSuccessStatusCode())
        {
            auto json = co_await ParseHttpMsgToJsonAsync(response);
            if (json != nullptr)
            {
                // Extract the text completion data from the JSON response
                auto id = json.GetNamedString(L"id");
                auto object = json.GetNamedString(L"object");
                auto created = json.GetNamedNumber(L"created");

                // Get choices vector
                auto choices = json.GetNamedArray(L"choices");

                std::vector<Chat::ChatChoice> chatChoiceList{};

                for (auto jsonValue : choices)
                {
                    auto obj = jsonValue.GetObject();
                    auto index = static_cast<uint32_t>(obj.GetNamedNumber(L"index"));
                    auto finishReason = obj.GetNamedString(L"finish_reason");

                    auto message_obj = obj.GetNamedObject(L"message");
                    auto message_role = message_obj.GetNamedString(L"role");
                    auto message_content = message_obj.GetNamedString(L"content");

                    auto message = winrt::make<Chat::implementation::ChatMessage>(message_role, message_content);

                    chatChoiceList.push_back(
                        winrt::make<OpenAI::Chat::implementation::ChatChoice>(index, finishReason, message));
                }

                co_return winrt::make<OpenAI::Chat::implementation::ChatResponse>(
                    id,
                    object,
                    created,
                    chatChoiceList);
            }
        }
        else
        {
            auto error = co_await GetErrorFromMessageAsync(response);
            co_return winrt::make<OpenAI::Chat::implementation::ChatResponse>(error);
        }
    }

    WF::IAsyncOperation<WWH::HttpResponseMessage> OpenAiService::PerformHttpRequestAsync(OpenAI::BaseRequest const request)
    {
        if (!(m_isInternetAvailable = ::Utils::Connection::IsInternetConnected()))
        {
            co_return nullptr;
        }

        if (m_openAiOptions != nullptr && request.IsValid())
        {
            auto openAiKey = m_openAiOptions_impl->OpenAiKey();

            // Create an HTTP client to make the API request
            WWH::HttpClient httpClient;
            httpClient.DefaultRequestHeaders().Authorization(
                WWH::Headers::HttpCredentialsHeaderValue(L"Bearer", openAiKey));

            auto httpRequest = request.BuildHttpRequest();

            try
            {
                // Send the request and retrieve the response
                co_return co_await httpClient.SendRequestAsync(httpRequest);
            }
            catch (winrt::hresult const& ex)
            {
                // TODO:
            }
        }

        co_return nullptr;
    }

    WF::IAsyncOperation<WDJ::JsonObject> OpenAiService::ParseHttpMsgToJsonAsync(WWH::HttpResponseMessage const msg)
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