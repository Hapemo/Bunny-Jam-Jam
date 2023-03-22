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
	//serverAddr.sin_addr.s_addr = inet_addr(std::string("172.28.139.212").c_str());
    
    //m_ServerInstance.m_ServerInfo = *(struct sockaddr_in*)info->ai_addr;
    //m_ServerInstance.m_ServerInfo.sin_port = htons(serverPortNumber);
    //m_ServerInstance.m_ServerAddress = std::string(ipstr);
    //m_ServerInstance.m_ServerPort = htons(serverPortNumber);
    
   // //!< Binding the Server socket to the address & port. We need this because port numbers for UDP are more sensitive
   // int reiterations = 0;
   // do
   // {
   //     // skip the first iteration
   //     if (++reiterations != 1) {
   //         std::cout << "Error Code: " << WSAGetLastError() << " - ";
   //         std::cout << "Unable to connect to " << inet_ntoa(m_ServerInstance.m_ServerInfo.sin_addr) << " port " << ntohs(m_ServerInstance.m_ServerInfo.sin_port) << std::endl;
   //         std::cout << "Retrying with port number: " << ++serverPortNumber << "\n";

			//m_ServerInstance.m_ServerPort = htons(serverPortNumber);
   //         //closesocket(m_ServerSocket);
   //         //WSACleanup();
   //         //return false;
   //     }

   //     //m_ServerInstance.m_ServerInfo.sin_family = AF_INET;                                                     // The address family. MUST be AF_INET
   //     //m_ServerInstance.m_ServerInfo.sin_addr.s_addr = inet_addr(m_ServerInstance.m_ServerAddress.c_str());    // converts a string containing ipv4 address into a proper address for the IN_ADDR struct  
   //     //m_ServerInstance.m_ServerInfo.sin_port = htons(serverPortNumber);                                       // converts a u_short from host to TCP/IP network byte order (which is big-endian)
   // } while (bind(m_ServerSocket, (struct sockaddr*)&m_ServerInstance.m_ServerInfo, sizeof(m_ServerInstance.m_ServerInfo)) == SOCKET_ERROR);
    
    
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
bool ServerManager::serverSendData(const char* data, int size)
{
    //std::cout << "ClientList Size: " << m_ClientList.size() << "\n";
    
    // Broadcast data to all clients
    for (auto const& c : m_ClientList) {
        if (SendMsg(c.second, data, size) == false)
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
        //char cBuffer[1024];
        //int fromlength = sizeof(cBuffer);

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
        
        if (nLength >= 0) {
            //cBuffer[nLength] = '\0';
            std::cout << ">> [S] Data received!\n";
        }

        if (nLength == SOCKET_ERROR) {
            std::cout << "RECV SOCKET ERROR\n";
            return;
        }

        unsigned long newPacketNum = *reinterpret_cast<unsigned long*>(NetworkSerializationManager::GetInstance()->mRecvBuff + (nLength - sizeof(unsigned long)));

        // get the client's IP address
        std::string clientdata = inet_ntoa(clientAddr.sin_addr);
        
        // check and update clientlist
        auto clientinstance = ServerManager::GetInstance()->m_ClientList.find(clientdata);
        if(clientinstance == ServerManager::GetInstance()->m_ClientList.end()) {
            CLIENT_INFO ci_instance;
            ci_instance.clientAddr = clientAddr;
            ServerManager::GetInstance()->m_ClientList[clientdata] = ci_instance;
            std::cout << ">> [SERVER] :: Received a new client connection: " << clientdata << "\n";
		    }
        else {
          // wrong instnace
          if (clientinstance->second.clientPacketNum < newPacketNum) {
            clientinstance->second.clientPacketNum = newPacketNum;
            // tell jazz to deserialize
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

//bool ServerManager::InitWinSock2_0()
//{
//    WSADATA wsaData;
//    WORD wVersion = MAKEWORD(2, 0);
//
//    if (WSAStartup(wVersion, &wsaData))
//    {
//        std::cout << "Unable to Initialize Windows Socket environment" << WSAGetLastError() << std::endl;
//        return false;
//    }
//
//    return true;
//}

//BOOL WINAPI ClientServerThread(LPVOID lpData)
//{
//
//    CLIENT_INFO* pClientInfo = (CLIENT_INFO*)lpData;
//    CLIENT_INFO clientInfo = *pClientInfo;
//    char szBuffer[1024];
//    int nLength{ 0 };
//
//    clientInfo.semaphore = CreateSemaphore(NULL, 0, 1, NULL);
//    if (clientInfo.semaphore == NULL)
//    {
//        printf("CreateSemaphore error: %d\n", GetLastError());
//        return 1;
//    }
//
//    while (true)
//    {
//        nLength = recv(clientInfo.hClientSocket, szBuffer, sizeof(szBuffer), 0);
//        if (nLength > 0)
//        {
//            if (nLength < static_cast<int>(sizeof(szBuffer)))
//                szBuffer[nLength] = '\0';
//
//            strcpy(clientInfo.username, szBuffer);
//
//            //check for existing users
//            if (ServerManager::GetInstance()->IsUsernameTaken(clientInfo.username))
//            {
//                std::string sameUsername = "[Username has already been used. Please enter another name.]";
//                ServerManager::GetInstance()->SendMsg(clientInfo, sameUsername);
//                continue;
//            }
//
//            //display welcome message to the user
//            std::string tempUsername = clientInfo.username;
//            //for (CLIENT_INFO const& c : ServerManager::GetInstance()->m_ClientList)
//            {
//                // Broadcast welcome message to all existing users
//                std::string joinMsg = "[" + tempUsername + " joined]";
//                //ServerManager::GetInstance()->SendMsg(c, joinMsg);                // Greet new client
//            }
//            std::string welcomeMsg = "[Welcome " + tempUsername + "!]";
//            ServerManager::GetInstance()->SendMsg(clientInfo, welcomeMsg);
//            //ServerManager::GetInstance()->m_ClientList.push_back(clientInfo);     // add new client
//
//            std::cout << tempUsername << " connected from IP: " << inet_ntoa(clientInfo.clientAddr.sin_addr) << std::endl;
//
//            HANDLE clientThread;
//            DWORD threadId;
//
//            // Create an extra thread to settle one client instance
//            clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, (LPVOID)&clientInfo, 0, &threadId);
//
//            if (clientThread == NULL)
//            {
//                std::cout << "Unable to create client thread" << std::endl;
//                return FALSE;
//            }
//            else
//                CloseHandle(clientThread);
//
//            while (WaitForSingleObject(clientInfo.semaphore, 0) != WAIT_OBJECT_0)
//            {
//                //std::cout << "WAITING\n";
//            }
//            return TRUE;
//        }
//    }
//}

//BOOL WINAPI ClientThread(LPVOID lpData)
//{
//    CLIENT_INFO* pClientInfo = (CLIENT_INFO*)lpData;
//    CLIENT_INFO clientInfo = *pClientInfo;
//    char szBuffer[1024];
//    int nLength;
//
//    if (clientInfo.semaphore)
//    {
//        ReleaseSemaphore(clientInfo.semaphore, 1, NULL);
//    }
//
//    while (1)
//    {
//        nLength = recv(clientInfo.hClientSocket, szBuffer, sizeof(szBuffer), 0);
//        if (nLength > 0)
//        {
//            if (nLength < static_cast<int>(sizeof(szBuffer)))
//                szBuffer[nLength] = '\0';
//
//            // Convert the string to upper case and send it back, if its not QUIT
//            //_strupr( szBuffer ) ;
//            if (strcmp(szBuffer, "@quit") == 0)
//            {
//                ServerManager::GetInstance()->QuitMessage(clientInfo.username);
//                closesocket(clientInfo.hClientSocket);
//                return TRUE;
//            }
//
//            if (strcmp(szBuffer, "@names") == 0)
//            {
//                ServerManager::GetInstance()->DisplayAllUsers(clientInfo);
//                continue;
//            }
//
//            // broadcast message from this user to all other users
//            ServerManager::GetInstance()->BroadcastMessage(clientInfo, szBuffer);
//        }
//        else
//        {
//            std::cout << "Error reading the data from " << inet_ntoa(clientInfo.clientAddr.sin_addr) << std::endl;
//            ServerManager::GetInstance()->QuitMessage(clientInfo.username);
//            closesocket(clientInfo.hClientSocket);
//            break;
//        }
//    }
//
//    return TRUE;
//}



//void ServerManager::QuitMessage(std::string const& username)
//{
//    //for (size_t i = 0; i < m_ClientList.size(); ++i)
//    for(auto& x : m_ClientList)
//    {
//        if (username == x.second.username)
//        {
//            m_ClientList.erase(x);   // remove client from vector
//            std::cout << username << " exited\n";
//            break;
//        }
//    }
//
//    //for (size_t i = 0; i < m_ClientList.size(); ++i)
//    for(const auto& x : m_ClientList)
//    {
//        std::string message = "[" + username + " exited]";
//        SendMsg(x.second, message);
//    }
//}


//void ServerManager::DisplayAllUsers(CLIENT_INFO const& ReqClient)
//{
//    std::string users = "[Connected users: ";
//    for (size_t i = 0; i < m_ClientList.size(); ++i)
//    {
//        users += m_ClientList[i].username;
//
//        if (i != m_ClientList.size() - 1)
//            users += ", ";
//        else
//            users += "]";
//    }
//
//    SendMsg(ReqClient, users);
//}

