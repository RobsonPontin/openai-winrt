#include "pch.h"

#include "EditsResponse.h"
#if __has_include("Edits.EditsResponse.g.cpp")
#include "Edits.EditsResponse.g.cpp"
#endif

#if __has_include("Edits.EditsChoice.g.cpp")
#include "Edits.EditsChoice.g.cpp"
#endif

namespace winrt::OpenAI::Edits::implementation
{
	EditsResponse::EditsResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	EditsResponse::EditsResponse(std::vector<Edits::EditsChoice> editsChoices)
	{
		m_editsChoices = editsChoices;
	}

	EditsChoice::EditsChoice(uint32_t index, winrt::hstring text)
	{
		m_index = index;
		m_text = text;
	}
}