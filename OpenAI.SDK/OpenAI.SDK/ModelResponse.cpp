#include "pch.h"

#include "ModelResponse.h"
#if __has_include("ModelResponse.g.cpp")
#include "ModelResponse.g.cpp"
#endif

#if __has_include("ModelValue.g.cpp")
#include "ModelValue.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	ModelResponse::ModelResponse(std::vector<OpenAI::ModelValue> models, hstring object)
	{
		m_models = models;
		m_object = object;
	}

	ModelResponse::ModelResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}

	ModelValue::ModelValue(hstring id, hstring object, hstring ownedBy)
	{
		m_id = id;
		m_object = object;
		m_ownedBy = ownedBy;
	}
}