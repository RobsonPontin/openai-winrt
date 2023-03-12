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
	ChatRequest::ChatRequest()
	{
		m_messages = WFC::IVector<OpenAI::Chat::ChatMessage>
		{ 
			winrt::single_threaded_vector<OpenAI::Chat::ChatMessage>() 
		};
	}

	WWH::HttpRequestMessage ChatRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/chat/completions"));

		WDJ::JsonObject jsonObj{};

		winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());
		jsonObj.Insert(L"model", WDJ::JsonValue::CreateStringValue(model));

		WDJ::JsonArray messageArray{};

		for (int i = 0; i < m_messages.Size(); ++i)
		{
			WDJ::JsonObject message{};			
			message.Insert(L"role", WDJ::JsonValue::CreateStringValue(m_messages.GetAt(i).Role()));
			message.Insert(L"content", WDJ::JsonValue::CreateStringValue(m_messages.GetAt(i).Content()));

			messageArray.Append(message);
		}

		jsonObj.Insert(L"messages", messageArray);
		auto dd = jsonObj.ToString();

		WWH::HttpStringContent content(jsonObj.ToString(), WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}

	ChatMessage::ChatMessage(winrt::hstring role, winrt::hstring content)
	{
		m_role = role;
		m_content = content;
	}
}