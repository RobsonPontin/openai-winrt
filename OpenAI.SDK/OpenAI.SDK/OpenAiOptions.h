#pragma once

#include "OpenAiOptions.g.h"

namespace winrt::OpenAI::implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions>
	{
		OpenAiOptions() {}

		void SetOpenAikey(hstring key)
		{
			m_openAiKey = key;
		}

		hstring OpenAiKey()
		{
			return m_openAiKey;
		}

		OpenAI::ModelType Model()
		{
			return m_modelType;
		}

	private:
		hstring m_openAiKey{ L"" };
		OpenAI::ModelType m_modelType{ ModelType::image_alpha_001 };
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions, implementation::OpenAiOptions>
	{
	};
}

