#pragma once

#include "OpenAiService.g.h"

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Storage.Streams.h>
#include <ImageRequest.h>
#include <ImageResponse.h>
#include <CompletionRequest.h>
#include <CompletionResponse.h>
#include <EmbeddingRequest.h>
#include <EmbeddingResponse.h>
#include <ModerationRequest.h>
#include <ModerationResponse.h>


namespace winrt::OpenAI::implementation
{
    struct OpenAiService : OpenAiServiceT<OpenAiService>
    {
    public:

        OpenAiService()
        {}

        OpenAiService(OpenAI::OpenAiOptions const& options)
        {
            m_openAiOptions = options;
        }

        WF::IAsyncOperation<OpenAI::Image::ImageResponse> RunRequestAsync(winrt::OpenAI::Image::ImageRequest const& imageRequest);

        WF::IAsyncOperation<OpenAI::Completion::CompletionResponse> RunRequestAsync(winrt::OpenAI::Completion::CompletionRequest const& completionRequest);

        WF::IAsyncOperation<OpenAI::Embedding::EmbeddingResponse> RunRequestAsync(winrt::OpenAI::Embedding::EmbeddingRequest const& embeddingRequest);

        WF::IAsyncOperation<OpenAI::Moderation::ModerationResponse> RunRequestAsync(winrt::OpenAI::Moderation::ModerationRequest const& moderationRequest);

        void Options(OpenAI::OpenAiOptions val)
        {
            m_openAiOptions = val;
        }
        OpenAI::OpenAiOptions Options()
        {
            return m_openAiOptions;
        }

        void SetOpenAiSecretKey(winrt::hstring key)
        {
            if (m_openAiOptions != nullptr)
            {
                m_openAiOptions.OpenAiKey(key);
            }
        }

        bool IsRunning()
        {
            return m_isServiceRunning;
        }

        private:
            bool m_isServiceRunning{ false };
            OpenAI::OpenAiOptions m_openAiOptions;

            const winrt::hstring OPEN_AI_API_URL = L"https://api.openai.com";

            WF::IAsyncOperation<WWH::HttpResponseMessage> PerformHttpRequestAsync(OpenAI::BaseRequest const& request);
            WF::IAsyncOperation<WDJ::JsonObject> ParseHttpMsgToJsonAsync(WWH::HttpResponseMessage const& msg);
    };
}

namespace winrt::OpenAI::factory_implementation
{
    struct OpenAiService : OpenAiServiceT<OpenAiService, implementation::OpenAiService>
    {
    };
}