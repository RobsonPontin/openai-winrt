#pragma once

#include <winrt/Windows.Storage.Streams.h>

namespace OpenAI_SDK
{
    struct OpenAiService
    {
    public:

        OpenAiService()
        {}

        /// <summary>
        /// Generates an images based off keywords
        /// </summary>
        /// <param name="keywords">A range of keywords</param>
        /// <param name="openAiKey">The OpenAI secret key</param>
        /// <returns></returns>
        WF::IAsyncOperation<WS::Streams::IBuffer> GenerateDalleImageAsync(std::vector<winrt::hstring> keywords);

        WF::IAsyncOperation<WS::Streams::IBuffer> FileToPngBufferAsync(WS::StorageFile const& file);

        WF::IAsyncOperation<WS::Streams::IBuffer> GenerateDalleVariantAsync(WS::StorageFile const& file);

        // Utils
        WF::IAsyncOperation<WS::StorageFile> CreateFileFromImageAsync(WS::Streams::IBuffer buffer, bool saveToTempFolder);


        void SetOpenAiSecretKey(winrt::hstring key)
        {
            m_openAiSecretKey = key;
        }

        bool IsRunning()
        {
            return m_isServiceRunning;
        }

        private:
            WS::StorageFile m_imageCreated{ nullptr };
            WS::StorageFile m_imageVariant{ nullptr };

            bool m_isServiceRunning{ false };
            winrt::hstring m_openAiSecretKey{ L"" };
    };
}