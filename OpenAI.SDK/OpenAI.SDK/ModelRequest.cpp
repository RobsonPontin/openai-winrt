#include "pch.h"

#include "ModelRequest.h"
#if __has_include("ModelRequest.g.cpp")
#include "ModelRequest.g.cpp"
#endif

#include "EnumUtils.h"

namespace winrt::OpenAI::implementation
{
	WWH::HttpRequestMessage ModelRequest::BuildHttpRequest()
	{
		winrt::hstring modelRequest = L"models";

		if (Model() != ModelType::Unknown)
		{
			modelRequest = L"model/" +  ::Utils::Converters::ModelTypeToString(Model());
		}

		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/" + modelRequest));

		return request;
	}
}