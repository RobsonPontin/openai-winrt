#pragma once

#include "OpenAiService.h"

namespace OpenAI::Converters
{
	winrt::hstring ModelTypeToString(winrt::OpenAI::ModelType model);
}