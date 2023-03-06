#pragma once

#include "Edits.EditsResponse.g.h"
#include "Edits.EditsChoice.g.h"
#include "Edits.EditsUsage.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Edits::implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, OpenAI::implementation::BaseResponse>
	{
		EditsResponse() {}
	    EditsResponse(OpenAI::ResponseError const& error);
		EditsResponse(std::vector<Edits::EditsChoice> editsChoices, Edits::EditsUsage const& editsUsage);

		WF::Collections::IVector<Edits::EditsChoice> Choices()
		{
			auto editsChoices_copy = m_editsChoices;
			auto result{ winrt::single_threaded_vector<Edits::EditsChoice>(std::move(editsChoices_copy)) };
			return result;
		}

		Edits::EditsUsage Usage()
		{
			return m_editsUsage;
		}

	private:
		std::vector<Edits::EditsChoice> m_editsChoices;
		Edits::EditsUsage m_editsUsage{ nullptr };
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

	struct EditsUsage : EditsUsageT<EditsUsage>
	{
		EditsUsage() {}
		EditsUsage(uint32_t promptTokens, uint32_t completionTokens, uint32_t totalTokens);

		uint32_t PromptTokens()
		{
			return m_promptTokens;
		}

		uint32_t CompletionTokens()
		{
			return m_completionTokens;
		}

		uint32_t TotalTokens()
		{
			return m_totalTokens;
		}

	private:
		uint32_t m_promptTokens = 0;
		uint32_t m_completionTokens = 0;
		uint32_t m_totalTokens = 0;
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

	struct EditsUsage : EditsUsageT<EditsUsage, implementation::EditsUsage>
	{
	};
}