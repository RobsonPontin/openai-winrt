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
	ChatResponse::ChatResponse(hstring id, hstring object, hstring finishReason, double created)
	{
		m_id = id;
		m_object = object;
		m_finishReason = finishReason;
		m_created = created;
	}

	ChatResponse::ChatResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	ChatChoice::ChatChoice(uint32_t index, Chat::ChatMessage const& message)
	{
		m_index = index;
		m_message = message;
	}
}