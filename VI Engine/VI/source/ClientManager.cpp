#include "NetworkSerialization.h";
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include "ClientManager.h"
//== Defualt ctor
ClientManager::ClientManager() {}

//== Default dtor
ClientManager::~ClientManager() {
    m_ClientRecvThread.join();
    clientClose();
}

//== init winsock2
//== set up client UDP socket
bool ClientManager::clientInit(std::string serverIPAddress, u_short serverPortNumber, std::string name)
{
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 0);
    std::cout << "Enter client init\n";

    if (WSAStartup(wVersion, &wsaData)) {
        std::cout << "Unable to Initialize Windows Socket environment" << WSAGetLastError() << std::endl;
        return false;
    }
    
    // Create the Client UDP Socket 
    m_ClientInstance.clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    m_ClientInstance.username = name;
	  if (m_ClientInstance.clientSocket == INVALID_SOCKET) {
		  std::cout << "Unable to create Client UDP Socket. Error Code: " << WSAGetLastError() << std::endl;
      WSACleanup();
		  return false;
	  } else {
        std::cout << "Client Socket created successfully\n";
    }

	  //!< Everything past here means that it is a valid socket
    m_ServerInstance.m_ServerInfo.sin_family         = AF_INET;
    m_ServerInstance.m_ServerInfo.sin_addr.s_addr    = inet_addr(serverIPAddress.c_str());
    m_ServerInstance.m_ServerInfo.sin_port           = htons(serverPortNumber);

    //!! == EXPERIMENT SENDING CLIENT STRUCT TO SERVER HERE AS A CONNECTION REQUEST
    std::cout << "Preparing to fo first data sending\n";

    std::string sConnectionSetup = "Client Connected";
    int nLength = strlen(sConnectionSetup.c_str());
    if (sendto(m_ClientInstance.clientSocket, sConnectionSetup.c_str(), nLength, 0, (sockaddr*)&m_ServerInstance.m_ServerInfo, sizeof(m_ServerInstance.m_ServerInfo)) == SOCKET_ERROR) {
        std::cout << "ERROR: Unable to send initial connection request message\n";
        return false;
    }
    std::cout << "Successfully sent first data, going to create thread now\n";

    
    // ======= THREADS =======
    m_ClientRecvThread = std::thread(clientRecvData);
    std::cout << "Successfully created client recv thread\n";

    return true;
}


//== Receive message from server. Might need to change the recv to recvfrom
void clientRecvData()
{
    while (1)
    {
        //char buffer[1024];
        //int fromlength = sizeof(buffer);
        //if (ClientManager::GetInstance()->m_StopReceive)
        //    return;

        char localBuff[MAX_UDP_PACKET_SIZE];
        int size = sizeof(localBuff);

        int nLength = recvfrom(ClientManager::GetInstance()->m_ClientInstance.clientSocket, 
                               localBuff,
                               MAX_UDP_PACKET_SIZE, 0, 
                               (sockaddr*)&ClientManager::GetInstance()->m_ServerInstance.m_ServerInfo, 
                               &size);

        //if (nLength > 0) std::cout << "\n\n>> [CLIENT] Received data!\n" << "nLength: " << nLength << '\n';

        if (nLength == SOCKET_ERROR) {
          std::cout << "RECV SOCKET ERROR\n";
          std::cout << "WSAGetLastError: " << WSAGetLastError() <<'\n';
          return;
        }

        memset(NetworkSerializationManager::GetInstance()->mRecvBuff, 0, MAX_UDP_PACKET_SIZE);
        memcpy(NetworkSerializationManager::GetInstance()->mRecvBuff, localBuff, nLength);

        if (nLength == size)
            std::cout << "the sizes are the same!\n";
        else
            std::cout << "nlength: " << nLength << ", size: " << size << "\n";
        
        //NetworkSerializationManager::GetInstance()->PrintRecvBuff();
        unsigned long newPacketNum = *reinterpret_cast<unsigned long*>(NetworkSerializationManager::GetInstance()->mRecvBuff + (nLength - sizeof(unsigned long) - sizeof(char)));


        //std::cout << ">> [CLIENT] oldpacketnum: " << ClientManager::GetInstance()->serverPacketNum << '\n';
        //std::cout << ">> [CLIENT] newPacketNum: " << newPacketNum << '\n';
        if (ClientManager::GetInstance()->serverPacketNum < newPacketNum) 
        {
          //std::cout << "Successful, going to deserialise\n";
          ClientManager::GetInstance()->serverPacketNum = newPacketNum;
          // tell jazz to deserialize
          NetworkSerializationManager::GetInstance()->DeserialiseAndLoad();
          NetworkSerializationManager::GetInstance()->mPlayerID = static_cast<int>(*(NetworkSerializationManager::GetInstance()->mRecvBuff + nLength - sizeof(char)));
          //std::cout << ">> [CLIENT] :: PlayerID in clientRecvData is " << NetworkSerializationManager::GetInstance()->mPlayerID << "\n\ncvcv";
          NetworkSerializationManager::GetInstance()->UpdatePrevTime(NetworkSerializationManager::GetInstance()->GetTime());
        }
    }

    return;
}


//== Send message to the server.
bool ClientManager::clientSendData(const char* Data, int size)
{
    int nSendCnt = 0;
	//int nLength = strlen(Data.c_str());
	int nLength = size;
	//char* pBuffer = (char*)Data.c_str();
    
    while ((nSendCnt = sendto(m_ClientInstance.clientSocket, Data, nLength, 0, (sockaddr*)&m_ServerInstance.m_ServerInfo, sizeof(m_ServerInstance.m_ServerInfo))) != nLength)
    {
        if (nSendCnt == -1)
        {
            std::cout << "Error sending the data to server" << std::endl;
            return false;
        }

        if (nSendCnt == nLength)
            break;

        Data += nSendCnt;
        nLength -= nSendCnt;
    }

    return true;
}

//== Closes the client socket
void ClientManager::clientClose()
{
	closesocket(m_ClientInstance.clientSocket);
	WSACleanup();
}



//// private function to initialize winsock2
//bool ClientManager::InitWinSock2_0()
//{
//    WSADATA wsaData;
//    WORD wVersion = MAKEWORD(2, 0);
//
//    if (!WSAStartup(wVersion, &wsaData))
//        return true;
//
//    return false;
//}















