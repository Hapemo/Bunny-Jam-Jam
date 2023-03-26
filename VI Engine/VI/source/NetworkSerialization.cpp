#include <NetworkSerialization.h>
#include <ECSManager.h>
#include "Input.h"
#include "GameStateManager.h"
#include "ServerManager.h"
#include "ClientManager.h"

#define DEBUGPRINT 1


namespace {
	const char generalName[] = "GeneralComponent";
	const char transformName[] = "TransformComponent";
	const char physics2dName[] = "Physics2DComponent";
	const char rectColliderName[] = "RectColliderComponent";
	const char circleColliderName[] = "CircleColliderComponent";
}


NetworkSerializationManager::NetworkSerializationManager() : 
	mSendBuff(new char[MAX_UDP_PACKET_SIZE]), mRecvBuff(new char[MAX_UDP_PACKET_SIZE]), mSize(0),
	mNumberOfClientConnected(), mPlayerID()
{
	std::cout << "mRecvBuff: " << mRecvBuff << '\n';
}

NetworkSerializationManager::~NetworkSerializationManager() {
	delete[] mSendBuff;
}

void NetworkSerializationManager::SerialiseAndSend(NETWORKDATATYPE dataType) {
	// Serialise into mSendBuff
	int dataSize{};
	switch (dataType) {
	case NETWORKDATATYPE::C2SPlayerControls:
		dataSize = SerialisePlayerControls();
		break;
		
	case NETWORKDATATYPE::C2SPlayAgain:
		dataSize = SerialisePlayAgain();
		break;
		
	case NETWORKDATATYPE::ServerDataTypes:
		break;// DO NOTHING HERE PLEASE

	case NETWORKDATATYPE::S2CGameStarted:
		dataSize = SerialiseGameStarted();
		break;

	case NETWORKDATATYPE::S2CNumOfClientConnected:
		dataSize = SerialiseNumberOfClientConnected();
		break;
		
	case NETWORKDATATYPE::S2CGamePlayData:
		dataSize = SerialiseGamePlayData();
		break;
		
	case NETWORKDATATYPE::S2CEntityDetail:
		//dataSize = SerialiseEntityDetail();
		break;
		
	case NETWORKDATATYPE::S2CGameStats:
		//dataSize = SerialiseGameStats();
		break;
		
	case NETWORKDATATYPE::S2CPlayAgainCount:
		//dataSize = SerialiseNumOfPlayerReplay();
		break;

	case NETWORKDATATYPE::C2SData:
	case NETWORKDATATYPE::S2CData:

		break;
	}

	if (dataSize == 0) return;

	bool isServer = static_cast<char>(dataType) > static_cast<char>(NETWORKDATATYPE::ServerDataTypes);

	if (isServer) {
		*reinterpret_cast<unsigned long*>(mSendBuff + dataSize) = ++ServerManager::GetInstance()->serverPacketNum;
#if DEBUGPRINT
		std::cout << "ServerManager::GetInstance()->serverPacketNum: " << ServerManager::GetInstance()->serverPacketNum << '\n';
#endif
		ServerManager::GetInstance()->serverSendData(mSendBuff, dataSize + sizeof(unsigned long));
	} else {
		*reinterpret_cast<unsigned long*>(mSendBuff + dataSize) = ++ClientManager::GetInstance()->clientPacketNum;
#if DEBUGPRINT
		std::cout << "ClientManager::GetInstance()->clientPacketNum: " << ClientManager::GetInstance()->clientPacketNum << '\n';
#endif
		ClientManager::GetInstance()->clientSendData(mSendBuff, dataSize + sizeof(unsigned long));
	}

#if DEBUGPRINT
	std::cout << "*reinterpret_cast<unsigned long*>(mSendBuff + dataSize): " << *reinterpret_cast<unsigned long*>(mSendBuff + dataSize) << '\n';
#endif
	//ServerManager::GetInstance()->serverSendData(mSendBuff, dataSize);
}

