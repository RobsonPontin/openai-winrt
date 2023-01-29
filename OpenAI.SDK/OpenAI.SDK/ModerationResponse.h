#pragma once

#include "Moderation.ModerationResponse.g.h"
#include "Moderation.ModerationValue.g.h"


namespace winrt::OpenAI::Moderation::implementation
{
	struct ModerationResponse : ModerationResponseT<ModerationResponse>
	{
		ModerationResponse() {}
		ModerationResponse(winrt::hstring id, std::vector<Moderation::ModerationValue> const& moderationValues);

		winrt::hstring Id()
		{
			return m_id;
		}

		bool IsResponseSuccess()
		{
			return false;
		}

	private:
		winrt::hstring m_id = L"";
		std::vector<Moderation::ModerationValue> m_moderationValues;
	};

	struct ModerationValue : ModerationValueT<ModerationValue>
	{
		ModerationValue() {}
		ModerationValue(ModerationCategory category, bool flagged, double score);

		ModerationCategory Category()
		{
			return m_category;
		}

		bool Flagged()
		{
			return m_flagged;
		}

		double Score()
		{
			return m_score;
		}

	private:
		ModerationCategory m_category = ModerationCategory::None;
		bool m_flagged = false;
		double m_score = 0.0;
	};
}

namespace winrt::OpenAI::Moderation::factory_implementation
{
	struct ModerationResponse : ModerationResponseT<ModerationResponse, implementation::ModerationResponse>
	{
	};

	struct ModerationValue : ModerationValueT<ModerationValue, implementation::ModerationValue>
	{
	};
}