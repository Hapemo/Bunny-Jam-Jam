#pragma once
#include "singleton.h"
#include "pch.h"
#include <winsock2.h>


struct CLIENT_INFO
{
	SOCKET				hClientSocket;
	struct sockaddr_in	clientAddr;

	char				username[516];
	HANDLE				semaphore;
};

struct S_CLIENT
{
	SOCKET clientSocket;
	std::string username;
};

struct S_SERVER
{
	struct sockaddr_in	m_ServerInfo;
	std::string			m_ServerAddress;
	int					m_ServerPort;
};

class ServerManager : public Singleton<ServerManager> {
public:

	bool serverInit(std::string serverIPAddress, u_short serverPortNumber);
	bool serverSendData(std::string data);
	bool serverRecvData();
	void serverClose();

	bool InitWinSock2_0();

	// Helper functions
	bool IsUsernameTaken(std::string const& username);
	void SendMsg(CLIENT_INFO const& receiver, std::string const& msg);
	void BroadcastMessage(CLIENT_INFO const& sender, std::string const& msg);
	void QuitMessage(std::string const& username);
	void DisplayAllUsers(CLIENT_INFO const& ReqClient);
	
//private:
	SOCKET						m_ServerSocket;
	S_SERVER					m_ServerInstance;
	std::vector<CLIENT_INFO>	m_ClientList;
	std::string					m_ServerIPAddress;
	std::string					m_ServerStringBuffer;
	u_short						m_ServerPort;
};


BOOL WINAPI ClientServerThread(LPVOID lpData);
BOOL WINAPI ClientThread(LPVOID lpData);