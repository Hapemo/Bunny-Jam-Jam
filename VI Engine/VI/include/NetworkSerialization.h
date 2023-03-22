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
		S2CPlayAgainCount
	};

	NetworkSerializationManager();
	~NetworkSerializationManager();

	void SerialiseAndSend(NETWORKDATATYPE);
	void DeserialiseAndLoad();

	int SerialisePlayerControls();														// App::FirstUpdate(), after glfwinputpoll
	int SerialisePlayAgain();																	// App::SecondUpdate(), since it'll be after the button update
	int SerialiseNumberOfClientConnected();										// ServerManager when a client joined and initialised in the server
	int SerialiseGamePlayData();															// App::SecondUpdate(), since it's after all the logic and collision systems
	int SerialiseGameStats(char*&);														// same
	int SerialiseMultipleEntities(char*&, std::set<Entity>);	// same
	int SerialiseEntityDetail(char*&, Entity);								// same
	int SerialisePlayAgainCount();														// After play again number changes. If gamestate is asking for play again and compare mPlayAgainCount with mPrevPlayAgainCount

	void DeserialisePlayerControls();													// App::FirstUpdate(), since it's before logic system
	void DeserialisePlayAgain();															// App::FirstUpdate(), since it's before logic system
	void DeserialiseNumberOfClientConnected();								// App::FirstUpdate(), since it's before logic system
	void DeserialiseGamePlayData();														// App::FirstUpdate(), since it's before logic system
	void DeserialiseGameStats(char*&);												// App::FirstUpdate(), since it's before logic system
	void DeserialiseMultipleEntities(char*&);									// App::FirstUpdate(), since it's before logic system
	void DeserialiseEntityDetail(char* currBuff);							// App::FirstUpdate(), since it's before logic system
	void DeserialisePlayAgainCount();													// App::FirstUpdate(), since it's before logic system

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

	bool mPlayAgain;
	int mPrevPlayAgainCount;
	int mPlayAgainCount; // if -1, quit. if 0, no data yet. if 1, 1 person play again. if 2, all player want to play again

	int mJam;
	float mTimeRemaining;
	int mRound;

#ifndef _SERVER //If client
	char prevInput;
#endif

#ifndef _CLIENT //If server
	bool mP1InputW;
	bool mP1InputA;
	bool mP1InputS;
	bool mP1InputD;
	bool mP2InputW;
	bool mP2InputA;
	bool mP2InputS;
	bool mP2InputD;
#endif




private:
	size_t mSize;

};



