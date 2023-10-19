#pragma once
#include <memory>


#ifdef BOOM_PLATFORM_WINDOWS
#include "Windows/WindowsNetwork.h"
#include "Windows/WindowsClient.h"

#else

#endif


namespace Boomerang
{

	class NetworkFactory
	{
	public:
		static std::unique_ptr<Network> CreateNetwork() {
		#ifdef BOOM_PLATFORM_WINDOWS
					return std::make_unique<WindowsNetwork>();
		#else
					// Other platforms...
		#endif
		}
	};

	class NetworkInstance
	{
	public:
		static Network& GetInstance() {
			static std::unique_ptr<Network> instance = NetworkFactory::CreateNetwork();
			return *instance;
		}
	};

	class ClientFactory
	{
	public:
		static std::unique_ptr<Client> CreateNetwork() {
		#ifdef BOOM_PLATFORM_WINDOWS
					return std::make_unique<WindowsClient>();
		#else
					// Other platforms...
		#endif
		}
	};

	class ClientInstance
	{
	public:
		static Client& GetInstance() {
			static std::unique_ptr<Client> instance = ClientFactory::CreateNetwork();
			return *instance;
		}
	};
}
