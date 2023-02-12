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

		/// <summary>
		/// What sampling temperature to use, between 0 and 2. 
		/// Higher values like 0.8 will make the output more random, 
		/// while lower values like 0.2 will make it more focused and deterministic.
		/// </summary>
		void GenerationNumber(uint16_t val)
		{
			m_generationNumber = val;
		}

		double Temperature()
		{
			return m_temperature;
		}

		void Temperature(double val)
		{
			if (val >= MAX_TEMPERATURE_VALUE)
			{
				m_temperature = MAX_TEMPERATURE_VALUE;
			}
			else
			{
				m_temperature = val;
			}
		}

		/// <summary>
		/// An alternative to sampling with temperature, 
		/// called nucleus sampling, where the model considers 
		/// the results of the tokens with top_p probability mass. 
		/// So 0.1 means only the tokens comprising the top 10% probability mass are considered.
		/// </summary>
		double TopP()
		{
			return m_topP;
		}

		void TopP(double val)
		{
			m_topP = val;
		}

		bool IsValid()
		{
			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		const double MAX_TEMPERATURE_VALUE = 2.0;

		winrt::hstring m_input{ L"" };
		winrt::hstring m_instruction{ L"" };
		uint16_t m_generationNumber = 1;
		double m_temperature = 1.0;
		double m_topP = 1.0;
		OpenAI::ModelType m_model{ ModelType::text_davinci_003 };
	};
}

namespace winrt::OpenAI::Edits::factory_implementation
{
	struct EditsRequest : EditsRequestT<EditsRequest, implementation::EditsRequest>
	{
	};
}