void NetworkSerializationManager::DeserialiseAndLoad() {
	NETWORKDATATYPE dataType = static_cast<NETWORKDATATYPE>(mRecvBuff[0]);

	switch (dataType) {
	case NETWORKDATATYPE::C2SPlayerControls:
		DeserialisePlayerControls();
		break;

	case NETWORKDATATYPE::C2SPlayAgain:
		break;

	case NETWORKDATATYPE::S2CGameStarted:
		DeserialiseGameStarted();
		break;

	case NETWORKDATATYPE::S2CNumOfClientConnected:
		DeserialiseNumberOfClientConnected();
		break;

	case NETWORKDATATYPE::S2CGamePlayData:
		DeserialiseGamePlayData();
		break;

	case NETWORKDATATYPE::S2CEntityDetail:
		break;

	case NETWORKDATATYPE::S2CGameStats:
		break;

	case NETWORKDATATYPE::S2CPlayAgainCount:
		break;

	case NETWORKDATATYPE::C2SData:
	case NETWORKDATATYPE::S2CData:
		DeserialiseData();

		break;
	}
}

// TODO Do you want me to check if there is input update here before sending?
int NetworkSerializationManager::SerialisePlayerControls() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::C2SPlayerControls);
	++currBuff; // 1 space in

	// Data consists of 4 keys, each key has 1 state (1 bytes each).
	// So 4 bytes in total. plus another 4 bytes at the front for mPlayerID

	*reinterpret_cast<int*>(currBuff) = mPlayerID;
	currBuff += sizeof(int);

	E_KEY up = E_KEY::W;		// 0001
	E_KEY down = E_KEY::S;	// 0010
	E_KEY left = E_KEY::A;	// 0100
	E_KEY right = E_KEY::D;	// 1000

	char currInput{};

	if (Input::CheckKey(PRESS, up) || Input::CheckKey(HOLD, up)) {
		currInput |= (1L << 0);
	}
	if (Input::CheckKey(PRESS, down) || Input::CheckKey(HOLD, down)) {
		currInput |= (1L << 1);
	}
	if (Input::CheckKey(PRESS, left) || Input::CheckKey(HOLD, left)) {
		currInput |= (1L << 2);
	}
	if (Input::CheckKey(PRESS, right) || Input::CheckKey(HOLD, right)) {
		currInput |= (1L << 3);
	}

	if (currInput == prevInput) return 0;
	prevInput = currInput;

	currBuff[0] = currInput;
	++currBuff;

	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialisePlayerControls() {
	char input{ mRecvBuff[5] };
	//bool up{static_cast<bool>(mRecvBuff[5])};
	//bool down{static_cast<bool>(mRecvBuff[6])};
	//bool left{static_cast<bool>(mRecvBuff[7])};
	//bool right{static_cast<bool>(mRecvBuff[8])};

	if (NetworkSerializationManager::GetInstance()->mPlayerID == 1) {
		mP1InputW = static_cast<bool>((input & (1L << 0)));
		mP1InputA = static_cast<bool>((input & (1L << 1)));
		mP1InputS = static_cast<bool>((input & (1L << 2)));
		mP1InputD = static_cast<bool>((input & (1L << 3)));
	} else if (NetworkSerializationManager::GetInstance()->mPlayerID == 2) {
		mP2InputW = static_cast<bool>((input & (1L << 0)));
		mP2InputA = static_cast<bool>((input & (1L << 1)));
		mP2InputS = static_cast<bool>((input & (1L << 2)));
		mP2InputD = static_cast<bool>((input & (1L << 3)));
	}

#if DEBUGPRINT
	std::cout << "Player " << NetworkSerializationManager::GetInstance()->mPlayerID;
	std::cout << "up: " << static_cast<bool>(input & (1L << 0)) << '\n';
	std::cout << "down: " << static_cast<bool>(input & (1L << 1)) << '\n';
	std::cout << "left: " << static_cast<bool>(input & (1L << 2)) << '\n';
	std::cout << "right: " << static_cast<bool>(input & (1L << 3)) << '\n';
#endif
	//PrintSendBuff();
}


