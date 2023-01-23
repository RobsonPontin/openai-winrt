#pragma once

#include "OpenAiOptions.g.h"


namespace winrt::OpenAI::implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions>
	{
		OpenAiOptions() {}

		/// <summary>
		/// The Open AI Secrete Key used to perform HTTP requests.
		/// </summary>
		void OpenAiKey(hstring key)
		{
			m_openAiKey = key;
		}
		hstring OpenAiKey()
		{			
			return m_openAiKey;
		}

		/// <summary>
		/// It sets the Open AI secret key by trying to get it from the
		/// passed enviroment variable.
		/// </summary>
		/// <param name="enviromentVar">The enviroment variable that hold secret key</param>
		/// <returns>True if env var and key are found, false otherwise.</returns>
		bool SetOpenAiKeyFromEnvironmentVar(hstring enviromentVar);

	private:
		hstring m_openAiKey{ L"" };

		winrt::hstring TryGetOpenAiKeyFromEnvVar(winrt::hstring const& envVarName);
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct OpenAiOptions : OpenAiOptionsT<OpenAiOptions, implementation::OpenAiOptions>
	{
	};
}

