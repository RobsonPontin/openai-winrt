#include "pch.h"

#include "StringUtils.h"
#include <cwctype>

namespace Utils::String
{
    winrt::hstring ConvertToLowerCase(winrt::hstring str)
    {
        std::wstring lowerExtension(str.c_str());
        std::transform(lowerExtension.begin(), lowerExtension.end(), lowerExtension.begin(), std::towlower);
        return winrt::hstring{ lowerExtension };
    }
}