#pragma once
#include "ModelResponse.g.h"
#include "ModelValue.g.h"

#include "BaseResponse.h"

namespace winrt::OpenAI::implementation
{
	struct ModelResponse : ModelResponseT< ModelResponse, OpenAI::implementation::BaseResponse>
	{
		ModelResponse() {}
		ModelResponse(std::vector<OpenAI::ModelValue> models, hstring object);	
		ModelResponse(OpenAI::ResponseError const& error);

		winrt::hstring Object()
		{
			return m_object;
		}

		WF::Collections::IVector<OpenAI::ModelValue> Data()
		{
			auto models_cpy = m_models;
			WF::Collections::IVector<OpenAI::ModelValue> result
			{ 
				winrt::single_threaded_vector<OpenAI::ModelValue>(std::move(models_cpy))
			};
			return result;
		}

		bool IsResponseSuccess()
		{
			if (m_models.size() > 0)
			{
				return true;
			}

			return false;
		}

	private:
		winrt::hstring m_object = L"";
		std::vector<OpenAI::ModelValue> m_models;
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

		// TODO: permission

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
