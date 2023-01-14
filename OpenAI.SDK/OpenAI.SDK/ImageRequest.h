#pragma once

#include "Image.ImageRequest.g.h"
#include "Image.ImageCreateRequest.g.h"
#include "Image.ImageVariationRequest.g.h"
#include "Image.ImageEditRequest.g.h"

namespace winrt::OpenAI::Image::implementation
{
	struct ImageRequest : ImageRequestT<ImageRequest>
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

		int CreationNumber()
		{
			return m_generationNumber;
		}
		void CreationNumber(int val)
		{
			m_generationNumber = val;
		}

		SizeType Size()
		{
			return m_size;
		}
		void Size(SizeType val)
		{
			m_size = val;
		}

		bool IsValid() { return false; }

	private:
		winrt::hstring m_prompt{ L"" };
		winrt::hstring m_imageName{ L"" };
		int m_generationNumber{ 1 };
		SizeType m_size{ SizeType::Size1024 };
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
	};

	struct ImageVariationRequest : ImageVariationRequestT<ImageVariationRequest, ImageRequest>
	{
		ImageVariationRequest() {}

		bool IsValid()
		{
			if (Prompt() != L"")
			{
				return true;
			}

			return false;
		}
	};

	struct ImageEditRequest : ImageEditRequestT<ImageEditRequest, ImageRequest>
	{
		ImageEditRequest() {}

		bool IsValid()
		{
			if (Prompt() != L"")
			{
				return true;
			}

			return false;
		}
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