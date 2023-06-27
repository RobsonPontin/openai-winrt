#include "pch.h"

#include "AudioResponse.h"
#if __has_include("Audio.AudioResponse.g.cpp")
#include "Audio.AudioResponse.g.cpp"
#endif

namespace winrt::OpenAI::Audio::implementation
{
	AudioResponse::AudioResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}