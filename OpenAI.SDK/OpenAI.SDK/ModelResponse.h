#pragma once
#include "ModelResponse.g.h"
#include "ModelValue.g.h"

#include "BaseResponse.h"

namespace winrt::OpenAI::implementation
{
	struct ModelResponse : ModelResponseT< ModelResponse, OpenAI::implementation::BaseResponse>
	{
		ModelResponse() {}

		ModelResponse(OpenAI::ResponseError const& error);
	};

	struct ModelValue : ModelValueT<ModelValue>
	{
		ModelValue() {}
		ModelValue(hstring id, hstring object, hstring ownedBy);

		winrt::hstring Id()
		{
			return m_id;
		}

		winrt::hstring Object()
		{
			return m_object;
		}

		winrt::hstring OwnedBy()
		{
			return m_ownedBy;
		}

	private:
		winrt::hstring m_id = L"";
		winrt::hstring m_object = L"";
		winrt::hstring m_ownedBy = L"";
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct ModelResponse : ModelResponseT<ModelResponse, implementation::ModelResponse>
	{
	};

	struct ModelValue : ModelValueT<ModelValue, implementation::ModelValue>
	{
	};
}
