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
		C2SData,

		// Server to Client 
		ServerDataTypes, // This mark the beginning of server data types
		S2CNumOfClientConnected,
		S2CGamePlayData,
		S2CEntityDetail, // For player and jam
		S2CGameStats, // TODO What game stats are there
		S2CPlayAgainCount,
		S2CData
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
	
	int SerialiseData(); // Serialise a string and int to send over

	void DeserialisePlayerControls();													// App::FirstUpdate(), since it's before logic system
	void DeserialisePlayAgain();															// App::FirstUpdate(), since it's before logic system
	void DeserialiseNumberOfClientConnected();								// App::FirstUpdate(), since it's before logic system
	void DeserialiseGamePlayData();														// App::FirstUpdate(), since it's before logic system
	void DeserialiseGameStats(char*&);												// App::FirstUpdate(), since it's before logic system
	void DeserialiseMultipleEntities(char*&);									// App::FirstUpdate(), since it's before logic system
	void DeserialiseEntityDetail(char* currBuff);							// App::FirstUpdate(), since it's before logic system
	void DeserialisePlayAgainCount();													// App::FirstUpdate(), since it's before logic system

	// To use the data bank, firstly input the data using PrepareData(). Secondly, send the data using SerialiseAndSend(C2SData or S2CData)
	void PrepareData(std::string, int);
	bool GetFromBank(std::string, int*);
	void DeserialiseData();

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








	std::map<std::string, int> dataBank;

private:

	std::string dataBankBuffStr;
	int dataBankBuffInt;
	size_t mSize;

};



