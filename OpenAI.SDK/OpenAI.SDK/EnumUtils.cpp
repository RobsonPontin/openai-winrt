#include "pch.h"
#include "EnumUtils.h"


namespace OpenAI::Converters
{
    static const std::map<winrt::OpenAI::ModelType, winrt::hstring> ModelTypesNames =
    {
        { winrt::OpenAI::ModelType::image_alpha_001, L"image-alpha-001" },
        { winrt::OpenAI::ModelType::text_embedding_ada_002,L"text-embedding-ada-002" },
        { winrt::OpenAI::ModelType::text_davinci_003, L"text-davinci-003" },
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
}