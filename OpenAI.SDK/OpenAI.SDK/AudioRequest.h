#pragma once

#include "Audio.AudioRequest.g.h"

#include "winrt/Windows.Web.Http.h"
#include "winrt/Windows.Storage.h"


namespace winrt::OpenAI::Audio::implementation
{
	struct AudioRequest : AudioRequestT<AudioRequest, BaseRequest>
	{
		AudioRequest();

		WWH::HttpRequestMessage BuildHttpRequest();

		bool IsValid();

		/// <summary>
		/// The audio file object (not file name), in one of these formats: mp3, mp4, mpeg, mpga, m4a, wav, or webm.
		/// </summary>
		WS::StorageFile AudioFile()
		{
			return m_audioFile;
		}

		void AudioFile(WS::StorageFile const& val)
		{
			m_audioFile = val;
		}

		/// <summary>
		/// The type of Audio request to be made.
		/// </summary>
		AudioRequestType ResquetType()
		{
			return m_audioReqType;
		}

		void RequestType(AudioRequestType val)
		{
			m_audioReqType = val;
		}
		
		/// <summary>
		/// ID of the model to use. Only whisper-1 is currently available.
		/// </summary>
		winrt::hstring Model()
		{
			return m_model;
		}
		void Model(winrt::hstring val)
		{
			m_model = val;
		}

		/// <summary>
		/// An optional text to guide the model's style or continue a previous audio segment. The prompt should match the audio language.
		/// </summary>
		winrt::hstring Prompt()
		{
			return m_prompt;
		}
		void Prompt(winrt::hstring val)
		{
			m_prompt = val;
		}

		/// <summary>
		/// The format of the transcript output, in one of these options: json, text, srt, verbose_json, or vtt.
		/// </summary>
		winrt::hstring ResponseFormat()
		{
			return m_responseFormat;
		}
		void ResponseFormat(winrt::hstring val)
		{
			m_responseFormat = val;
		}

		/// <summary>
		/// The sampling temperature, between 0 and 1. 
		/// Higher values like 0.8 will make the output more random, while lower values like 0.2 will make it more focused and deterministic. 
		/// If set to 0, the model will use log probability to automatically increase the temperature until certain thresholds are hit.
		/// </summary>
		double Temperature()
		{
			return m_temperature;
		}
		void Temperature(double val)
		{
			m_temperature = val;
		}

		/// <summary>
		/// The language of the input audio. Supplying the input language in ISO-639-1 format will improve accuracy and latency.
		/// </summary>
		/// <remarks>
		/// Only used if the request type is "Translation".
		/// </remarks>
		winrt::hstring Language()
		{
			return m_language;
		}
		void Language(winrt::hstring val)
		{
			m_language = val;
		}

	private:
		AudioRequestType m_audioReqType{ AudioRequestType::Transcription };
		winrt::hstring m_model{ L"whisper-1" };
		winrt::hstring m_prompt{ L"" };
		winrt::hstring m_responseFormat{ L"" };
		winrt::hstring m_language{ L"" };
		double m_temperature = 1.0;
		WS::StorageFile m_audioFile{ nullptr };
		WSS::IBuffer m_audioBuffer{ nullptr };

		winrt::hstring TryGetHttpContentTypeForFile();
	};
}

namespace winrt::OpenAI::Audio::factory_implementation
{
	struct AudioRequest : AudioRequestT<AudioRequest, implementation::AudioRequest>
	{
	};
}
