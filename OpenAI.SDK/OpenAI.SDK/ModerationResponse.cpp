#include "pch.h"

#include "ModerationResponse.h"
#if __has_include("Moderation.ModerationResponse.g.cpp")
#include "Moderation.ModerationResponse.g.cpp"
#endif

#if __has_include("Moderation.ModerationValue.g.cpp")
#include "Moderation.ModerationValue.g.cpp"
#endif

namespace winrt::OpenAI::Moderation::implementation
{
	ModerationResponse::ModerationResponse(winrt::hstring id, std::vector<Moderation::ModerationValue> const& moderationValues)
	{
		m_id = id;
		m_moderationValues = moderationValues;
	}

	ModerationResponse::ModerationResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	ModerationValue::ModerationValue(ModerationCategory category, bool flagged, double score)
	{
		m_category = category;
		m_flagged = flagged;
		m_score = score;
	}
}