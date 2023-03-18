#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ServerManager.h"
#include <WS2tcpip.h>

ServerManager::ServerManager()
    : m_ServerRecvThread(serverRecvData) {}

ServerManager::~ServerManager() {
    m_ServerRecvThread.join();
}

// Create a Server Socket
bool ServerManager::serverInit(u_short serverPortNumber)
{
    if (!InitWinSock2_0())
    {
		std::cout << "Unable to initialize Windows Socket Environment: " << WSAGetLastError() << std::endl;
		return false;
    }

    // Creating a Socket
    m_ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Obtaining hostname
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    m_HostName = std::string(hostname);

    // Obtaining my own IP Address, as a server
    struct addrinfo hints, * info;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_DGRAM;
    hints.ai_protocol   = IPPROTO_UDP;

    if (getaddrinfo(hostname, NULL, &hints, &info) != 0) {
        std::cout << "SERVER:: getaddrinfo failed with an error.\n";
        WSACleanup();
        return false;
    }

    char ipstr[INET_ADDRSTRLEN];
    struct sockaddr_in* addr = (struct sockaddr_in*)info->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));      // convert the address to a string
    m_ServerIPAddress = std::string(ipstr);
    
    if (m_ServerSocket == INVALID_SOCKET)
    {
        std::cout << "Unable to initialize Server Socket\n";
        WSACleanup();
        return false;
    }
    else {
		std::cout << "Chat Server Socket Initialized\n";
    }
    
    //!< Binding the Server socket to the address & port
    int reiterations = 0;
    do
    {
        if (++reiterations != 1) {
            std::cout << "Error Code: " << WSAGetLastError() << " - ";
            std::cout << "Unable to connect to " << inet_ntoa(m_ServerInstance.m_ServerInfo.sin_addr) << " port " << ntohs(m_ServerInstance.m_ServerInfo.sin_port) << std::endl;
            std::cout << "Retrying with port number: " << ++serverPortNumber << "\n";
            //closesocket(m_ServerSocket);
            //WSACleanup();
            //return false;
        }

        m_ServerInstance.m_ServerInfo.sin_family = AF_INET;                                     // The address family. MUST be AF_INET
        m_ServerInstance.m_ServerInfo.sin_addr.s_addr = inet_addr(m_ServerIPAddress.c_str());   // converts a string containing ipv4 address into a proper address for the IN_ADDR struct  
        m_ServerInstance.m_ServerInfo.sin_port = htons(serverPortNumber);                       // converts a u_short from host to TCP/IP network byte order (which is big-endian)
    } while (bind(m_ServerSocket, (struct sockaddr*)&m_ServerInstance.m_ServerInfo, sizeof(m_ServerInstance.m_ServerInfo)) == SOCKET_ERROR);

    std::cout << "Server Connected. Information: " << inet_ntoa(m_ServerInstance.m_ServerInfo.sin_addr) << " port " << ntohs(m_ServerInstance.m_ServerInfo.sin_port) << std::endl;
    return true;
}

bool ServerManager::serverSendData(std::string data)
{
    // Broadcast data to all clients
    for (CLIENT_INFO const& c : m_ClientList) {
        if (SendMsg(c, data) == false)
            return false;
    }

    return true;
}

void serverRecvData()
{
    int nLength = 0;
    int nFromLength = 1024;
    char cBuffer[1024];

    CLIENT_INFO localClient{};

    nLength = recvfrom(ServerManager::GetInstance()->m_ServerSocket, cBuffer, sizeof(cBuffer), 0, (sockaddr*)&localClient.clientAddr, &nFromLength);
    if (nLength >= 0)
    {
        cBuffer[nLength] = '\0';
        std::cout << "Server Received: " << cBuffer << "\n";
    }
}

void ServerManager::serverClose()
{
    closesocket(m_ServerSocket);
    WSACleanup();
}

bool ServerManager::IsUsernameTaken(std::string const& username)
{
    for (size_t i = 0; i < m_ClientList.size(); ++i)
    {
        if (m_ClientList[i].username == username)
        {
            return true;
        }
    }
    return false;
}

bool ServerManager::SendMsg(CLIENT_INFO const& receiver, std::string const& msg)
{
    char buffer[1024];
    strcpy(buffer, msg.c_str());
    char* pBuffer = buffer;

    int msgLength = static_cast<int>(msg.size());
    int nCntSend{};
    
    while ((nCntSend = sendto(receiver.hClientSocket, pBuffer, msgLength, 0, (sockaddr*)&m_ServerInstance.m_ServerInfo, sizeof(m_ServerInstance.m_ServerInfo)) != msgLength))
    {
        if (nCntSend == -1) {
            std::cout << "Error sending the data to " << inet_ntoa(receiver.clientAddr.sin_addr) << std::endl;
            break;
        }

        if (nCntSend == msgLength)
            break;
        
        // error sending data
        if (nCntSend == SOCKET_ERROR) {
            std::cout << "Error sending the data to " << inet_ntoa(receiver.clientAddr.sin_addr) << std::endl;
            closesocket(receiver.hClientSocket);
            WSACleanup();
            return false;
        }

        pBuffer += nCntSend;
        msgLength -= nCntSend;
    }

    return true;
}

