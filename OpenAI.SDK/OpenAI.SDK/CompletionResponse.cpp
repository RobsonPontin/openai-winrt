#include "pch.h"

#include "CompletionResponse.h"
#if __has_include("Completion.CompletionResponse.g.cpp")
#include "Completion.CompletionResponse.g.cpp"
#endif

#if __has_include("Completion.CompletionChoice.g.cpp")
#include "Completion.CompletionChoice.g.cpp"
#endif

#if __has_include("Completion.CompletionUsage.g.cpp")
#include "Completion.CompletionUsage.g.cpp"
#endif

namespace winrt::OpenAI::Completion::implementation
{
	// Response

	CompletionResponse::CompletionResponse(
		hstring id,
		hstring object,
		double created,
		hstring model,
		std::vector<Completion::CompletionChoice> completionChoices,
		Completion::CompletionUsage const& completionUsage)
	{
		m_id = id;
		m_object = object;
		m_created = created;
		m_model = model;
		m_completionChoices = completionChoices;
		m_completionUsage = completionUsage;
	}

	CompletionResponse::CompletionResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	// Choice

	CompletionChoice::CompletionChoice(winrt::hstring text, hstring finish_reason, uint32_t index)
	{
		m_text = text;
		m_finishReason = finish_reason;
		m_index = index;
	}

	// Usage

	CompletionUsage::CompletionUsage(uint32_t promptTokens, uint32_t completionTokens, uint32_t totalTokens)
	{
		m_promptTokens = promptTokens;
		m_completionTokens = completionTokens;
		m_totalTokens = totalTokens;
	}
}