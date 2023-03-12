#include "pch.h"

#include "ChatRequest.h"
#if __has_include("Chat.ChatRequest.g.cpp")
#include "Chat.ChatRequest.g.cpp"
#endif

#if __has_include("Chat.ChatMessage.g.cpp")
#include "Chat.ChatMessage.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include "EnumUtils.h"


namespace winrt::OpenAI::Chat::implementation
{
	WWH::HttpRequestMessage ChatRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/chat/completions"));

		// TODO:

		return request;
	}

	ChatMessage::ChatMessage(winrt::hstring role, winrt::hstring content)
	{
		m_role = role;
		m_content = content;
	}
}