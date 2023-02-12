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
			WF::Uri(L"https://api.openai.com/v1/embeddings"));

		winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());

		auto prompt = L"{\"model\": \"" + model + L"\", \"input\": \"" + Input() + L"\", \"instruction\": \"" + Instruction() + L"\"}";
		WWH::HttpStringContent content(prompt, WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}