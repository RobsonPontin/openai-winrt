#pragma once

#include "Moderation.ModerationRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Moderation::implementation
{
	struct ModerationRequest : ModerationRequestT<ModerationRequest, BaseRequest>
	{
		ModerationRequest() {}

		/// <summary>
		/// The input text to classify
		/// </summary>
		WFC::IVector<winrt::hstring> InputAsList()
		{
			auto result{ winrt::single_threaded_vector<winrt::hstring>(std::move(m_inputAsList)) };
			return result;
		}

		void InputAsList(WFC::IVector<winrt::hstring> value)
		{
			std::vector<winrt::hstring> strVector;
			strVector.assign(std::cbegin(value), std::cend(value));

			m_inputAsList = strVector;
		}

		/// <summary>
		/// The input text to classify
		/// </summary>
		winrt::hstring Input()
		{
			return m_input;
		}
		void Input(winrt::hstring value)
		{
			m_input = value;
		}

		WFC::IVector<winrt::hstring> InputCalculated()
		{
			if (Input() != L"" && InputAsList() != nullptr)
			{
				// TODO: throw ValidationException("Input and InputAsList can not be assigned at the same time. One of them is should be null.");
			}

			if (Input() != L"")
			{
				std::vector<winrt::hstring> inputList{};
				for (auto const& word : Input())
				{
					
					inputList.push_back(&word);
				}

				auto result{ winrt::single_threaded_vector<winrt::hstring>(std::move(inputList)) };
				return result;
			}

			return InputAsList();

		}

		bool IsValid()
		{
			if (Input() != L"" || m_inputAsList.size() > 0)
			{
				return true;
			}

			return false;
		}
		
		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		std::vector<winrt::hstring> m_inputAsList;
		winrt::hstring m_input = L"";
	};
}

namespace winrt::OpenAI::Moderation::factory_implementation
{
	struct ModerationRequest : ModerationRequestT<ModerationRequest, implementation::ModerationRequest>
	{
	};
}