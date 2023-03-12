#pragma once

#include "Chat.ChatRequest.g.h"
#include "Chat.ChatMessage.g.h"
#include "winrt/Windows.Web.Http.h"


namespace winrt::OpenAI::Chat::implementation
{
	struct ChatRequest : ChatRequestT<ChatRequest, BaseRequest>
	{
		ChatRequest() = default;
	
		winrt::hstring User()
		{
			return m_user;
		}

		void User(winrt::hstring user)
		{
			m_user = user;
		}

		WFC::IVector<Chat::ChatMessage> Messages()
		{
			return m_messages;
		}

		void Messages(WFC::IVector<Chat::ChatMessage> messages)
		{
			m_messages = messages;
		}

		uint32_t MaxTokens()
		{
			return m_maxTokens;
		}

		void MaxTokens(uint32_t val)
		{
			m_maxTokens = val;
		}

		double Temperature()
		{
			return m_temperature;
		}

		void Temperature(double val)
		{
			m_temperature = val;
		}

		double TopP()
		{
			return m_topP;
		}

		void TopP(double val)
		{
			m_topP = val;
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

		winrt::hstring Stop()
		{
			return m_stop;
		}

		void Stop(winrt::hstring val)
		{
			m_stop = val;
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
			return false;
		}

		WWH::HttpRequestMessage BuildHttpRequest();

	private:
		const double MAX_PRESENCE_PENALTY = 2.0;
		const double MIN_PRESENCE_PENALTY = -2.0;
		const double MAX_FREQUENCY_PENALTY = 2.0;
		const double MIN_FREQUENCY_PENALTY = -2.0;

		uint32_t m_maxTokens = 16;
		uint16_t m_generationNumber = 1;
		double m_temperature = 1.0;
		double m_topP = 1.0;
		double m_presencePenalty = 0.0;
		double m_frequencyPenalty = 0.0;
		bool m_stream = false;
		winrt::hstring m_stop; // NOTE: (it can be an array) Up to 4 sequences where the API will stop generating further tokens.
		winrt::hstring m_user;
		// TODO: logit_bias

		WFC::IVector<OpenAI::Chat::ChatMessage> m_messages;
	};

	struct ChatMessage : ChatMessageT<ChatMessage>
	{
		ChatMessage(winrt::hstring role, winrt::hstring content);

		winrt::hstring Role()
		{
			return m_role;
		}

		winrt::hstring Content()
		{
			return m_content;
		}

	private:
		winrt::hstring m_role;
		winrt::hstring m_content;
	};
}

namespace winrt::OpenAI::Chat::factory_implementation
{
	struct ChatRequest : ChatRequestT<ChatRequest, implementation::ChatRequest>
	{
	};

	struct ChatMessage : ChatMessageT<ChatMessage, implementation::ChatMessage>
	{
	};
}