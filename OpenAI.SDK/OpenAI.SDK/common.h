#pragma once

// Forward declaration
namespace winrt
{
	namespace Windows
	{
		namespace Foundation {}
		namespace Storage {}
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
	}
}

namespace WF = winrt::Windows::Foundation;
namespace WS = winrt::Windows::Storage;
namespace WGI = winrt::Windows::Graphics::Imaging;
namespace WUXC = winrt::Windows::UI::Xaml::Controls;
namespace WWH = winrt::Windows::Web::Http;
namespace WDJ = winrt::Windows::Data::Json;