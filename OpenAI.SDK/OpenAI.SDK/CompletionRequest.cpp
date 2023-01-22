#include "pch.h"

#include "CompletionRequest.h"
#if __has_include("Completion.CompletionRequest.g.cpp")
#include "Completion.CompletionRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include "EnumUtils.h"


namespace winrt::OpenAI::Completion::implementation
{
	WWH::HttpRequestMessage CompletionRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/completions"));

		winrt::hstring model = ::OpenAI::Converters::ModelTypeToString(Model());

		auto prompt = L"{\"model\": \"" + model + L"\", \"prompt\": \"" + Prompt() + L"\"}";
		WWH::HttpStringContent content(prompt, winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}