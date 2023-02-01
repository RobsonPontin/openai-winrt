#pragma once

#include "ResponseError.g.h"


namespace winrt::OpenAI::implementation
{
    struct ResponseError : ResponseErrorT<ResponseError>
    {
        ResponseError() = default;
        ResponseError(winrt::hstring errorMessage);

        winrt::hstring ErrorMessage()
        {
            return m_errorMessage;
        }

    private:
        winrt::hstring m_errorMessage = L"";
    };
}

namespace winrt::OpenAI::factory_implementation
{
    struct ResponseError : ResponseErrorT<ResponseError, implementation::ResponseError>
    {
    };
}
