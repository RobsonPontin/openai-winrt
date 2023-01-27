#pragma once

#pragma once

#include "Moderation.ModerationResponse.g.h"


namespace winrt::OpenAI::Moderation::implementation
{
	struct ModerationResponse : ModerationResponseT<ModerationResponse>
	{
		ModerationResponse() {}

		bool IsResponseSuccess()
		{
			return false;
		}
	};
}

namespace winrt::OpenAI::Moderation::factory_implementation
{
	struct ModerationResponse : ModerationResponseT<ModerationResponse, implementation::ModerationResponse>
	{
	};
}