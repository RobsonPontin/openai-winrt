#include "pch.h"

#include "ChatResponse.h"
#if __has_include("Chat.ChatResponse.g.cpp")
#include "Chat.ChatResponse.g.cpp"
#endif

#if __has_include("Chat.ChatChoice.g.cpp")
#include "Chat.ChatChoice.g.cpp"
#endif


namespace winrt::OpenAI::Chat::implementation
{
	ChatResponse::ChatResponse(hstring id, hstring object, double created, std::vector<Chat::ChatChoice> chatChoices)
	{
		m_id = id;
		m_object = object;
		m_created = created;
		m_chatChoices = chatChoices;
	}

	ChatResponse::ChatResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	ChatChoice::ChatChoice(uint32_t index, winrt::hstring finish_reason, Chat::ChatMessage const& message)
	{
		m_index = index;
		m_finishReason = finish_reason;
		m_message = message;
	}
}