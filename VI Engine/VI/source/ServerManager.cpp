#include "NetworkSerialization.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ServerManager.h"
#include <WS2tcpip.h>

//== Default ctor
ServerManager::ServerManager() {}

//== Default dtor
ServerManager::~ServerManager() {
    std::cout << "SERVER ---------- DESTRUCTOR\n";
    m_ServerRecvThread.join();
    serverClose();
}

int stupidserver = 0;
//== Create and bind Server Socket
bool ServerManager::serverInit(u_short serverPortNumber)
{
    std::cout << "Enter server Init\n";
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 0);
    if (WSAStartup(wVersion, &wsaData))
    {
        std::cout << "Unable to Initialize Windows Socket environment" << WSAGetLastError() << std::endl;
        return false;
    }

    // Creating a Socket
    m_ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_ServerSocket == INVALID_SOCKET)
    {
        std::cout << "Unable to initialize Server Socket\n";
        WSACleanup();
        return false;
    }
    else {
		    std::cout << "Chat Server Socket Initialized\n";
    }
    std::cout << "Preparing host name\n";

    //!<== Obtaining hostname
    char hostname[256];
    gethostname(hostname, sizeof(hostname));
    m_HostName = std::string(hostname);
    std::cout << ">> [SERVER]:: Hostname: " << m_HostName << "\n";

    //!<== Obtaining my own IP Address, as a server
    struct addrinfo hints, *info;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = SOCK_DGRAM;
    hints.ai_protocol   = IPPROTO_UDP;

    std::cout << "Preparing to getaddrinfo\n";
    if (getaddrinfo(hostname, NULL, &hints, &info) != 0) {
        std::cout << "SERVER:: getaddrinfo failed with an error.\n";
        WSACleanup();
        return false;
    }

    // parsing the IP address to a string
    char ipstr[INET_ADDRSTRLEN];
    struct sockaddr_in* addr = (struct sockaddr_in*)info->ai_addr;
    inet_ntop(AF_INET, &addr->sin_addr, ipstr, sizeof(ipstr));      // convert the address to a string
    
    struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPortNumber);
	serverAddr.sin_addr.s_addr = inet_addr(ipstr);
    
  std::cout << "Preparing to bind\n";
  if (bind(m_ServerSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Error Code: " << WSAGetLastError() << " - ";
        std::cout << "Unable to connect to " << inet_ntoa(serverAddr.sin_addr) << " port " << ntohs(serverAddr.sin_port) << std::endl;
        closesocket(m_ServerSocket);
        WSACleanup();
        return false;
    }
  std::cout << "Successfully binded\n";

    std::cout << ">> [SERVER]:: Server Connected. Information: " << inet_ntoa(serverAddr.sin_addr) << " port " << htons(serverAddr.sin_port) << std::endl;

    // start the constant running thread
    std::cout << "Preparing server recv thread\n";
    m_ServerRecvThread = std::thread(serverRecvData);
    std::cout << "Successfully created server recv thread\n";

    return true;
}

//== Batch send to all connected clients
bool ServerManager::serverSendData(char* data, int size)
{
    // Broadcast data to all clients
    for (auto const& c : m_ClientList) 
    {
        data[size] = static_cast<char>(c.second.sPlayerNum);
        
        if (SendMsg(c.second, data, size + sizeof(char)) == false)
            return false;
    }

    return true;
}

