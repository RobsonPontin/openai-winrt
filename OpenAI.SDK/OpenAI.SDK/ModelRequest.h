#pragma once
#include "ModelRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::implementation
{
	struct ModelRequest : ModelRequestT<ModelRequest, BaseRequest>
	{
		ModelRequest() {}

		OpenAI::ModelType Model()
		{
			return m_model;
		}
		void Model(OpenAI::ModelType val)
		{
			m_model = val;
		}

		bool IsValid()
		{
			return true;
		}

		WWH::HttpRequestMessage BuildHttpRequest();
	
	private:
		OpenAI::ModelType m_model{ ModelType::Unknown };
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct ModelRequest : ModelRequestT<ModelRequest, implementation::ModelRequest>
	{
	};
}