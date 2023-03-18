#pragma once
#include "singleton.h"
#include "pch.h"

#include <winsock2.h>

struct C_CLIENT
{
	SOCKET clientSocket;
	std::string username;
};

struct C_SERVER
{
	struct sockaddr_in	m_ServerInfo;
	std::string			m_ServerAddress;
	int					m_ServerPort;
};

//class ClientManager
class ClientManager : public Singleton<ClientManager>
{
public:

	ClientManager();
	~ClientManager();

	bool clientInit(std::string, u_short, std::string);
	bool clientSendData(std::string data);
	void clientClose();
	bool InitWinSock2_0();
	
	//!< Private Client Variable
	std::thread			m_ClientRecvThread;

	C_CLIENT			m_ClientInstance;
	C_SERVER			m_ServerInstance;
	char				m_ServerIPAddress[20];
	int					m_ServerPort;
	bool				m_StopReceive;
};

void clientRecvData();

