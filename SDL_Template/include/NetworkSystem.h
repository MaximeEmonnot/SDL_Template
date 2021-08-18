#pragma once
#include <SDL_net.h>
#include <vector>
#include "SingletonMaker.h"
#include "EngineException.h"
#include "Window.h"

#define SERVER 0
#define CLIENT 1

namespace Network {
	class NetworkSystem : public CoreSystem::SingletonMaker<NetworkSystem>
	{
	public:
		NetworkSystem(const std::string& ip, Uint16 localPort, Uint16 remotePort);
		~NetworkSystem();

		void SendPackage(const std::vector<Uint8>& data);
		std::vector<Uint8> RecievePackage();

	private:
		void CreatePackets(int size);

	public:
		std::shared_ptr<CoreSystem::Window> pWnd;
	protected:
		IPaddress ipAdress;
		UDPsocket udpSock;
		std::shared_ptr<UDPpacket> pPacketOut;
		std::shared_ptr<UDPpacket> pPacketIn;
	};
}