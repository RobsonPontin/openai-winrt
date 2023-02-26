#pragma once

#include "Completion.CompletionResponse.g.h"
#include "Completion.CompletionChoice.g.h"
#include "Completion.CompletionUsage.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Completion::implementation
{
	struct CompletionResponse : CompletionResponseT<CompletionResponse, OpenAI::implementation::BaseResponse>
	{
		CompletionResponse() {}
		CompletionResponse(
			hstring id, 
			hstring object, 
			double created, 
			hstring model, 
			std::vector<Completion::CompletionChoice> completionChoices,
			Completion::CompletionUsage const& completionUsage);
		
		CompletionResponse(OpenAI::ResponseError const& error);

		bool IsResponseSuccess()
		{
			if (m_completionUsage != nullptr 
				&& m_completionChoices.size() > 0)
			{
				return true;
			}

			return false;
		}

		winrt::hstring Id()
		{
			return m_id;
		}

		winrt::hstring Object()
		{
			return m_object;
		}

		double Created()
		{
			return m_created;
		}

		hstring Model()
		{
			return m_model;
		}

		WF::Collections::IVector<Completion::CompletionChoice> Choices()
		{
			auto result{ winrt::single_threaded_vector<Completion::CompletionChoice>(std::move(m_completionChoices)) };
			return result;
		}

		Completion::CompletionUsage Usage()
		{
			return m_completionUsage;
		}

	private:
		hstring m_id = L"";
		hstring m_object = L"";
		hstring m_model = L"";
		double m_created = 0;

		Completion::CompletionUsage m_completionUsage{ nullptr };
		std::vector<Completion::CompletionChoice> m_completionChoices;
	};

	struct CompletionChoice : CompletionChoiceT<CompletionChoice>
	{
		CompletionChoice() {}
		CompletionChoice(winrt::hstring text, hstring finish_reason, uint32_t index);

		winrt::hstring Text()
		{
			return m_text;
		}

		winrt::hstring FinishReason()
		{
			return m_finishReason;
		}

		uint32_t Index()
		{
			return m_index;
		}

	private:
		winrt::hstring m_text = L"";
		winrt::hstring m_finishReason = L"";
		uint32_t m_index = 0;
	};

	struct CompletionUsage : CompletionUsageT<CompletionUsage>
	{
		CompletionUsage() {}
		CompletionUsage(uint32_t promptTokens, uint32_t completionTokens, uint32_t totalTokens);

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

namespace winrt::OpenAI::Completion::factory_implementation
{
	struct CompletionResponse : CompletionResponseT<CompletionResponse, implementation::CompletionResponse>
	{
	};

	struct CompletionChoice : CompletionChoiceT<CompletionChoice, implementation::CompletionChoice>
	{
	};

	struct CompletionUsage : CompletionUsageT<CompletionUsage, implementation::CompletionUsage>
	{
	};
}