#include "pch.h"

#include "OpenAiOptions.h"
#if __has_include("OpenAiOptions.g.cpp")
#include "OpenAiOptions.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	bool OpenAiOptions::SetOpenAiKeyFromEnvironmentVar(hstring enviromentVar)
	{
		auto openAiKey = TryGetOpenAiKeyFromEnvVar(enviromentVar);
		if (openAiKey == L"")
		{
			return false;
		}

		m_openAiKey = openAiKey;

		return true;
	}

	winrt::hstring OpenAiOptions::TryGetOpenAiKeyFromEnvVar(winrt::hstring const& envVarName)
	{
		winrt::hstring strResult = L"";

		if (envVarName == L"")
		{
			return strResult;
		}

		LPCWSTR envVar = envVarName.c_str();

		// Get the value of the "TEMP" environment variable
		DWORD bufferSize = GetEnvironmentVariableW(envVar, NULL, 0);

		if (bufferSize > 0)
		{
			wchar_t* buffer = new wchar_t[bufferSize];
			auto result = GetEnvironmentVariableW(envVar, buffer, bufferSize);
			if (result > 0)
			{
				strResult = winrt::hstring{ buffer };
			}
		}
		else if (bufferSize == 0)
		{
			auto errorCode = GetLastError();
			LPWSTR errorMessage = nullptr;

			FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER
				| FORMAT_MESSAGE_FROM_SYSTEM
				| FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&errorMessage,
				0,
				NULL);

			hstring error = to_hstring(errorMessage);
			// TODO: log error? env var does not exist, etc..
		}

		return strResult;
	}
}