#pragma once

#include "ResponseError.g.h"


namespace winrt::OpenAI::implementation
{
    struct ResponseError : ResponseErrorT<ResponseError>
    {
        ResponseError() = default;
        ResponseError(winrt::hstring const& message);
        ResponseError(winrt::hstring const& code, winrt::hstring const& message, winrt::hstring const& type);

        winrt::hstring Code()
        {
            return m_errorCode;
        }

        winrt::hstring Message()
        {
            return m_errorMessage;
        }

        winrt::hstring Type()
        {
            return m_errorType;
        }

    private:
        winrt::hstring m_errorCode = L"Unknown";
        winrt::hstring m_errorMessage = L"Unknown";
        winrt::hstring m_errorType = L"Unknown";
    };
}

namespace winrt::OpenAI::factory_implementation
{
    struct ResponseError : ResponseErrorT<ResponseError, implementation::ResponseError>
    {
    };
}
