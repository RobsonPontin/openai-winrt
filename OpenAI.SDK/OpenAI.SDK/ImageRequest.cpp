#include "pch.h"

#include "ImageRequest.h"
#if __has_include("Image.ImageRequest.g.cpp")
#include "Image.ImageRequest.g.cpp"
#endif

#if __has_include("Image.ImageCreateRequest.g.cpp")
#include "Image.ImageCreateRequest.g.cpp"
#endif

#if __has_include("Image.ImageVariationRequest.g.cpp")
#include "Image.ImageVariationRequest.g.cpp"
#endif

#if __has_include("Image.ImageEditRequest.g.cpp")
#include "Image.ImageEditRequest.g.cpp"
#endif

#include "ImageUtils.h"
#include "winrt/Windows.Web.Http.Headers.h"

namespace winrt::OpenAI::Image::implementation
{
	// Image Generation
	WWH::HttpRequestMessage ImageCreateRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/images/generations"));

		winrt::hstring response_format = L"url"; // Either "url" or "b64_json"

		auto prompt = L"{\"model\": \"image-alpha-001\", \"prompt\": \"" + Prompt() + L"\", \"num_images\":1, \"size\":\"1024x1024\", \"response_format\": \"" + response_format + L"\"}";
		WWH::HttpStringContent content(prompt, winrt::Windows::Storage::Streams::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}

	// Image Variant
	WF::IAsyncOperation<bool> ImageVariationRequest::SetImageAsync(WS::StorageFile file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			m_imageBuffer = imgBuffer;

			co_return true;
		}

		co_return false;
	}

	WWH::HttpRequestMessage ImageVariationRequest::BuildHttpRequest()
	{
		WWH::HttpBufferContent bContent{ m_imageBuffer };
		bContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"image/png"));

		WWH::HttpMultipartFormDataContent multipartContent{};

		auto fileName = ImageName() == L"" ? L"untitled" : ImageName();
		multipartContent.Add(bContent, L"image", fileName);

		winrt::hstring boundary = winrt::to_hstring(Windows::Foundation::GuidHelper::CreateNewGuid()); // create unique ID
		multipartContent.Headers().TryAppendWithoutValidation(L"Content-Type", L"multipart/form-data; boundary=" + boundary);

		// Set up the API endpoint and parameters            
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/images/variations"));

		request.Content(multipartContent);

		return request;
	}

	// Image Edit
	WF::IAsyncOperation<bool> ImageEditRequest::SetImageAsync(WS::StorageFile file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			m_imageBuffer = imgBuffer;

			co_return true;
		}

		co_return false;
	}
}