int NetworkSerializationManager::SerialisePlayAgain() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::C2SPlayAgain);
	
	// TODO In script, get the bool value of play again or not.
	currBuff[0] = static_cast<char>(mPlayAgain);

	return static_cast<int>(++currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialisePlayAgain() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::C2SPlayAgain)
		std::cout << "NETWORKDATATYPE::C2SPlayAgain\n";
	
	bool playAgain{ static_cast<bool>(mRecvBuff[1]) };
	//std::cout << "playAgain: " << playAgain << '\n';

	if (mPlayAgainCount != -1)
	if (playAgain) { ++mPlayAgainCount; } 
	else mPlayAgainCount = -1;

	mPrevPlayAgainCount = mPlayAgainCount;
}

//-------------------------------
// Server to client
//-------------------------------

int NetworkSerializationManager::SerialiseGameStarted() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CGameStarted);

	currBuff[0] = static_cast<char>(mGameStarted);

	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialiseGameStarted() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CGameStarted)
		std::cout << "NETWORKDATATYPE::S2CGameStarted\n";

	mGameStarted = static_cast<bool>(mRecvBuff[1]);
}
       
int NetworkSerializationManager::SerialiseNumberOfClientConnected() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CNumOfClientConnected);
	
	currBuff[0] = static_cast<char>(mNumberOfClientConnected);

	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialiseNumberOfClientConnected() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CNumOfClientConnected)
		std::cout << "NETWORKDATATYPE::S2CNumOfClientConnected\n";

	mNumberOfClientConnected = static_cast<int>(mRecvBuff[1]);
	if (!mNumberOfClientConnected) mPlayerID = mNumberOfClientConnected;

#if DEBUGPRINT
	std::cout << "mNumberOfClientConnected: " << mNumberOfClientConnected << '\n';
#endif
}

int NetworkSerializationManager::SerialiseGamePlayData() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CGamePlayData);

	std::set<Entity> entitiesToSerialise;

	SerialiseGameStats(currBuff);
	SerialiseMultipleEntities(currBuff, entitiesToSerialise);

	return currBuff - mSendBuff;
}

void NetworkSerializationManager::DeserialiseGamePlayData() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CGamePlayData)
		std::cout << "NETWORKDATATYPE::S2CGamePlayData\n";

	char* currBuff{ mRecvBuff + 1 };

	DeserialiseGameStats(currBuff);
	DeserialiseMultipleEntities(currBuff);
}

