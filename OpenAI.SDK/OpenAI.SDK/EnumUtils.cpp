#include "pch.h"
#include "EnumUtils.h"

using namespace winrt::OpenAI::Image;

namespace Utils::Converters
{
    static const std::map<winrt::OpenAI::ModelType, winrt::hstring> ModelTypesNames =
    {
        { winrt::OpenAI::ModelType::image_alpha_001, L"image-alpha-001" },
        { winrt::OpenAI::ModelType::text_embedding_ada_002,L"text-embedding-ada-002" },
        { winrt::OpenAI::ModelType::text_davinci_003, L"text-davinci-003" },
		{ winrt::OpenAI::ModelType::text_davinci_edit_001, L"text-davinci-edit-001" },
		{ winrt::OpenAI::ModelType::code_davinci_edit_001, L"code-davinci-edit-001" },
		{ winrt::OpenAI::ModelType::text_moderation_stable, L"text-moderation-stable" },
		{ winrt::OpenAI::ModelType::text_moderation_latest, L"text-moderation-latest" },
		{ winrt::OpenAI::ModelType::gpt_3_5_turbo, L"gpt-3.5-turbo"},
    };

    winrt::hstring ModelTypeToString(winrt::OpenAI::ModelType model)
    {
        auto it = ModelTypesNames.find(model);
        if (it == ModelTypesNames.end())
        {
            return L"";
        }
        else
        {
            return it->second;
        }
    }

	winrt::hstring ResponseFormatToString(ResponseFormatType format)
	{
		switch (format)
		{
		case ResponseFormatType::Url:
			return L"url";

		case ResponseFormatType::b64_json:
			return L"b64_json";
		}

		return L"";
	}

	winrt::hstring ImageSizeToString(SizeType size)
	{
		switch (size)
		{
		case SizeType::Size1024:
			return L"1024x1024";
		}

		return L"";
	}
}