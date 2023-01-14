#pragma once

#include "common.h"
#include "winrt/Windows.Storage.h"
#include "winrt/Windows.Storage.Streams.h"

namespace OpenAI::Utils
{
	struct Convert
	{
		static WF::IAsyncOperation<WS::Streams::IBuffer> FileToPngBufferAsync(WS::StorageFile const& file);
	};

}
