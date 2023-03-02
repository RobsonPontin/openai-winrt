#include "pch.h"
#include "FilesResponse.h"

#if __has_include("FilesResponse.g.cpp")
#include "FilesResponse.g.cpp"
#endif

#if __has_include("FileValue.g.cpp")
#include "FileValue.g.cpp"
#endif


namespace winrt::OpenAI::implementation
{
	FilesResponse::FilesResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}