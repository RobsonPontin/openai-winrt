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

		Windows::Data::Json::JsonObject jsonObj{};
		jsonObj.Insert(L"input", WDJ::JsonValue::CreateStringValue(Input()));

		if (Model() != OpenAI::ModelType::Unknown)
		{
			winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());
			jsonObj.Insert(L"model", WDJ::JsonValue::CreateStringValue(model));
		}

		WWH::HttpStringContent content(jsonObj.ToString(), WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}