#include "pch.h"

#include "CompletionRequest.h"
#if __has_include("Completion.CompletionRequest.g.cpp")
#include "Completion.CompletionRequest.g.cpp"
#endif

#include "winrt/Windows.Web.Http.Headers.h"


namespace winrt::OpenAI::Completion::implementation
{
	winrt::hstring ModelToString(ModelType model)
	{
		switch (model)
		{
		case ModelType::image_alpha_001:
			return L"image-alpha-001";

		case ModelType::text_embedding_ada_002:
			return L"text-embedding-ada-002";

		case ModelType::text_davinci_003:
			return L"text-davinci-003";
		}

		return L"";
	}

	WWH::HttpRequestMessage CompletionRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/completions"));

		winrt::hstring model = ModelToString(Model());

		auto prompt = L"{\"model\": \"" + model + L"\", \"prompt\": \"" + Prompt() + L"\"}";
		WWH::HttpStringContent content(prompt, winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}
}