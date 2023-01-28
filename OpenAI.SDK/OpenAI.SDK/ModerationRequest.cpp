#include "pch.h"

#include "ModerationRequest.h"
#if __has_include("Moderation.ModerationRequest.g.cpp")
#include "Moderation.ModerationRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include "EnumUtils.h"


namespace winrt::OpenAI::Moderation::implementation
{
	WWH::HttpRequestMessage ModerationRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/moderations"));

		auto prompt = L"{\"input\": \"" + Input() + L"\"}";
		WWH::HttpStringContent content(prompt, winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}