int NetworkSerializationManager::SerialiseGameStats(char*& currBuff) {
	// int jam collected, float time remaining, bool swap
	int jam{};
	float timeRemain{};

	*(reinterpret_cast<int*>(currBuff)) = jam;
	currBuff += sizeof(int);
	*(reinterpret_cast<int*>(currBuff)) = timeRemain;
	currBuff += sizeof(float);
	
	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialiseGameStats(char*& currBuff) {
	mJam = *(reinterpret_cast<int*>(currBuff));
	currBuff += sizeof(int);
	mTimeRemaining = *(reinterpret_cast<int*>(currBuff));
	currBuff += sizeof(float);
}

int NetworkSerializationManager::SerialiseMultipleEntities(char*& currBuff, std::set<Entity> entities) {
	// Data Structure
	// NETWORKDATATYPE::S2CEntityDetail<NumberOfEntities><ListOfMemoryPosition><EachEntityData>
	//memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	//char* currBuff{ mSendBuff + 1 };
	//mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CEntityDetail);
	
	*reinterpret_cast<int*>(currBuff) = static_cast<int>(entities.size());
	currBuff += sizeof(int);

	// Reserve space for later
	int* entityMemoryPositions = reinterpret_cast<int*>(currBuff);
	currBuff += sizeof(int) * entities.size();

	for (Entity e : entities) {
		*entityMemoryPositions = static_cast<int>(currBuff - mSendBuff);
		SerialiseEntityDetail(currBuff, e);
		++entityMemoryPositions;
	}
	return currBuff - mSendBuff;
}

void NetworkSerializationManager::DeserialiseMultipleEntities(char*& currBuff) {
	int* entityPosition{ reinterpret_cast<int*>(currBuff) };
	int entityCount{ *entityPosition };
	++entityPosition;
	currBuff += sizeof(int);

	for (int i{}; i < entityCount; ++i, ++entityPosition) {
		int position = *entityPosition;
		DeserialiseEntityDetail(mRecvBuff + position);
	}
}

int NetworkSerializationManager::SerialiseEntityDetail(char*& currBuff, Entity _e) {
	// General component
	// GeneralComponent<name\0><isActive><isPaused>
	General& generalComp = _e.GetComponent<General>();
	memcpy(currBuff, generalName, strlen(generalName));
	currBuff += strlen(generalName);
	memcpy(currBuff, generalComp.name.c_str(), generalComp.name.size() + 1);
	// Need error control if entity is not found TODO
	currBuff += generalComp.name.size() + 1;
	currBuff[0] = static_cast<char>(generalComp.isActive);
	currBuff[1] = static_cast<char>(generalComp.isPaused);
	currBuff += 2;

	// Transform component
	// TransformComponent<Whole transform component>
	if (_e.HasComponent<Transform>()) {
		Transform& transformComp = _e.GetComponent<Transform>();
		memcpy(currBuff, transformName, strlen(transformName));
		currBuff += strlen(transformName);
		memcpy(currBuff, &transformComp, sizeof(transformComp));
		currBuff += sizeof(transformComp);
	}

	// Physics2D component
	// Size until before std::vector<Force> is 56
	// Physics2DComponent<56 bytes of physics2dComp><numOfForces><ListOfForces>
	if (_e.HasComponent<Physics2D>()) {
		Physics2D& physics2dComp = _e.GetComponent<Physics2D>();
		memcpy(currBuff, physics2dName, strlen(physics2dName));
		currBuff += strlen(physics2dName);
		memcpy(currBuff, &physics2dComp, 56);
		currBuff += 56;
		currBuff[0] = static_cast<char>(physics2dComp.forceList.size());
		++currBuff;
		for (Force& force : physics2dComp.forceList) {
			memcpy(currBuff, &force, sizeof(Force));
			currBuff += sizeof(Force);
		}
	}

	// RectCollider
	// RectColliderComponent<Whole RectCollider component>
	if (_e.HasComponent<RectCollider>()) {
		RectCollider& rectColliderComp = _e.GetComponent<RectCollider>();
		memcpy(currBuff, rectColliderName, strlen(rectColliderName));
		currBuff += strlen(rectColliderName);
		memcpy(currBuff, &rectColliderComp, sizeof(rectColliderComp));
		currBuff += sizeof(rectColliderComp);
	}

	// CircleCollider
	// CircleColliderComponent<Whole CircleCollider component>
	if (_e.HasComponent<CircleCollider>()) {
		CircleCollider& circleColliderComp = _e.GetComponent<CircleCollider>();
		memcpy(currBuff, circleColliderName, strlen(circleColliderName));
		currBuff += strlen(circleColliderName);
		memcpy(currBuff, &circleColliderComp, sizeof(circleColliderComp));
		currBuff += sizeof(circleColliderComp);
	}

	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialiseEntityDetail(char* currBuff) {
	// General component
	// <name\0><isActive><isPaused>
	char* generalComp = FindSubStringEnd(currBuff, generalName);
	std::string entityName = generalComp;
	Entity e = GameStateManager::GetInstance()->GetEntity(entityName);
	currBuff += entityName.size() + 1;

	e.GetComponent<General>().isActive = static_cast<bool>(currBuff[0]);
	e.GetComponent<General>().isPaused = static_cast<bool>(currBuff[1]);
	currBuff += 2;

#if DEBUGPRINT
	std::cout << "==> General Component\n";
	std::cout << "name: " << e.GetComponent<General>().name << '\n';
	std::cout << "isActive: " << e.GetComponent<General>().isActive << '\n';
	std::cout << "isPaused: " << e.GetComponent<General>().isPaused << '\n';
#endif
	// Transform component
	// TransformComponent<Whole transform component>
	char* transformPtr{ nullptr };
	if (transformPtr = FindSubStringEnd(currBuff, transformName)) {
		currBuff = transformPtr;
		e.GetComponent<Transform>() = *reinterpret_cast<Transform*>(currBuff);
		Transform Trans = e.GetComponent<Transform>();

#if DEBUGPRINT
		std::cout << "==> Transform Component\n";
		std::cout << "Trans.rotation: " << Trans.rotation << '\n';
		std::cout << "Trans.scale: " << Trans.scale << '\n';
		std::cout << "Trans.translation: " << Trans.translation << '\n';
#endif
	}
	
	// Physics2D component
	// Size until before std::vector<Force> is 56
	// Physics2DComponent<56 bytes of physics2dComp><numOfForces><ListOfForces>
	char* physics2dPtr{ nullptr };

	if (physics2dPtr = FindSubStringEnd(currBuff, physics2dName)) {
		Physics2D& physicsComp{ e.GetComponent<Physics2D>() };
		memcpy(&physicsComp, currBuff, 56);
		currBuff += 56;
		int forceSize = static_cast<int>(currBuff[0]);
		++currBuff;

		physicsComp.forceList.clear();
		for (int i{}; i < forceSize; ++i) {
			Force tempForce{ *reinterpret_cast<Force*>(currBuff) };
			physicsComp.forceList.push_back(tempForce);
			currBuff += sizeof(Force);
		}

#if DEBUGPRINT
		Physics2D physics = e.GetComponent<Physics2D>();
		std::cout << "===> Physics2D Component\n";
		std::cout << "mass: " << physics.mass << '\n';
		std::cout << "inertia: " << physics.inertia << '\n';
		std::cout << "restitution: " << physics.restitution << '\n';
		std::cout << "friction: " << physics.friction << '\n';
		std::cout << "damping: " << physics.damping << '\n';
		std::cout << "accumulatedForce: " << physics.accumulatedForce << '\n';
		std::cout << "velocity: " << physics.velocity << '\n';
		std::cout << "acceleration: " << physics.acceleration << '\n';
		std::cout << "angularVelocity: " << physics.angularVelocity << '\n';
		std::cout << "angularTorque: " << physics.angularTorque << '\n';
#endif
	}

	// RectCollider
	// RectColliderComponent<Whole RectCollider component>
	char* rectColliderPtr{ nullptr };
	if (rectColliderPtr = FindSubStringEnd(currBuff, rectColliderName)) {
		currBuff = rectColliderPtr;
		e.GetComponent<RectCollider>() = *reinterpret_cast<RectCollider*>(currBuff);
		
#if DEBUGPRINT
		RectCollider rectcollider = e.GetComponent<RectCollider>();
		std::cout << "===> RectCollider Component\n";
		std::cout << "centerOffset" << rectcollider.centerOffset << '\n';
		std::cout << "scaleOffset" << rectcollider.scaleOffset << '\n';
		std::cout << "isTrigger" << rectcollider.isTrigger << '\n';
		std::cout << "renderFlag" << rectcollider.renderFlag << '\n';
#endif
	}

	// CircleCollider
	// CircleColliderComponent<Whole CircleCollider component>
	char* circleColliderPtr{ nullptr };
	if (circleColliderPtr = FindSubStringEnd(currBuff, circleColliderName)) {
		currBuff = circleColliderPtr;
		e.GetComponent<CircleCollider>() = *reinterpret_cast<CircleCollider*>(currBuff);

#if DEBUGPRINT
		CircleCollider circlecollider = e.GetComponent<CircleCollider>();
		std::cout << "===> CircleCollider Component\n";
		std::cout << "centerOffset" << circlecollider.centerOffset << '\n';
		std::cout << "scaleOffset" << circlecollider.scaleOffset << '\n';
		std::cout << "isTrigger" << circlecollider.isTrigger << '\n';
		std::cout << "renderFlag" << circlecollider.renderFlag << '\n';
#endif
	}

}



int NetworkSerializationManager::SerialisePlayAgainCount() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CPlayAgainCount);

	*reinterpret_cast<int*>(currBuff) = mPlayAgainCount;

	return static_cast<int>(++currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialisePlayAgainCount() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CPlayAgainCount)
		std::cout << "NETWORKDATATYPE::S2CPlayAgainCount\n";

	mPlayAgainCount = *reinterpret_cast<int*>(mRecvBuff + 1);
}


