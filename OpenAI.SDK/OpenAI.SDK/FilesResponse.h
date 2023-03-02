#pragma once
#include "FilesResponse.g.h"
#include "FileValue.g.h"
#include "BaseResponse.h"

namespace winrt::OpenAI::implementation
{
	struct FilesResponse : FilesResponseT< FilesResponse, OpenAI::implementation::BaseResponse>
	{
		FilesResponse() {}

		FilesResponse(OpenAI::ResponseError const& error);

	private:
		// TODO: List of Data with files 
		// std::vector<OpenAI::FileValue> m_filesData;
	};

	struct FileValue : FileValueT<FileValue>
	{
		FileValue() {}

		winrt::hstring Id()
		{
			return m_id;
		}

	private:
		winrt::hstring m_id = L"";
		winrt::hstring m_object = L"";
		winrt::hstring m_fileName = L"";
		winrt::hstring m_purpose = L"";
		uint32_t m_bytesCount = 0;
		double m_createdAt = 0;
	};
}

namespace winrt::OpenAI::factory_implementation
{
	struct FilesResponse : FilesResponseT<FilesResponse, implementation::FilesResponse>
	{
	};

	struct FileValue : FileValueT<FileValue, implementation::FileValue>
	{
	};
}