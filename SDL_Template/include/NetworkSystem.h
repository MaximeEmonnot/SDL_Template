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
	private:
		enum class TCPSocketType {
			Host,
			Client,
			None
		};

	public:
		NetworkSystem() = default;
		NetworkSystem(const std::string& ip, Uint16 localPort, Uint16 remotePort);
		~NetworkSystem();

		void TCPWaitForClient();
		void TCPJoinHost();
		void CloseTCP();

		void SendDataTCP(const std::vector<Uint8>& data);
		std::vector<Uint8> RecieveDataTCP();

		void SendPackage(const std::vector<Uint8>& data);
		std::vector<Uint8> RecievePackage();

	private:
		void CreatePackets(int size);

	public:
		std::shared_ptr<CoreSystem::Window> pWnd;
	private:
		TCPSocketType type = TCPSocketType::None;

		IPaddress udpIpAddress;
		IPaddress tcpIpAddress;
		UDPsocket udpSock;
		TCPsocket tcpSockPersonnal;
		TCPsocket tcpSockGuest;
		std::shared_ptr<UDPpacket> pPacketOut;
		std::shared_ptr<UDPpacket> pPacketIn;
	};
}