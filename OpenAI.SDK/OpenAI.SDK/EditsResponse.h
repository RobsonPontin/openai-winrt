#pragma once

#include "Edits.EditsResponse.g.h"
#include "Edits.EditsValue.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::Edits::implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, OpenAI::implementation::BaseResponse>
	{
		EditsResponse() {}
	    EditsResponse(OpenAI::ResponseError const& error);
		EditsResponse(std::vector<Edits::EditsValue> editsValues);

		WF::Collections::IVector<Edits::EditsValue> Choices()
		{
			auto result{ winrt::single_threaded_vector<Edits::EditsValue>(std::move(m_editsValues)) };
			return result;
		}

	private:
		std::vector<Edits::EditsValue> m_editsValues;
	};

	struct EditsValue : EditsValueT<EditsValue>
	{
		EditsValue() {}
		EditsValue(uint32_t index, winrt::hstring text);

		uint32_t Index()
		{
			return m_index;
		}

		winrt::hstring Text()
		{
			return m_text;
		}

	private:
		winrt::hstring m_text = L"";
		uint32_t m_index = 0;
	};
}

namespace winrt::OpenAI::Edits::factory_implementation
{
	struct EditsResponse : EditsResponseT<EditsResponse, implementation::EditsResponse>
	{
	};

	struct EditsValue : EditsValueT<EditsValue, implementation::EditsValue>
	{
	};
}