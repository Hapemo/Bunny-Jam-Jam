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

REGISTER_SCRIPT(ScriptComponent, BunnyChef_PlayerMovement);
bool isPlayer1{ false };
bool BunnyOrChef{ false }; //False = Bunny , True = Chef
namespace 
{
	Entity BunnyPlayer;
	Entity ChefPlayer;
	Entity INGAMESTATEMANAGER;
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

	



}

void BunnyChef_PlayerMovement::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Update(const Entity& _e) {
	(void)_e;
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	NetworkSerializationManager* manager = NetworkSerializationManager::GetInstance();
	ServerManager* server = ServerManager::GetInstance();
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_INGAME) {
#ifdef _SERVER
		// Update character directions
		/*float movementSpeed{ 40.f };

		if (server->mP1Input[0]) {
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0, movementSpeed }, 0.0f);
			std::cout << "mP1InputW\n";
		}
		else if (server->mP1Input[1]) {
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0, -movementSpeed }, 0.0f);
			std::cout << "mP1InputS\n";
		}
		else if (server->mP1Input[2]) {
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { -movementSpeed, 0 }, 0.0f);
			std::cout << "mP1InputA\n";
		}
		else if (server->mP1Input[3]) {
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { movementSpeed, 0 }, 0.0f);
			std::cout << "mP1InputD\n";
		}

		if (server->mP2Input[0]) {
			VI::iPhysics::ApplyImpulse(ChefPlayer, { 0, movementSpeed }, 0.0f);
			std::cout << "mP2InputW\n";
		}
		else if (server->mP2Input[1]) {
			VI::iPhysics::ApplyImpulse(ChefPlayer, { 0, -movementSpeed }, 0.0f);
			std::cout << "mP2InputS\n";
		}
		else if (server->mP2Input[2]) {
			VI::iPhysics::ApplyImpulse(ChefPlayer, { -movementSpeed, 0 }, 0.0f);
			std::cout << "mP2InputA\n";
		}
		else if (server->mP2Input[3]) {
			VI::iPhysics::ApplyImpulse(ChefPlayer, { movementSpeed, 0 }, 0.0f);
			std::cout << "mP2InputD\n";
		}*/

		{
			if (server->mP1Input[0])
			{
				if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP)
				{
					sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
					BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				}
			}

			//Move Down
			if (server->mP1Input[1])
			{
				if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN)
				{
					sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
					BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
				}
			}

			//Move Left
			if (server->mP1Input[2])
			{
				if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT)
				{
					if (BunnyPlayer.GetComponent<Transform>().scale.x > 0.0f)
						BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
					sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
					BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

				}
			}

			//Move Right
			if (server->mP1Input[3])
			{
				if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT)
				{
					if (BunnyPlayer.GetComponent<Transform>().scale.x < 0.0f)
						BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
					sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
					BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
					BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

				}
			}
		}
		switch (sPLAYERDIRECTION)
		{
		case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,40.0f }, 0.0f);
			break;
		case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,-40.0f }, 0.0f);
			break;
		case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { -40.0f,0.0f }, 0.0f);
			break;
		case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
			VI::iPhysics::ApplyImpulse(BunnyPlayer, { 40.0f,0.0f }, 0.0f);
			break;
		default:
			break;
		}
	}



	//Mac Chef

	{
		if (server->mP2Input[0])
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP)
			{
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (server->mP2Input[1])
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN)
			{
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (server->mP2Input[2])
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT)
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x > 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
		if (server->mP2Input[3])
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT)
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x < 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
	}

	switch (sCHEFDIRECTION)
	{
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,-40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { -40.0f,0.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 40.0f,0.0f }, 0.0f);
		break;
	default:
		break;
		//std::cout << "mP1Input: " << manager->mP1Input << '\n';
		//std::cout << "mP2Input: " << manager->mP2Input << '\n';

	}
	// Update bunny movement
	//MoveCharacter(BunnyPlayer, sPLAYERDIRECTION);
	//MoveCharacter(ChefPlayer, sCHEFDIRECTION);
#endif

	//std::cout << "state: " << INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs << std::endl;


#ifdef _CLIENT

	if (!BunnyPlayer.HasComponent<Bunny>())
		BunnyPlayer.AddComponent(Bunny{});


	//Move Up
	if (NetworkSerializationManager::GetInstance()->mPlayerID == 1)
	{
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::W))
		{
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP)
			{
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}

		//Move Down
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::S))
		{
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN)
			{
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}

		//Move Left
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::A))
		{
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT)
			{
				if (BunnyPlayer.GetComponent<Transform>().scale.x > 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}

		//Move Right
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::D))
		{
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT)
			{
				if (BunnyPlayer.GetComponent<Transform>().scale.x < 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
	}
	switch (sPLAYERDIRECTION)
	{
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
		VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
		VI::iPhysics::ApplyImpulse(BunnyPlayer, { 0.0f,-40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
		VI::iPhysics::ApplyImpulse(BunnyPlayer, { -40.0f,0.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
		VI::iPhysics::ApplyImpulse(BunnyPlayer, { 40.0f,0.0f }, 0.0f);
		break;
	default:
		break;
	}



	//Mac Chef

	if (NetworkSerializationManager::GetInstance()->mPlayerID == 2)
	{
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::W))
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP)
			{
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::S))
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN)
			{
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::A))
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT)
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x > 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::D))
		{
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT)
			{
				if (ChefPlayer.GetComponent<Transform>().scale.x < 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
	}

	switch (sCHEFDIRECTION)
	{
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 0.0f,-40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { -40.0f,0.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
		VI::iPhysics::ApplyImpulse(ChefPlayer, { 40.0f,0.0f }, 0.0f);
		break;
	default:
		break;
	}
#endif
	}
//if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::SPACE)) { BunnyOrChef = !BunnyOrChef; }
#ifndef _SERVER
}
#endif // _SERVER

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
