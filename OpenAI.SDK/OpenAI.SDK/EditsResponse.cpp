#include "pch.h"

#include "EditsResponse.h"
#if __has_include("Edits.EditsResponse.g.cpp")
#include "Edits.EditsResponse.g.cpp"
#endif

namespace winrt::OpenAI::Edits::implementation
{
	EditsResponse::EditsResponse(OpenAI::ResponseError const& error)
	{
		m_error = error;
	}
}