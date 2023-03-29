/*!*****************************************************************************
\file PlayerMovementScript.cpp
\author Lee Hsien Wei, Joachim, Kew Yu Jun, Chen Jia Wen
\par DP email: l.hsienweijoachim@digipen.edu, c.jiawen\@digipen.edu
\par Course: GAM200
\par Group: Memory Leak Studios
\date 24-11-2022
\brief
The PlayerMovementScript allows the player to move around the map according to
the mouse click position. It also handles the sprites changes according to the
direction the boat is facing. The end dialogue scene camera zoom out is also
handled here.

Press "CTRL+SHIFT+L" to skip to the game over screen.
Press "CTRL+SHIFT+M" to toggle player invincible mode.
Press "CTRL+SHIFT+B" to toggle player's speed increase.
Press "ESC" to toggle the pause menu.
*******************************************************************************/

#include "BunnyChef_PlayerMovement.h"
#include "NetworkSerialization.h"
#include "ServerManager.h"
#include "ClientManager.h"
REGISTER_SCRIPT(ScriptComponent, BunnyChef_PlayerMovement);
bool isPlayer1{ false };
bool BunnyOrChef{ false }; //False = Bunny , True = Chef
bool clientPrediction{ false };
bool serverReconciliation{ false };
bool EntityInterpolation{ false };
namespace
{
	Entity BunnyPlayer;
	Entity ChefPlayer;
	Entity INGAMESTATEMANAGER;
	Entity clientPred;
	Entity ServerRecon;
	Entity entityInter;
	//Transform Bunny_prevXform{};
	//Transform Chef_prevXform{};
}


