#pragma once

#include "Chat.ChatResponse.g.h"
#include "Chat.ChatChoice.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Chat::implementation
{
	struct ChatResponse : ChatResponseT<ChatResponse, OpenAI::implementation::BaseResponse>
	{
		ChatResponse() {}
		ChatResponse(hstring id, hstring object, hstring finishReason, double created);
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

		winrt::hstring FinishReason()
		{
			return m_finishReason;
		}

		double Created()
		{
			return m_created;
		}

	private:
		hstring m_id = L"";
		hstring m_object = L"";
		hstring m_finishReason = L"";
		double m_created = 0;
	};

	struct ChatChoice : ChatChoiceT<ChatChoice>
	{
		ChatChoice() {}
		ChatChoice(uint32_t index, Chat::ChatMessage const& message);

		uint32_t Index()
		{
			return m_index;
		}

		Chat::ChatMessage Message()
		{
			return m_message;
		}

	private:
		uint32_t m_index = 0;
		OpenAI::Chat::ChatMessage m_message;
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