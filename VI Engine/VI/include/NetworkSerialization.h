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


	//---------------
	// All Game Data
	//---------------
	int mNumberOfClientConnected;
	int mPlayerID;

	int mPlayAgainCount; // if -1, quit. if 0, no data yet. if 1, 1 person play again. if 2, all player want to play again

	int mJam;
	float mTimeRemaining;
	int mRound;

#ifndef _SERVER //If client
	char prevInput;
#endif

	bool mP1InputW;
	bool mP1InputA;
	bool mP1InputS;
	bool mP1InputD;
	bool mP2InputW;
	bool mP2InputA;
	bool mP2InputS;
	bool mP2InputD;




private:
	size_t mSize;

};



