#pragma once

#include "Edits.EditsResponse.g.h"
#include "Edits.EditsChoice.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Edits::implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, OpenAI::implementation::BaseResponse>
	{
		EditsResponse() {}
	    EditsResponse(OpenAI::ResponseError const& error);
		EditsResponse(std::vector<Edits::EditsChoice> editsChoices);

		WF::Collections::IVector<Edits::EditsChoice> Choices()
		{
			auto result{ winrt::single_threaded_vector<Edits::EditsChoice>(std::move(m_editsChoices)) };
			return result;
		}

	private:
		std::vector<Edits::EditsChoice> m_editsChoices;
	};

	struct EditsChoice : EditsChoiceT<EditsChoice>
	{
		EditsChoice() {}
		EditsChoice(uint32_t index, winrt::hstring text);

		uint32_t Index()
		{
			return m_index;
		}

		winrt::hstring Text()
		{
			return m_text;
		}

	private:
		winrt::hstring m_text = L"";
		uint32_t m_index = 0;
	};
}

namespace winrt::OpenAI::Edits::factory_implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, implementation::EditsResponse>
	{
	};

	struct EditsChoice : EditsChoiceT<EditsChoice, implementation::EditsChoice>
	{
	};
}