// Helper functions
void MoveCharacter(Entity _e, BUNNY_PLAYER_DIRECTION direction) {
	float movementSpeed{ 40.f };

	switch (direction) {
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
		VI::iPhysics::ApplyImpulse(_e, { 0, movementSpeed }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
		VI::iPhysics::ApplyImpulse(_e, { 0, -movementSpeed }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
		VI::iPhysics::ApplyImpulse(_e, { -movementSpeed, 0 }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
		VI::iPhysics::ApplyImpulse(_e, { movementSpeed, 0 }, 0.0f);
		break;
	}
}


void BunnyChef_PlayerMovement::Alive(const Entity& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Init(const Entity& _e) {
	(void)_e;
	inited = false;
	sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_NONE;
	sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_NONE;
	Carrots = 0;
	


	BunnyPlayer = VI::iEntity::GetEntity("Bunny", "");
	ChefPlayer = VI::iEntity::GetEntity("Chef", "");
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");

	//Network Toggles
	clientPred = VI::iEntity::GetEntity("clientPred", "");
	ServerRecon = VI::iEntity::GetEntity("serverRecon", "");
	entityInter = VI::iEntity::GetEntity("entityInter", "");






}

void BunnyChef_PlayerMovement::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Update(const Entity& _e)
{
	(void)_e;
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	NetworkSerializationManager* manager = NetworkSerializationManager::GetInstance();
	ServerManager* server = ServerManager::GetInstance();
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_INGAME) {







#ifdef _SERVER
		// Update character directions
			//Move Up
		if (server->mP1Input[0])
		{
			{
				/*		BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/
				VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,4000.0f }, 0.0f);
			}
		}

		//Move Down
		if (server->mP1Input[1])
		{
			{
				//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,-4000.0f }, 0.0f);

			}
		}

		//Move Left
		if (server->mP1Input[2])
		{
			{
				if (BunnyPlayer.GetComponent<Transform>().scale.x > 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				VI::iPhysics::ApplyImpulse(BunnyPlayer, { -4000.0f,0.0f }, 0.0f);

			}
		}

		//Move Right
		if (server->mP1Input[3])
		{
			{
				if (BunnyPlayer.GetComponent<Transform>().scale.x < 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				/*			BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
							BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
							BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/

				VI::iPhysics::ApplyImpulse(BunnyPlayer, { 4000.0f,0.0f }, 0.0f);
			}
		}

		//Move Up
		if (server->mP2Input[0])
		{
			{
				/*		BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/
				VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,4000.0f }, 0.0f);
			}
		}

		//Move Down
		if (server->mP2Input[1])
		{

			{
				//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,-4000.0f }, 0.0f);

			}
		}

		//Move Left
		if (server->mP2Input[2])
		{
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x > 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				VI::iPhysics::ApplyImpulse(ChefPlayer, { -4000.0f,0.0f }, 0.0f);

			}
		}

		//Move Right
		if (server->mP2Input[3])
		{
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x < 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				/*			BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
							BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
							BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/

				VI::iPhysics::ApplyImpulse(ChefPlayer, { 4000.0f,0.0f }, 0.0f);
			}
		}
		// Update bunny movement
#endif

	//std::cout << "state: " << INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs << std::endl;


#ifdef _CLIENT


	// Network Toggle
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::_1)) { clientPrediction = !clientPrediction; }
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::_2)) { 
			serverReconciliation = !serverReconciliation; 
			ClientManager::GetInstance()->serverReconcilation = serverReconciliation;
		}
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::_3))
		{
			EntityInterpolation = !EntityInterpolation;
			NetworkSerializationManager::FlipEntityInterpolation();
		}
		clientPred.GetComponent<Sprite>().color = (clientPrediction) ? Color{ 255, 255, 255, 255 } : Color{ 255, 255, 255, 0 };
		ServerRecon.GetComponent<Sprite>().color = (serverReconciliation) ? Color{ 255, 255, 255, 255 } : Color{ 255, 255, 255, 0 };
		entityInter.GetComponent<Sprite>().color = (EntityInterpolation) ? Color{ 255, 255, 255, 255 } : Color{ 255, 255, 255, 0 };
		//if (clientPrediction) {}
		//if (serverReconciliation) {}
		if (EntityInterpolation) {
			Transform tmp{};
			if (NetworkSerializationManager::GetInstance()->mPlayerID == 1)
			{
				//ChefPlayer.GetComponent<Transform>() = NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), ChefPlayer.GetComponent<Transform>());
					//std::cout << "c:" << ChefPlayer.GetComponent<Transform>().translation.x << std::endl;
				ChefPlayer.GetComponent<Transform>().translation.x += NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), ChefPlayer.GetComponent<Transform>()).translation.x;
				ChefPlayer.GetComponent<Transform>().translation.y += NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), ChefPlayer.GetComponent<Transform>()).translation.y;
			
			}
			if (NetworkSerializationManager::GetInstance()->mPlayerID == 2)
			{
				//BunnyPlayer.GetComponent<Transform>() = NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), BunnyPlayer.GetComponent<Transform>());

				BunnyPlayer.GetComponent<Transform>().translation.x += NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), BunnyPlayer.GetComponent<Transform>()).translation.x;
				BunnyPlayer.GetComponent<Transform>().translation.y += NetworkSerializationManager::GetInstance()->EntityInterpolation(NetworkSerializationManager::GetInstance()->GetCurrXform(), BunnyPlayer.GetComponent<Transform>()).translation.y;
					//std::cout << "b: "<< BunnyPlayer.GetComponent<Transform>().translation.x << std::endl;
			}
		}
		else
		{
			if (NetworkSerializationManager::GetInstance()->mPlayerID == 1)
				NetworkSerializationManager::GetInstance()->GetCurrXform() = ChefPlayer.GetComponent<Transform>();
			if (NetworkSerializationManager::GetInstance()->mPlayerID == 2)
				NetworkSerializationManager::GetInstance()->GetCurrXform() = BunnyPlayer.GetComponent<Transform>();
		}


		if (!BunnyPlayer.HasComponent<Bunny>())
			BunnyPlayer.AddComponent(Bunny{});

		
		if (NetworkSerializationManager::GetInstance()->mPlayerID == 1)
		{
			if (clientPrediction)
			{
				//Move Up
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::W))
				{
					{
						/*		BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
								BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
								BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/
						VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,4000.0f }, 0.0f);
					}
				}

				//Move Down
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::S))
				{
					{
						//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
						VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,-4000.0f }, 0.0f);

					}
				}

				//Move Left
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::A))
				{
					{
						if (BunnyPlayer.GetComponent<Transform>().scale.x > 0.0f)
							BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
						//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
						VI::iPhysics::ApplyImpulse(BunnyPlayer, { -4000.0f,0.0f }, 0.0f);

					}
				}

				//Move Right
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::D))
				{
					{
						if (BunnyPlayer.GetComponent<Transform>().scale.x < 0.0f)
							BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
						/*			BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
									BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
									BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/

						VI::iPhysics::ApplyImpulse(BunnyPlayer, { 4000.0f,0.0f }, 0.0f);
					}
				}
			}
		}

		//Move Up
		if (NetworkSerializationManager::GetInstance()->mPlayerID == 2) 
		{
			if (clientPrediction)
			{
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::W))
				{
					{
						/*		BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
								BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
								BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/
						VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,4000.0f }, 0.0f);
					}
				}

				//Move Down
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::S))
				{

					{
						//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
						VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,-4000.0f }, 0.0f);

					}
				}

				//Move Left
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::A))
				{
					{
						if (ChefPlayer.GetComponent<Transform>().scale.x > 0.0f)
							ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
						sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
						//BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
						//BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
						VI::iPhysics::ApplyImpulse(ChefPlayer, { -4000.0f,0.0f }, 0.0f);

					}
				}

				//Move Right
				if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::D))
				{
					{
						if (ChefPlayer.GetComponent<Transform>().scale.x < 0.0f)
							ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
						/*			BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
									BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
									BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;*/

						VI::iPhysics::ApplyImpulse(ChefPlayer, { 4000.0f,0.0f }, 0.0f);
					}
				}
			}
		}
#endif
	}
	//if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::SPACE)) { BunnyOrChef = !BunnyOrChef; }
}
void BunnyChef_PlayerMovement::FixedUpdate(const Entity& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::LateUpdate(Entity const& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Exit(const Entity& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Dead(const Entity& _e) {
	(void)_e;
}
