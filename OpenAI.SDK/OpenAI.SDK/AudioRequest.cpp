#include "pch.h"

#include "AudioRequest.h"
#if __has_include("Audio.AudioRequest.g.cpp")
#include "Audio.AudioRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include "winrt/Windows.Storage.Streams.h"
#include "StringUtils.h"


namespace winrt::OpenAI::Audio::implementation
{
	std::map<hstring,hstring> _supportedFileFormats =
	{
		{ L"mp3", L"audio" },
		{ L"mpga", L"audio" },
		{ L"wav", L"audio" },
		{ L"m4a", L"audio" },
		{ L"mp4", L"video" },
		{ L"mpeg", L"video" },
		{ L"webm", L"video" },
		{ L"unknown", L"unknown" }
	};

	AudioRequest::AudioRequest()
	{
	}

	bool AudioRequest::IsValid()
	{
		if (m_audioFile != nullptr
			&& Prompt() != L""
			&& Model() != L"")
		{
			if (TryGetHttpContentTypeForFile() != L"")
			{
				return true;
			}
		}

		return false;
	}

	WWH::HttpRequestMessage AudioRequest::BuildHttpRequest()
	{
		WWH::HttpBufferContent bContent{ m_audioBuffer };

		auto contentType = TryGetHttpContentTypeForFile();
		bContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(contentType));

		WWH::HttpMultipartFormDataContent multipartContent{};

		multipartContent.Add(bContent, L"model", Model());

		auto reqType = L"https://api.openai.com/v1/audio/transcriptions";
		if (ResquetType() == AudioRequestType::Translation)
		{
			reqType = L"https://api.openai.com/v1/audio/translations";
		}

		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(reqType));
		
		request.Content(multipartContent);

		return request;
	}

	WF::IAsyncAction AudioRequest::SetAudioFileAsync(WS::StorageFile const file)
	{
		if (file == nullptr)
		{
			throw winrt::hresult_invalid_argument(L"File cannot be null.");
		}

		// TODO: check supported formats, perhaps do conversion

		try
		{			
			auto audioStream = co_await file.OpenReadAsync();
			WS::Streams::Buffer buffer{ static_cast<uint32_t>(audioStream.Size()) };
			audioStream.WriteAsync(buffer);

			m_audioBuffer = buffer;
		}
		catch (winrt::hresult_error const&)
		{
			// TODO: Log error
		}
	}

	winrt::hstring AudioRequest::TryGetHttpContentTypeForFile()
	{
		auto fileFormat = ::Utils::String::ConvertToLowerCase(m_audioFile.FileType());
		if (fileFormat != L"")
		{
			auto it = _supportedFileFormats.find(fileFormat);
			if (it == _supportedFileFormats.end())
			{
				return (it->second + L"/" + it->first);
			}
		}

		return L"";
	}
}