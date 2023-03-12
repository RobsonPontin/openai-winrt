#pragma once

#include "Chat.ChatResponse.g.h"
#include "Chat.ChatChoice.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Chat::implementation
{
	struct ChatResponse : ChatResponseT<ChatResponse, OpenAI::implementation::BaseResponse>
	{
		ChatResponse() {}
		ChatResponse(
			hstring id,
			hstring object,
			double created,
			std::vector<Chat::ChatChoice> chatChoices);
		ChatResponse(OpenAI::ResponseError const& error);

		bool IsResponseSuccess()
		{
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

		WF::Collections::IVector<Chat::ChatChoice> Choices()
		{
			auto chatChoises_cpy = m_chatChoices;
			auto result{ winrt::single_threaded_vector<Chat::ChatChoice>(std::move(chatChoises_cpy)) };
			return result;
		}

	private:
		hstring m_id = L"";
		hstring m_object = L"";
		double m_created = 0;
		std::vector<Chat::ChatChoice> m_chatChoices;
	};

	struct ChatChoice : ChatChoiceT<ChatChoice>
	{
		ChatChoice() = default;
		ChatChoice(uint32_t index, winrt::hstring finish_reason, Chat::ChatMessage const& message);

		uint32_t Index()
		{
			return m_index;
		}

		Chat::ChatMessage Message()
		{
			return m_message;
		}

		winrt::hstring FinishReason()
		{
			return m_finishReason;
		}

	private:
		hstring m_finishReason = L"";
		uint32_t m_index = 0;
		OpenAI::Chat::ChatMessage m_message{ nullptr };
	};
}

namespace winrt::OpenAI::Chat::factory_implementation
{
	struct ChatResponse : ChatResponseT<ChatResponse, implementation::ChatResponse>
	{
	};

	struct ChatChoice : ChatChoiceT<ChatChoice, implementation::ChatChoice>
	{
	};
}