void ServerManager::BroadcastMessage(CLIENT_INFO const& sender, std::string const& msg)
{
    std::string message = "[" + std::string(sender.username) + ":] " + msg;
    for (CLIENT_INFO const& c : m_ClientList)
    {
        // Don't broadcast message to self
        if (c.username == sender.username)
            continue;

        SendMsg(c, message);
    }
}

void ServerManager::QuitMessage(std::string const& username)
{
    for (size_t i = 0; i < m_ClientList.size(); ++i)
    {
        if (username == m_ClientList[i].username)
        {
            m_ClientList.erase(m_ClientList.begin() + i);   // remove client from vector
            std::cout << username << " exited\n";
            break;
        }
    }

    for (size_t i = 0; i < m_ClientList.size(); ++i)
    {
        std::string message = "[" + username + " exited]";
        SendMsg(m_ClientList[i], message);
    }
}

void ServerManager::DisplayAllUsers(CLIENT_INFO const& ReqClient)
{
    std::string users = "[Connected users: ";
    for (size_t i = 0; i < m_ClientList.size(); ++i)
    {
        users += m_ClientList[i].username;

        if (i != m_ClientList.size() - 1)
            users += ", ";
        else
            users += "]";
    }

    SendMsg(ReqClient, users);
}

bool ServerManager::InitWinSock2_0()
{
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 0);

    if (!WSAStartup(wVersion, &wsaData))
        return true;

    return false;
}

BOOL WINAPI ClientServerThread(LPVOID lpData)
{

    CLIENT_INFO* pClientInfo = (CLIENT_INFO*)lpData;
    CLIENT_INFO clientInfo = *pClientInfo;
    char szBuffer[1024];
    int nLength{ 0 };

    clientInfo.semaphore = CreateSemaphore(NULL, 0, 1, NULL);
    if (clientInfo.semaphore == NULL)
    {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return 1;
    }

    while (true)
    {
        nLength = recv(clientInfo.hClientSocket, szBuffer, sizeof(szBuffer), 0);
        if (nLength > 0)
        {
            if (nLength < static_cast<int>(sizeof(szBuffer)))
                szBuffer[nLength] = '\0';

            strcpy(clientInfo.username, szBuffer);

            //check for existing users
            if (ServerManager::GetInstance()->IsUsernameTaken(clientInfo.username))
            {
                std::string sameUsername = "[Username has already been used. Please enter another name.]";
                ServerManager::GetInstance()->SendMsg(clientInfo, sameUsername);
                continue;
            }

            //display welcome message to the user
            std::string tempUsername = clientInfo.username;
            for (CLIENT_INFO const& c : ServerManager::GetInstance()->m_ClientList)
            {
                // Broadcast welcome message to all existing users
                std::string joinMsg = "[" + tempUsername + " joined]";
                ServerManager::GetInstance()->SendMsg(c, joinMsg);                // Greet new client
            }
            std::string welcomeMsg = "[Welcome " + tempUsername + "!]";
            ServerManager::GetInstance()->SendMsg(clientInfo, welcomeMsg);
            ServerManager::GetInstance()->m_ClientList.push_back(clientInfo);     // add new client

            std::cout << tempUsername << " connected from IP: " << inet_ntoa(clientInfo.clientAddr.sin_addr) << std::endl;

            HANDLE clientThread;
            DWORD threadId;

            // Create an extra thread to settle one client instance
            clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)&clientInfo, 0, &threadId);

            if (clientThread == NULL)
            {
                std::cout << "Unable to create client thread" << std::endl;
                return FALSE;
            }
            else
                CloseHandle(clientThread);

            while (WaitForSingleObject(clientInfo.semaphore, 0) != WAIT_OBJECT_0)
            {
                //std::cout << "WAITING\n";
            }
            return TRUE;
        }
    }
}

BOOL WINAPI ClientThread(LPVOID lpData)
{
    CLIENT_INFO* pClientInfo = (CLIENT_INFO*)lpData;
    CLIENT_INFO clientInfo = *pClientInfo;
    char szBuffer[1024];
    int nLength;

    if (clientInfo.semaphore)
    {
        ReleaseSemaphore(clientInfo.semaphore, 1, NULL);
    }

    while (1)
    {
        nLength = recv(clientInfo.hClientSocket, szBuffer, sizeof(szBuffer), 0);
        if (nLength > 0)
        {
            if (nLength < static_cast<int>(sizeof(szBuffer)))
                szBuffer[nLength] = '\0';

            // Convert the string to upper case and send it back, if its not QUIT
            //_strupr( szBuffer ) ;
            if (strcmp(szBuffer, "@quit") == 0)
            {
                ServerManager::GetInstance()->QuitMessage(clientInfo.username);
                closesocket(clientInfo.hClientSocket);
                return TRUE;
            }

            if (strcmp(szBuffer, "@names") == 0)
            {
                ServerManager::GetInstance()->DisplayAllUsers(clientInfo);
                continue;
            }

            // broadcast message from this user to all other users
            ServerManager::GetInstance()->BroadcastMessage(clientInfo, szBuffer);
        }
        else
        {
            std::cout << "Error reading the data from " << inet_ntoa(clientInfo.clientAddr.sin_addr) << std::endl;
            ServerManager::GetInstance()->QuitMessage(clientInfo.username);
            closesocket(clientInfo.hClientSocket);
            break;
        }
    }

    return TRUE;
}








