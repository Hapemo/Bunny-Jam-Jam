#pragma once
#include "singleton.h"
#include "pch.h"
#include <map>
#include <winsock2.h>


struct CLIENT_INFO
{
	SOCKET				hClientSocket;
	struct sockaddr_in	clientAddr;
	char				username[516];
	HANDLE				semaphore;
};


struct S_SERVER
{
	struct sockaddr_in	m_ServerInfo;
	std::string			m_ServerAddress;
	int					m_ServerPort;
};

class ServerManager : public Singleton<ServerManager> {
public:
	
	ServerManager();
	~ServerManager();

	bool serverInit(u_short serverPortNumber);
	bool serverSendData(std::string data);
	void serverClose();

	// Helper functions
	bool SendMsg(CLIENT_INFO const& receiver, std::string msg);
	void BroadcastMessage(CLIENT_INFO const& sender, std::string const& msg);
	
//private:
	std::map<std::string, CLIENT_INFO>	m_ClientList;
	std::thread							m_ServerRecvThread;

	SOCKET								m_ServerSocket;
	S_SERVER							m_ServerInstance;

// some fancy stuff if we want to include
	std::string							m_HostName;
};

void serverRecvData();

//BOOL WINAPI ClientServerThread(LPVOID lpData);
//BOOL WINAPI ClientThread(LPVOID lpData);
//bool InitWinSock2_0();
//bool IsUsernameTaken(std::string const& username);
//void QuitMessage(std::string const& username);
//void DisplayAllUsers(CLIENT_INFO const& ReqClient);