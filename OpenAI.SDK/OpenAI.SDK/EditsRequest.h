#pragma once

#include "Edits.EditsRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Edits::implementation
{
	struct EditsRequest : EditsRequestT<EditsRequest, BaseRequest>
	{
		EditsRequest() {}

		OpenAI::ModelType Model()
		{
			return m_model;
		}
		void Model(OpenAI::ModelType val)
		{
			m_model = val;
		}

		winrt::hstring Input()
		{
			return m_input;
		}
		void Input(winrt::hstring val)
		{
			m_input = val;
		}

		winrt::hstring Instruction()
		{
			return m_instruction;
		}

		void Instruction(winrt::hstring val)
		{
			m_instruction = val;
		}

		uint16_t GenerationNumber()
		{
			return m_generationNumber;
		}

		void GenerationNumber(uint16_t val)
		{
			m_generationNumber = val;
		}

		bool IsValid()
		{
			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		winrt::hstring m_input{ L"" };
		winrt::hstring m_instruction{ L"" };
		uint16_t m_generationNumber = 1;
		OpenAI::ModelType m_model{ ModelType::text_davinci_003 };
	};
}

namespace winrt::OpenAI::Edits::factory_implementation
{
	struct EditsRequest : EditsRequestT<EditsRequest, implementation::EditsRequest>
	{
	};
}