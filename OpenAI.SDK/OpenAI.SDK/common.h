#pragma once

// Forward declaration
namespace winrt
{
	namespace Windows
	{
		namespace Foundation {
			namespace Collections {}
		}
		namespace Storage {
			namespace Streams {}
		}
		namespace Graphics {
			namespace Imaging {}
		}

		namespace UI {
			namespace Xaml {
				namespace Controls {}
			}
		}

		namespace Web {
			namespace Http {}
		}

		namespace Data {
			namespace Json {}
		}

		namespace Networking {
			namespace Connectivity {}
		}
	}
}

namespace WF = winrt::Windows::Foundation;
namespace WFC = winrt::Windows::Foundation::Collections;
namespace WS = winrt::Windows::Storage;
namespace WSS = winrt::Windows::Storage::Streams;
namespace WGI = winrt::Windows::Graphics::Imaging;
namespace WWH = winrt::Windows::Web::Http;
namespace WDJ = winrt::Windows::Data::Json;
namespace WNC = winrt::Windows::Networking::Connectivity;