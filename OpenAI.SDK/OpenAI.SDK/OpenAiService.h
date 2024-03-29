#pragma once

#include "OpenAiService.g.h"

#include <OpenAiOptions.h>
#include <ImageRequest.h>
#include <ImageResponse.h>
#include <CompletionRequest.h>
#include <CompletionResponse.h>
#include <EmbeddingRequest.h>
#include <EmbeddingResponse.h>
#include <ModerationRequest.h>
#include <ModerationResponse.h>
#include <EditsRequest.h>
#include <EditsResponse.h>
#include <ModelRequest.h>
#include <ModelResponse.h>
#include <FilesRequest.h>
#include <FilesResponse.h>
#include <ChatRequest.h>
#include <ChatResponse.h>
#include <AudioRequest.h>
#include <AudioResponse.h>

#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Storage.Streams.h>


namespace winrt::OpenAI::implementation
{
    struct OpenAiService : OpenAiServiceT<OpenAiService>
    {
    public:

        OpenAiService();
        OpenAiService(OpenAI::OpenAiOptions const& options);

        WF::IAsyncOperation<OpenAI::Image::ImageResponse> RunRequestAsync(winrt::OpenAI::Image::ImageRequest const imageRequest);

        WF::IAsyncOperation<OpenAI::Completion::CompletionResponse> RunRequestAsync(winrt::OpenAI::Completion::CompletionRequest const completionRequest);

        WF::IAsyncOperation<OpenAI::Embedding::EmbeddingResponse> RunRequestAsync(winrt::OpenAI::Embedding::EmbeddingRequest const embeddingRequest);

        WF::IAsyncOperation<OpenAI::Moderation::ModerationResponse> RunRequestAsync(winrt::OpenAI::Moderation::ModerationRequest const moderationRequest);

        WF::IAsyncOperation<OpenAI::Edits::EditsResponse> RunRequestAsync(winrt::OpenAI::Edits::EditsRequest const editRequest);

        WF::IAsyncOperation<OpenAI::ModelResponse> RunRequestAsync(winrt::OpenAI::ModelRequest const modelRequest);

        WF::IAsyncOperation<OpenAI::FilesResponse> RunRequestAsync(winrt::OpenAI::FilesRequest const filesRequest);

        WF::IAsyncOperation<OpenAI::Chat::ChatResponse> RunRequestAsync(winrt::OpenAI::Chat::ChatRequest const chatRequest);

        WF::IAsyncOperation<OpenAI::Audio::AudioResponse> RunRequestAsync(winrt::OpenAI::Audio::AudioRequest const audioRequest);

        void Options(OpenAI::OpenAiOptions val)
        {
            SetOpenAiOptionsInternal(val);
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
            OpenAI::OpenAiOptions m_openAiOptions{ nullptr };
            winrt::com_ptr<implementation::OpenAiOptions> m_openAiOptions_impl;
            bool m_isInternetAvailable{ false };

            const winrt::hstring OPEN_AI_API_URL = L"https://api.openai.com";

            void SetOpenAiOptionsInternal(OpenAI::OpenAiOptions const& options);

            WF::IAsyncOperation<WWH::HttpResponseMessage> PerformHttpRequestAsync(OpenAI::BaseRequest const request);
            WF::IAsyncOperation<WDJ::JsonObject> ParseHttpMsgToJsonAsync(WWH::HttpResponseMessage const msg);
            WF::IAsyncOperation<OpenAI::ResponseError> GetErrorFromMessageAsync(WWH::HttpResponseMessage const message);
    };
}

namespace winrt::OpenAI::factory_implementation
{
    struct OpenAiService : OpenAiServiceT<OpenAiService, implementation::OpenAiService>
    {
    };
}