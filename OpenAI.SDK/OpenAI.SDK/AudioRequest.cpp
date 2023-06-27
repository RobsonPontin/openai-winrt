#include "pch.h"

#include "AudioRequest.h"
#if __has_include("Audio.AudioRequest.g.cpp")
#include "Audio.AudioRequest.g.cpp"
#endif


namespace winrt::OpenAI::Audio::implementation
{
	AudioRequest::AudioRequest()
	{
	}

	WWH::HttpRequestMessage AudioRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/audio/transcriptions"));
		
		// TODO: implementation of request

		return request;
	}

}