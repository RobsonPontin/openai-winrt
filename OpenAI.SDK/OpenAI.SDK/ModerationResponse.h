#pragma once

#include "Moderation.ModerationResponse.g.h"
#include "Moderation.ModerationValue.g.h"


namespace winrt::OpenAI::Moderation::implementation
{
	struct ModerationResponse : ModerationResponseT<ModerationResponse, BaseResponse>
	{
		ModerationResponse() {}
		ModerationResponse(winrt::hstring id, std::vector<Moderation::ModerationValue> const& moderationValues);

		bool IsResponseSuccess()
		{
			if (Id() != L"" && m_moderationValues.size() > 0)
			{
				return true;
			}

			return false;
		}

		winrt::hstring Id()
		{
			return m_id;
		}

		WF::Collections::IVector<Moderation::ModerationValue> Data()
		{
			auto result{ winrt::single_threaded_vector<Moderation::ModerationValue>(std::move(m_moderationValues)) };
			return result;
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