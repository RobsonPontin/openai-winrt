#include "pch.h"

#include "EditsRequest.h"
#if __has_include("Edits.EditsRequest.g.cpp")
#include "Edits.EditsRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include "EnumUtils.h"


namespace winrt::OpenAI::Edits::implementation
{
	WWH::HttpRequestMessage EditsRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/edits"));

		winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());

		Windows::Data::Json::JsonObject jsonObj{};
		jsonObj.Insert(L"model", WDJ::JsonValue::CreateStringValue(model));
		jsonObj.Insert(L"input", WDJ::JsonValue::CreateStringValue(Input()));
		jsonObj.Insert(L"instruction", WDJ::JsonValue::CreateStringValue(Instruction()));
		jsonObj.Insert(L"temperature", WDJ::JsonValue::CreateNumberValue(Temperature()));
		jsonObj.Insert(L"top_p", WDJ::JsonValue::CreateNumberValue(TopP()));
		jsonObj.Insert(L"n", WDJ::JsonValue::CreateNumberValue(GenerationNumber()));

		WWH::HttpStringContent content(jsonObj.ToString(), WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}