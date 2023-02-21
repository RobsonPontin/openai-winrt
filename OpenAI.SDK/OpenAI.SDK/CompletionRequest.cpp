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

		winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());

		Windows::Data::Json::JsonObject jsonObj{};
		jsonObj.Insert(L"model", WDJ::JsonValue::CreateStringValue(model));
		jsonObj.Insert(L"prompt", WDJ::JsonValue::CreateStringValue(Prompt()));
		jsonObj.Insert(L"suffix", WDJ::JsonValue::CreateStringValue(Suffix()));
		jsonObj.Insert(L"max_tokens", WDJ::JsonValue::CreateNumberValue(MaxTokens()));
		jsonObj.Insert(L"temperature", WDJ::JsonValue::CreateNumberValue(Temperature()));
		jsonObj.Insert(L"top_p", WDJ::JsonValue::CreateNumberValue(TopP()));
		jsonObj.Insert(L"n", WDJ::JsonValue::CreateNumberValue(GenerationNumber()));

		WWH::HttpStringContent content(jsonObj.ToString(), WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}