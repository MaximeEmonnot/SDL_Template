#include "NetworkSystem.h"
Network::NetworkSystem::NetworkSystem(const std::string& ip, Uint16 localPort, Uint16 remotePort)
	:
	pWnd(CoreSystem::Window::GetInstance())
{
	if (SDLNet_Init() < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Initialisation.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	udpSock = SDLNet_UDP_Open(localPort);
	if (!udpSock) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client Socket Opening.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	if (SDLNet_ResolveHost(&ipAdress, ip.c_str(), remotePort) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client Host Resolution.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	if (SDLNet_UDP_Bind(udpSock, -1, &ipAdress) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client UDP Binding.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	CreatePackets(65536);
}

Network::NetworkSystem::~NetworkSystem()
{
	SDLNet_UDP_Close(udpSock);
	SDLNet_Quit();
}

void Network::NetworkSystem::SendPackage(const std::vector<Uint8>& data)
{
	std::copy(data.begin(), data.end(), pPacketOut->data);

	pPacketOut->len = data.size();
	if (SDLNet_UDP_Send(udpSock, -1, pPacketOut.get()) == 0)
	{
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Packet Sending.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
}

std::vector<Uint8> Network::NetworkSystem::RecievePackage()
{
	std::vector<Uint8> output;

	int result = SDLNet_UDP_Recv(udpSock, pPacketIn.get());

	switch (result) {
	case -1:
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Packet Reception.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
		break;
	case 0:
		break;
	case 1:
		for (size_t i = 0; i < pPacketIn->len; i++) {
			output.emplace_back(pPacketIn->data[i]);
		}
		break;
	default:
		break;
	}
	return output;
}

void Network::NetworkSystem::CreatePackets(int size)
{
	//Input Packet
	pPacketIn.reset(SDLNet_AllocPacket(size), SDLNet_FreePacket);
	if (pPacketIn == nullptr)
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Packet Allocation.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	pPacketIn->address.host = ipAdress.host;
	pPacketIn->address.port = ipAdress.port;
	//Output Packet
	pPacketOut.reset(SDLNet_AllocPacket(size), SDLNet_FreePacket);
	if (pPacketOut == nullptr)
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Packet Allocation.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	pPacketOut->address.host = ipAdress.host;
	pPacketOut->address.port = ipAdress.port;

}
