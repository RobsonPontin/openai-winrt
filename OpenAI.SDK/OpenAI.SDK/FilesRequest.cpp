#include "pch.h"
#include "FilesRequest.h"

#if __has_include("FilesRequest.g.cpp")
#include "FilesRequest.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	WWH::HttpRequestMessage FilesRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/files"));

		return request;
	}
}