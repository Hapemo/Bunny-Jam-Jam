#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ClientManager.h"


// init winsock2
// set up client UDP socket
bool ClientManager::clientInit(std::string serverIPAddress, u_short serverPortNumber, std::string name)
{
    if (!InitWinSock2_0()) {
		std::cout << "Unable to initialize Windows Socket Environment. Error Code: " << WSAGetLastError() << std::endl;
        return false;
    }
    
    // Create the Client UDP Socket 
    m_ClientInstance.clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    m_ClientInstance.username = name;
	if (m_ClientInstance.clientSocket == INVALID_SOCKET)
	{
		std::cout << "Unable to create Client UDP Socket. Error Code: " << WSAGetLastError() << std::endl;
        WSACleanup();
		return false;
	}

	//!< Everything past here means that it is a valid socket
    m_ServerInstance.m_ServerInfo.sin_addr.s_addr    = inet_addr(serverIPAddress.c_str());
    m_ServerInstance.m_ServerInfo.sin_family         = AF_INET;
    m_ServerInstance.m_ServerInfo.sin_port           = htons(serverPortNumber);

    //!! == EXPERIMENT SENDING CLIENT STRUCT TO SERVER HERE AS A CONNECTION REQUEST
    std::string sConnectionSetup = "Client Connected";
    int nLength = strlen(sConnectionSetup.c_str());
    if (sendto(m_ClientInstance.clientSocket, sConnectionSetup.c_str(), nLength, 0, (sockaddr*)&serverIPAddress, sizeof(serverIPAddress) == -1))
    {
        std::cout << "ERROR: Unable to send initial connection request message\n";
        return false;
    }

    return true;
}

// private function to initialize winsock2
bool ClientManager::InitWinSock2_0()
{
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 0);

    if (!WSAStartup(wVersion, &wsaData))
        return true;

    return false;
}

// Receive message from server. Might need to change the recv to recvfrom
bool ClientManager::clientRecvData()
{
    char buffer[1024];

    while (1)
    {
        if (m_StopReceive)
            return false;

        int nLength = recv(m_ClientInstance.clientSocket, buffer, sizeof(buffer), 0);
        if (nLength > 0)
        {
            if (nLength < static_cast<int>(sizeof(buffer)))
                buffer[nLength] = '\0';

            std::cout << buffer << std::endl;
        }
    }

    return true;
}


// Send message to the server.
bool ClientManager::clientSendData(std::string Data)
{
    int nSendCnt = 0;
	int nLength = strlen(Data.c_str());
	char* pBuffer = (char*)Data.c_str();
    
    while ((nSendCnt = sendto(m_ClientInstance.clientSocket, pBuffer, nLength, 0, (sockaddr*)&m_ServerInstance.m_ServerAddress, sizeof(m_ServerInstance.m_ServerAddress))) != nLength)
    {
        if (nSendCnt == -1)
        {
            std::cout << "Error sending the data to server" << std::endl;
            return false;
        }

        if (nSendCnt == nLength)
            break;

        pBuffer += nSendCnt;
        nLength -= nSendCnt;
    }

    return true;
}

void ClientManager::clientClose()
{
	closesocket(m_ClientInstance.clientSocket);
	WSACleanup();
}


















