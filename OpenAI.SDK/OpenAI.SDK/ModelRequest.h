#pragma once
#include "ModelRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::implementation
{
	struct ModelRequest : ModelRequestT<ModelRequest, BaseRequest>
	{
		ModelRequest() {}

		winrt::hstring Model()
		{
			return m_model;
		}
		void Model(winrt::hstring val)
		{
			m_model = val;
		}

		bool IsValid()
		{
			return true;
		}

		WWH::HttpRequestMessage BuildHttpRequest();
	
	private:
		winrt::hstring m_model{ L"" };
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct ModelRequest : ModelRequestT<ModelRequest, implementation::ModelRequest>
	{
	};
}