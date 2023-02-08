#include "pch.h"

#include "EmbeddingRequest.h"
#if __has_include("Embedding.EmbeddingRequest.g.cpp")
#include "Embedding.EmbeddingRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"
#include <winrt/Windows.Data.Json.h>
#include "EnumUtils.h"

namespace winrt::OpenAI::Embedding::implementation
{
	WWH::HttpRequestMessage EmbeddingRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/embeddings"));

		winrt::hstring model = ::Utils::Converters::ModelTypeToString(Model());

		auto prompt = L"{\"model\": \"" + model + L"\", \"input\": \"" + Input() + L"\"}";
		WWH::HttpStringContent content(prompt, WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}

}