void NetworkSerializationManager::PrepareData(std::string str, int i) {
	dataBankBuffStr = str;
	dataBankBuffInt = i;
}

int NetworkSerializationManager::SerialiseData() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::C2SData);

	memcpy(currBuff, dataBankBuffStr.c_str(), dataBankBuffStr.size() + 1);
	currBuff += dataBankBuffStr.size() + 1;

	*reinterpret_cast<int*>(currBuff) = dataBankBuffInt;
	currBuff += sizeof(int);

	return static_cast<int>(currBuff - mSendBuff);
	
	// Doesn't matter what the data type is, just save it as C2SData
	//if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CPlayAgainCount)
	//	std::cout << "NETWORKDATATYPE::S2CPlayAgainCount\n";
}

void NetworkSerializationManager::DeserialiseData() {
	// Doesn't matter what the data type is, just save it as C2SData
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::C2SData || static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CData)
		std::cout << "NETWORKDATATYPE::C2SData or NETWORKDATATYPE::S2CData\n";

	char* currBuff{ mRecvBuff + 1 };
	std::string tempStr = currBuff;
	currBuff += tempStr.size() + 1;

	int tempInt = *reinterpret_cast<int*>(currBuff);

	dataBank[tempStr] = tempInt;
}

bool NetworkSerializationManager::GetFromBank(std::string name, int* intPtr) {
	try {
		*intPtr = dataBank.at(name);
		dataBank.erase(dataBank.find(name));
		return true;

	} catch (const std::out_of_range& err) {
		return false;
	}
}

