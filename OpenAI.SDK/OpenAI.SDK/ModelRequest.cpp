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

		if (Model() != L"")
		{
			modelRequest = L"model/" +  Model();
		}

		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Get(),
			WF::Uri(L"https://api.openai.com/v1/" + modelRequest));

		return request;
	}
}