//== Multithreaded function -- Constantly listening to client input
//== Updates the connected client list here
void serverRecvData()
{
    while (1)
    {
        int nLength = 0;

        CLIENT_INFO localClient{};
        struct sockaddr_in clientAddr;

        if (ServerManager::GetInstance()->m_ServerSocket == INVALID_SOCKET || ServerManager::GetInstance()->m_ServerSocket == SOCKET_ERROR)
        {
            std::cout << "Unable to initialize Server Socket\n";
            WSACleanup();
            break;
        }
        int temp{ MAX_UDP_PACKET_SIZE };
        char localBuff[MAX_UDP_PACKET_SIZE];

        nLength = recvfrom(ServerManager::GetInstance()->m_ServerSocket,
                           localBuff, 
                           MAX_UDP_PACKET_SIZE, 0, (sockaddr*)&clientAddr, &temp);
        
        memset(NetworkSerializationManager::GetInstance()->mRecvBuff, 0, MAX_UDP_PACKET_SIZE);
        memcpy(NetworkSerializationManager::GetInstance()->mRecvBuff, localBuff, nLength);

        if (nLength == SOCKET_ERROR) {
            std::cout << "RECV SOCKET ERROR\n";
            return;
        }

        unsigned long newPacketNum = *reinterpret_cast<unsigned long*>(NetworkSerializationManager::GetInstance()->mRecvBuff + (nLength - sizeof(unsigned long)));

        // get the client's IP address
        std::string clientdata = inet_ntoa(clientAddr.sin_addr);
        
        // check clientlist
        auto clientinstance = ServerManager::GetInstance()->m_ClientList.find(clientdata);

        //!< update clientlist if it encounters a new instance
        if (clientinstance == ServerManager::GetInstance()->m_ClientList.end()) 
        {
            CLIENT_INFO ci_instance;
            ci_instance.clientAddr = clientAddr;
            ci_instance.sPlayerNum = ++NetworkSerializationManager::GetInstance()->mNumberOfClientConnected;
            ServerManager::GetInstance()->m_ClientList[clientdata] = ci_instance;

            NetworkSerializationManager::GetInstance()->SerialiseAndSend(NetworkSerializationManager::NETWORKDATATYPE::S2CNumOfClientConnected);
            
            if (ServerManager::GetInstance()->m_ClientList.size() > 1) {
              std::cout << "More than 1 player, starting game\n";
              NetworkSerializationManager::GetInstance()->mGameStarted = true;
              NetworkSerializationManager::GetInstance()->mGameCountDown = 5;
            }
		}
        
        else 
        {
          if (clientinstance->second.clientPacketNum < newPacketNum) 
          {
            clientinstance->second.clientPacketNum = newPacketNum;
            NetworkSerializationManager::GetInstance()->mPlayerID = std::distance(ServerManager::GetInstance()->m_ClientList.begin(), clientinstance) + 1;
            
            NetworkSerializationManager::GetInstance()->DeserialiseAndLoad();
          }
        }
    }
}

//== Closes the server sockets. Cleanup
void ServerManager::serverClose()
{
    closesocket(m_ServerSocket);
    WSACleanup();
}

//== Sends the message to a particular client
bool ServerManager::SendMsg(CLIENT_INFO const& receiver, const char* Data, int size)
{
    int nSendCnt = 0;
    //int nLength = strlen(Data.c_str());
    int nLength = size;
    //char* pBuffer = (char*)Data.c_str();
    
    while ((nSendCnt = sendto(m_ServerSocket, Data, nLength, 0, (sockaddr*)&receiver.clientAddr, sizeof(receiver.clientAddr))) != nLength)
    {
        if (nSendCnt == nLength)
            break;
        
        // error sending data
        if (nSendCnt == SOCKET_ERROR) {
            std::cout << "Error sending the data to " << inet_ntoa(receiver.clientAddr.sin_addr) << std::endl;
            closesocket(receiver.hClientSocket);
            WSACleanup();
            return false;
        }

        Data += nSendCnt;
        nLength -= nSendCnt;
    }

    return true;
}

//== Broadcast messages to all connected clients
void ServerManager::BroadcastMessage(CLIENT_INFO const& sender, std::string const& msg)
{
    std::string message = "[" + std::string(sender.username) + ":] " + msg;
    int msgLen = strlen(message.c_str());
    for (auto const& c : m_ClientList) {
        SendMsg(c.second, message.c_str(), msgLen);
    }
}
