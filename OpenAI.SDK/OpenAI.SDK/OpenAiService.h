#pragma once

#include "OpenAiService.g.h"

#include <winrt/Windows.Storage.Streams.h>
#include <ImageRequest.h>
#include <ImageResponse.h>


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

        /// <summary>
        /// Generates an images based off keywords
        /// </summary>
        /// <param name="keywords">A range of keywords</param>
        /// <param name="openAiKey">The OpenAI secret key</param>
        /// <returns></returns>
        WF::IAsyncOperation<WS::Streams::IBuffer> GenerateDalleImageAsync(winrt::hstring const& keywords);

        WF::IAsyncOperation<WS::Streams::IBuffer> GenerateDalleVariantAsync(WS::StorageFile const& file);

        void SetOpenAiSecretKey(winrt::hstring key)
        {
            if (m_openAiOptions != nullptr)
            {
                m_openAiOptions.SetOpenAikey(key);
            }
        }

        bool IsRunning()
        {
            return m_isServiceRunning;
        }

        private:
            WS::StorageFile m_imageCreated{ nullptr };
            WS::StorageFile m_imageVariant{ nullptr };

            bool m_isServiceRunning{ false };
            OpenAI::OpenAiOptions m_openAiOptions;

            const winrt::hstring OPEN_AI_API_URL = L"https://api.openai.com";
    };
}

namespace winrt::OpenAI::factory_implementation
{
    struct OpenAiService : OpenAiServiceT<OpenAiService, implementation::OpenAiService>
    {
    };
}