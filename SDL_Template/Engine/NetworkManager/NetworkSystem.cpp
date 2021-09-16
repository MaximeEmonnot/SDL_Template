#include "NetworkSystem.h"
Network::NetworkSystem::NetworkSystem(const std::string& ip, Uint16 localPort, Uint16 remotePort)
{
	if (SDLNet_Init() < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Initialisation.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	udpSock = SDLNet_UDP_Open(localPort);
	if (!udpSock) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client Socket Opening.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	if (SDLNet_ResolveHost(&udpIpAddress, ip.c_str(), remotePort) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client UDP Host Resolution.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	if (SDLNet_UDP_Bind(udpSock, -1, &udpIpAddress) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client UDP Binding.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	CreatePackets(65536);
}

Network::NetworkSystem::~NetworkSystem()
{
	SDLNet_UDP_Close(udpSock);
	SDLNet_Quit();
}

void Network::NetworkSystem::TCPWaitForClient()
{
	type = TCPSocketType::Host;
	if (SDLNet_ResolveHost(&tcpIpAddress, NULL, 9999) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client TCP Host Resolution.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	tcpSockPersonnal = SDLNet_TCP_Open(&tcpIpAddress);
	if (!tcpSockPersonnal)
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during TCP Socket Opening.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);

	while (1) {
		tcpSockGuest = SDLNet_TCP_Accept(tcpSockPersonnal);
		if (tcpSockGuest) break;
	}
	printf("Client connected!\n");
}

void Network::NetworkSystem::TCPJoinHost()
{
	type = TCPSocketType::Client;
	if (SDLNet_ResolveHost(&tcpIpAddress, "127.0.0.1", 9999) < 0) {
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during Client TCP Host Resolution.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	}
	tcpSockPersonnal = SDLNet_TCP_Open(&tcpIpAddress);
	if (!tcpSockPersonnal)
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during TCP Socket Opening.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);

}

void Network::NetworkSystem::CloseTCP()
{
	type = TCPSocketType::None;
	SDLNet_TCP_Close(tcpSockPersonnal);
	SDLNet_TCP_Close(tcpSockGuest);
}

void Network::NetworkSystem::SendDataTCP(const std::vector<Uint8>& data)
{
	switch (type) {
	case TCPSocketType::Client:
	{
		int result = SDLNet_TCP_Send(tcpSockPersonnal, data.data(), data.size());
	}
		break;
	case TCPSocketType::Host:
	{
		int result = SDLNet_TCP_Send(tcpSockGuest, data.data(), data.size());
	}
		break;
	}
}

std::vector<Uint8> Network::NetworkSystem::RecieveDataTCP()
{
	std::vector<Uint8> output;

	Uint8* data = new Uint8[1024];	// I wanna puke and commit crimes because of this C ptr but I can't do anything else
	
	int len = 0;
	switch (type) {
	case TCPSocketType::Host:
		len = SDLNet_TCP_Recv(tcpSockGuest, data, 1024);
		break;
	case TCPSocketType::Client:
		len = SDLNet_TCP_Recv(tcpSockPersonnal, data, 1024); 
		break;
	}
	printf("Length : %d\n", len);

	output.assign(data, data + len);
	delete[] data;
	return output;
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
	pPacketIn->address.host = udpIpAddress.host;
	pPacketIn->address.port = udpIpAddress.port;
	//Output Packet
	pPacketOut.reset(SDLNet_AllocPacket(size), SDLNet_FreePacket);
	if (pPacketOut == nullptr)
		throw EngineException("SDL Net Exception caught", __FILE__, "An error has been caught during SDL Net Packet Allocation.\nMore informations: " + std::string(SDLNet_GetError()), __LINE__);
	pPacketOut->address.host = udpIpAddress.host;
	pPacketOut->address.port = udpIpAddress.port;

}