//---------------------------------------
// Helper functions
//---------------------------------------


void NetworkSerializationManager::PrintSendBuff() {
	std::cout << "Printing SendBuff\n";
	for (int i{}; i < MAX_UDP_PACKET_SIZE; ++i) {
		std::cout << (mSendBuff[i] != '\0' ? mSendBuff[i] : '*');

	}
	std::cout << "End Printing SendBuff\n";
}

void NetworkSerializationManager::PrintRecvBuff() {
	std::cout << "Printing SendBuff\n";
	for (int i{}; i < MAX_UDP_PACKET_SIZE; ++i) {
		std::cout << (mSendBuff[i] != '\0' ? mSendBuff[i] : '*');

	}
	std::cout << "End Printing SendBuff\n";
}

void NetworkSerializationManager::TestTransferData() {
	memcpy(mRecvBuff, mSendBuff, MAX_UDP_PACKET_SIZE);
}

char* NetworkSerializationManager::FindSubStringEnd(char*& str, const char* findee, int strLen) {
	const char* curr{ findee };
	int findeeLength = strlen(findee);
	int count{};
	for (; strLen != 0; --strLen, ++str) {
		if (*str == *curr) {
			++count;
			++curr;
		} else {
			curr = findee;
			count = 0;
		}
		if (count == findeeLength) {
			return ++str;
		}
	}
	return nullptr;
}
















