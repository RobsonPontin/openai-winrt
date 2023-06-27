#pragma once

#include "Audio.AudioResponse.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Audio::implementation
{
	struct AudioResponse : AudioResponseT<AudioResponse, OpenAI::implementation::BaseResponse>
	{
		AudioResponse() = default;
		AudioResponse(OpenAI::ResponseError const& error);

		/// <summary>
		/// The transcribed audio into the requested language.
		/// </summary>
		winrt::hstring Text()
		{
			return m_text;
		}

	private:
		winrt::hstring m_text{ L"" };
	};
}

namespace winrt::OpenAI::Audio::factory_implementation
{
	struct AudioResponse : AudioResponseT<AudioResponse, implementation::AudioResponse>
	{
	};
}