#include "pch.h"
#include "ConnectionUtils.h"

#ifdef WIN_APP_SDK
#include <wininet.h>
#else
#include <winrt/Windows.Networking.Connectivity.h>
#endif

namespace Utils::Connection
{
	bool IsInternetConnected()
	{
		BOOL result = false;

#ifdef WIN_APP_SDK
		DWORD dwReturnedFlag;

		if (InternetGetConnectedState(&dwReturnedFlag, 0))
		{
			result = true;
		}
#else
		auto connectProfiles = WNC::NetworkInformation::GetConnectionProfiles();

		for (int i = 0; i < connectProfiles.Size(); ++i)
		{
			auto connectProfile = connectProfiles.GetAt(i);
			if (connectProfile == nullptr)
			{
				continue;
			}

			if (connectProfile.GetNetworkConnectivityLevel() == WNC::NetworkConnectivityLevel::InternetAccess)
			{
				result = true;
				break;
			}
		}
#endif

		return result;
	}
}