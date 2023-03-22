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
		
	case NETWORKDATATYPE::S2CNumOfPlayerReplay:
		//dataSize = SerialiseNumOfPlayerReplay();
		break;
	}

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

	case NETWORKDATATYPE::S2CNumOfClientConnected:
		DeserialiseNumberOfClientConnected();
		break;

	case NETWORKDATATYPE::S2CGamePlayData:
		break;

	case NETWORKDATATYPE::S2CEntityDetail:
		break;

	case NETWORKDATATYPE::S2CGameStats:
		break;

	case NETWORKDATATYPE::S2CNumOfPlayerReplay:
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
	// So 4 bytes in total.

	E_KEY up = E_KEY::W;
	E_KEY down = E_KEY::S;
	E_KEY left = E_KEY::A;
	E_KEY right = E_KEY::D;

	if (Input::CheckKey(PRESS, up) || Input::CheckKey(HOLD, up)) 
		*currBuff = 1;
	++currBuff;
	if (Input::CheckKey(PRESS, down) || Input::CheckKey(HOLD, down)) 
		*currBuff = 1;
	++currBuff;
	if (Input::CheckKey(PRESS, left) || Input::CheckKey(HOLD, left)) 
		*currBuff = 1;
	++currBuff;
	if (Input::CheckKey(PRESS, right) || Input::CheckKey(HOLD, right)) 
		*currBuff = 1;
	++currBuff;

	return static_cast<int>(currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialisePlayerControls() {
	bool up{static_cast<bool>(mRecvBuff[1])};
	bool down{static_cast<bool>(mRecvBuff[2])};
	bool left{static_cast<bool>(mRecvBuff[3])};
	bool right{static_cast<bool>(mRecvBuff[4])};

#if DEBUGPRINT
	std::cout << "up: " << up << '\n';
	std::cout << "down: " << down << '\n';
	std::cout << "left: " << left << '\n';
	std::cout << "right: " << right << '\n';
#endif
	//PrintSendBuff();
}


int NetworkSerializationManager::SerialisePlayAgain() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::C2SPlayAgain);
	
	// TODO In script, get the bool value of play again or not.
	bool playAgain{ false };
	currBuff[0] = static_cast<char>(playAgain);

	return static_cast<int>(++currBuff - mSendBuff);
}

void NetworkSerializationManager::DeserialisePlayAgain() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::C2SPlayAgain)
		std::cout << "NETWORKDATATYPE::C2SPlayAgain\n";
	
	bool playAgain{ static_cast<bool>(mRecvBuff[1]) };
	//std::cout << "playAgain: " << playAgain << '\n';

}

//-------------------------------
// Server to client
//-------------------------------
       
int NetworkSerializationManager::SerialiseNumberOfClientConnected() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CNumOfClientConnected);

	int numOfClients{ 123 };
	currBuff[0] = static_cast<char>(numOfClients);

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

int NetworkSerializationManager::SerialiseGameStats(char*& currBuff) {
	// int jam collected, float time remaining, bool swap
	int jam{};
	float timeRemain{};
	bool swap{};

	*(reinterpret_cast<int*>(currBuff)) = jam;
	currBuff += sizeof(int);
	*(reinterpret_cast<int*>(currBuff)) = timeRemain;
	currBuff += sizeof(float);
	*(reinterpret_cast<bool*>(currBuff)) = swap;
	currBuff += sizeof(bool);
	
	return static_cast<int>(currBuff - mSendBuff);
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

void NetworkSerializationManager::DeserialiseMultipleEntities() {
	if (static_cast<NETWORKDATATYPE>(mRecvBuff[0]) == NETWORKDATATYPE::S2CEntityDetail)
		std::cout << "NETWORKDATATYPE::S2CEntityDetail\n";

	char* currBuff{ mRecvBuff + 1 };
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



int NetworkSerializationManager::SerialiseNumberOfPlayerReplay() {
	memset(mSendBuff, 0, MAX_UDP_PACKET_SIZE);
	char* currBuff{ mSendBuff + 1 };
	mSendBuff[0] = static_cast<char>(NETWORKDATATYPE::S2CNumOfClientConnected);

	int numOfClients{ 0 };
	currBuff[0] = static_cast<char>(numOfClients);

	return static_cast<int>(++currBuff - mSendBuff);
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
















