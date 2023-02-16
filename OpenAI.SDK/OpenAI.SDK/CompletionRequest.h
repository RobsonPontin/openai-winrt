#pragma once

#include "Completion.CompletionRequest.g.h"

#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Completion::implementation
{
	struct CompletionRequest : CompletionRequestT<CompletionRequest, BaseRequest>
	{
		CompletionRequest() {}

		OpenAI::ModelType Model()
		{
			return m_model;
		}
		void Model(OpenAI::ModelType val)
		{
			m_model = val;
		}
		
		winrt::hstring Prompt()
		{
			return m_prompt;
		}
		void Prompt(winrt::hstring val)
		{
			m_prompt = val;
		}

		winrt::hstring Suffix()
		{
			return m_suffix;
		}

		void Suffix(winrt::hstring val)
		{
			m_suffix = val;
		}

		uint32_t MaxTokens()
		{
			return m_maxTokens;
		}

		void MaxTokens(uint32_t val)
		{
			m_maxTokens = val;
		}

		uint16_t GenerationNumber()
		{
			return m_generationNumber;
		}

		void GenerationNumber(uint16_t val)
		{
			m_generationNumber = val;
		}

		bool Stream()
		{
			return m_stream;
		}

		void Stream(bool val)
		{
			m_stream = val;
		}

		/// <summary>
		/// Include the log probabilities on the logprobs most likely tokens, as well the chosen tokens. 
		/// For example, if logprobs is 5, the API will return a list of the 5 most likely tokens. 
		/// The API will always return the logprob of the sampled token, s
		/// </summary>
		/// <returns></returns>
		uint32_t Logprobs()
		{
			return m_logProbs;
		}

		void Logprobs(uint32_t val)
		{
			m_logProbs = val;
		}

		/// <summary>
		/// Number between -2.0 and 2.0. 
		/// Positive values penalize new tokens based on whether they appear in the text so far, 
		/// increasing the model's likelihood to talk about new topics.
		/// </summary>
		/// <returns></returns>
		double PresencePenalty()
		{
			return m_presencePenalty;
		}

		void PresencePenalty(double val)
		{
			if (val >= MAX_PRESENCE_PENALTY)
			{
				m_presencePenalty = MAX_PRESENCE_PENALTY;
			}
			else if (val <= MIN_PRESENCE_PENALTY)
			{
				m_presencePenalty = MIN_PRESENCE_PENALTY;
			}
			else
			{
				m_presencePenalty = val;
			}
		}

		/// <summary>
		/// Number between -2.0 and 2.0. 
		/// Positive values penalize new tokens based on their existing frequency in the text so far,
		/// decreasing the model's likelihood to repeat the same line verbatim.
		/// </summary>
		/// <returns></returns>
		double FrequencyPenalty()
		{
			return m_frequencyPenalty;
		}

		void FrequencyPenalty(double val)
		{
			if (val >= MAX_FREQUENCY_PENALTY)
			{
				m_frequencyPenalty = MAX_PRESENCE_PENALTY;
			}
			else if (val <= MIN_FREQUENCY_PENALTY)
			{
				m_frequencyPenalty = MIN_FREQUENCY_PENALTY;
			}
			else
			{
				m_frequencyPenalty = val;
			}
		}

		bool IsValid()
		{
			if (Prompt() != L"")
			{
				return true;
			}

			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		const double MAX_PRESENCE_PENALTY = 2.0;
		const double MIN_PRESENCE_PENALTY = -2.0;
		const double MAX_FREQUENCY_PENALTY = 2.0;
		const double MIN_FREQUENCY_PENALTY = -2.0;

		winrt::hstring m_prompt{ L"" };
		winrt::hstring m_suffix{ L"" };
		winrt::hstring m_stop{ L"" };
		uint32_t m_maxTokens = 0;
		uint32_t m_logProbs = 0;
		uint16_t m_generationNumber = 1;
		uint16_t m_bestOf = 1;
		double m_temperature = 0.0;
		double m_topP = 0.0;
		double m_presencePenalty = 0.0;
		double m_frequencyPenalty = 0.0;
		bool m_stream = false;
		bool m_echo = false;
		OpenAI::ModelType m_model{ ModelType::text_davinci_003 };
		// TODO: logit_bias - which needs to be a map
	};
}

namespace winrt::OpenAI::Completion::factory_implementation
{
	struct CompletionRequest : CompletionRequestT<CompletionRequest, implementation::CompletionRequest>
	{
	};
}