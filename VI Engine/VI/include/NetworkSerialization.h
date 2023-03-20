#pragma once
#include <ECS_systems.h>
#include <ECS_tools.h>
#include <vec2.h>

#define MAX_UDP_PACKET_SIZE 65507

/*!*****************************************************************************
\brief
	This class encapsulates the functions for Serialization manager
*******************************************************************************/
class NetworkSerializationManager : public Singleton<NetworkSerializationManager> {
public:
	enum class NETWORKDATATYPE : char {
		// Client to Server
		C2SPlayerControls = 1,
		C2SPlayAgain,

		// Server to Client 
		ServerDataTypes, // This mark the beginning of server data types
		S2CNumOfClientConnected,
		S2CGamePlayData,
		S2CEntityDetail, // For player and jam
		S2CGameStats, // TODO What game stats are there
		S2CNumOfPlayerReplay
	};

	NetworkSerializationManager();
	~NetworkSerializationManager();

	void SerialiseAndSend(NETWORKDATATYPE);
	void DeserialiseAndLoad();

	int SerialisePlayerControls();
	int SerialisePlayAgain();
	int SerialiseNumberOfClientConnected();
	int SerialiseGamePlayData();
	int SerialiseGameStats(char*&);
	int SerialiseMultipleEntities(char*&, std::set<Entity>);
	int SerialiseEntityDetail(char*&, Entity);

	int SerialiseNumberOfPlayerReplay();

	void DeserialisePlayerControls();
	void DeserialisePlayAgain();
	void DeserialiseNumberOfClientConnected();
	void DeserialiseMultipleEntities();
	void DeserialiseEntityDetail(char* currBuff);

	void PrintSendBuff();
	void TestTransferData();

	char* FindSubStringEnd(char*& str, const char* findee, int strLen = 30000);

	char* mSendBuff;
	char* mRecvBuff;
private:
	size_t mSize;

};



