#pragma once

#include "Image.ImageRequest.g.h"
#include "Image.ImageCreateRequest.g.h"
#include "Image.ImageVariationRequest.g.h"
#include "Image.ImageEditRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Image::implementation
{
	struct ImageRequest : ImageRequestT<ImageRequest, BaseRequest>
	{
		winrt::hstring Prompt()
		{
			return m_prompt;
		}
		void Prompt(winrt::hstring val)
		{
			m_prompt = val;
		}

		winrt::hstring ImageName()
		{
			return m_imageName;
		}
		void ImageName(winrt::hstring val)
		{
			m_imageName = val;
		}

		int GenerationNumber()
		{
			return m_generationNumber;
		}
		void GenerationNumber(int val)
		{
			if (val >= MAX_GENERATION_NUMBER)
			{
				m_generationNumber = MAX_GENERATION_NUMBER;
			}
			else
			{
				m_generationNumber = val;
			}
		}

		SizeType Size()
		{
			return m_size;
		}
		void Size(SizeType val)
		{
			m_size = val;
		}

		void ResponseFormat(ResponseFormatType val)
		{
			m_responseFormat = val;
		}
		ResponseFormatType ResponseFormat()
		{
			return m_responseFormat;
		}

		winrt::hstring User()
		{
			return m_user;
		}

		void User(winrt::hstring val)
		{
			m_user = val;
		}

		virtual bool IsValid()
		{
			return false; 
		}

		virtual WWH::HttpRequestMessage BuildHttpRequest() { return nullptr; }

	private:
		/// <summary>
		/// Max number of images to generate
		/// </summary>
		const int MAX_GENERATION_NUMBER = 10;

		winrt::hstring m_prompt{ L"" };
		winrt::hstring m_imageName{ L"" };
		winrt::hstring m_user = L"";
		int m_generationNumber{ 1 };
		SizeType m_size{ SizeType::Size1024 };
		ResponseFormatType m_responseFormat{ ResponseFormatType::Url };

	};

	struct ImageCreateRequest : ImageCreateRequestT<ImageCreateRequest, ImageRequest>
	{
		ImageCreateRequest() {}
		
		bool IsValid()
		{
			if(Prompt() != L"")
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();
	};

	struct ImageVariationRequest : ImageVariationRequestT<ImageVariationRequest, ImageRequest>
	{
		ImageVariationRequest() {}
		
		WF::IAsyncOperation<bool> SetImageAsync(WS::StorageFile const& file);

		bool IsValid()
		{
			if (m_imageBuffer != nullptr)
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		WSS::IBuffer m_imageBuffer{ nullptr };
	};

	struct ImageEditRequest : ImageEditRequestT<ImageEditRequest, ImageRequest>
	{
		ImageEditRequest() {}

		WF::IAsyncOperation<bool> SetImageAsync(WS::StorageFile const& file);

		WF::IAsyncOperation<bool> SetMaskAsync(WS::StorageFile const& file);
		WF::IAsyncOperation<bool> SetMaskAsync(WSS::IBuffer const& buffer);

		bool IsValid()
		{
			if (m_imageBuffer != nullptr
				&& Prompt() != L"")
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		WSS::IBuffer m_imageBuffer{ nullptr };
		WSS::IBuffer m_imageMask{ nullptr };
	};
}

namespace winrt::OpenAI::Image::factory_implementation
{
	struct ImageRequest
	{
	};

	struct ImageCreateRequest : ImageCreateRequestT<ImageCreateRequest, implementation::ImageCreateRequest>
	{
	};

	struct ImageVariationRequest : ImageVariationRequestT<ImageVariationRequest, implementation::ImageVariationRequest>
	{
	};

	struct ImageEditRequest : ImageEditRequestT<ImageEditRequest, implementation::ImageEditRequest>
	{
	};
}