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
#include "EnumUtils.h"
#include "winrt/Windows.Web.Http.Headers.h"

namespace winrt::OpenAI::Image::implementation
{
	const hstring DEFAULT_IMAGE_REQUEST_MODEL = L"image-alpha-001";

	ImageRequest::ImageRequest()
	{
		m_model = DEFAULT_IMAGE_REQUEST_MODEL;
	}

	// Image Generation

	// TODO: consider changing HttpStringContent to HttpMultipartFormDataContent
	WWH::HttpRequestMessage ImageCreateRequest::BuildHttpRequest()
	{
		// Set up the API endpoint and parameters
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/images/generations"));

		winrt::hstring response_format = ::Utils::Converters::ResponseFormatToString(ResponseFormat());
		winrt::hstring imageSize = ::Utils::Converters::ImageSizeToString(Size());

		Windows::Data::Json::JsonObject jsonObj{};
		jsonObj.Insert(L"model", WDJ::JsonValue::CreateStringValue(Model()));
		jsonObj.Insert(L"prompt", WDJ::JsonValue::CreateStringValue(Prompt()));
		jsonObj.Insert(L"num_images", WDJ::JsonValue::CreateNumberValue(GenerationNumber()));
		jsonObj.Insert(L"size", WDJ::JsonValue::CreateStringValue(imageSize));
		jsonObj.Insert(L"response_format", WDJ::JsonValue::CreateStringValue(response_format));
		jsonObj.Insert(L"user", WDJ::JsonValue::CreateStringValue(User()));

		WWH::HttpStringContent content(jsonObj.ToString(), WSS::UnicodeEncoding::Utf8);
		content.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"application/json"));
		request.Content(content);

		return request;
	}

	// Image Variation
	WF::IAsyncOperation<bool> ImageVariationRequest::SetImageAsync(WS::StorageFile const& file)
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
		multipartContent.Add(WWH::HttpStringContent{ winrt::to_hstring(GenerationNumber()) }, L"num_images");

		winrt::hstring boundary = winrt::to_hstring(WF::GuidHelper::CreateNewGuid()); // create unique ID
		multipartContent.Headers().TryAppendWithoutValidation(L"Content-Type", L"multipart/form-data; boundary=" + boundary);

		// Set up the API endpoint and parameters            
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/images/variations"));

		request.Content(multipartContent);

		return request;
	}

	// Image Edit
	WF::IAsyncOperation<bool> ImageEditRequest::SetImageAsync(WS::StorageFile const& file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			m_imageBuffer = imgBuffer;

			co_return true;
		}

		co_return false;
	}

	WF::IAsyncOperation<bool> ImageEditRequest::SetMaskAsync(WS::StorageFile const& file)
	{
		auto imgBuffer = co_await ::OpenAI::Utils::Convert::FileToPngBufferAsync(file);
		if (imgBuffer)
		{
			 m_imageMask = imgBuffer;

			co_return true;
		}

		co_return false;
	}

	WF::IAsyncOperation<bool> ImageEditRequest::SetMaskAsync(WS::Streams::IBuffer const& buffer)
	{
		if (buffer != nullptr)
		{
			m_imageMask = buffer;

			co_return true;
		}

		co_return false;
	}

	WWH::HttpRequestMessage ImageEditRequest::BuildHttpRequest()
	{
		// Build Image Buffer Content
		WWH::HttpBufferContent imgContent{ m_imageBuffer };
		imgContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"image/png"));

		WWH::HttpMultipartFormDataContent multipartContent{};

		auto fileName = ImageName() == L"" ? L"untitled" : ImageName();
		multipartContent.Add(imgContent, L"image", fileName);

		// Add optional mask argument
		if (m_imageMask)
		{
			WWH::HttpBufferContent maskContent{ m_imageMask };
			maskContent.Headers().ContentType(WWH::Headers::HttpMediaTypeHeaderValue(L"image/png"));

			multipartContent.Add(maskContent, L"mask", fileName);
		}

		multipartContent.Add(WWH::HttpStringContent{ Prompt()}, L"prompt");

		winrt::hstring boundary = winrt::to_hstring(WF::GuidHelper::CreateNewGuid()); // create unique ID
		multipartContent.Headers().TryAppendWithoutValidation(L"Content-Type", L"multipart/form-data; boundary=" + boundary);

		// Set up the API endpoint and parameters            
		WWH::HttpRequestMessage request(
			WWH::HttpMethod::Post(),
			WF::Uri(L"https://api.openai.com/v1/images/edits"));

		request.Content(multipartContent);

		return request;
	}
}