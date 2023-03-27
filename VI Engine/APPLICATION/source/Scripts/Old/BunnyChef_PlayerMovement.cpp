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

REGISTER_SCRIPT(ScriptComponent, BunnyChef_PlayerMovement);
bool isPlayer1{ false };
bool BunnyOrChef{ false }; //False = Bunny , True = Chef
namespace 
{
	Entity BunnyPlayer;
	Entity ChefPlayer;
	Entity INGAMESTATEMANAGER;
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
	//std::cout << "state: " << INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs << std::endl;
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_INGAME) {
#ifdef _SERVER
		// BUNNY
		//Move Up
		if (NetworkSerializationManager::GetInstance()->mP1InputW) {
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP) {
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}

		//Move Down
		if (NetworkSerializationManager::GetInstance()->mP1InputS) {
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN) {
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}

		//Move Left
		if (NetworkSerializationManager::GetInstance()->mP1InputA) {
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT) {
				if (BunnyPlayer.GetComponent<Transform>().scale.x > 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}

		//Move Right
		if (NetworkSerializationManager::GetInstance()->mP1InputD) {
			if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT) {
				if (BunnyPlayer.GetComponent<Transform>().scale.x < 0.0f)
					BunnyPlayer.GetComponent<Transform>().scale.x = -BunnyPlayer.GetComponent<Transform>().scale.x;
				sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
				BunnyPlayer.GetComponent<Physics2D>().acceleration = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.x = 0;
				BunnyPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}


		// CHEF JO AL
		if (NetworkSerializationManager::GetInstance()->mP2InputW) {
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP) {
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (NetworkSerializationManager::GetInstance()->mP2InputS) {
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN) {
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;
			}
		}
		if (NetworkSerializationManager::GetInstance()->mP2InputA) {
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT) {
				if (ChefPlayer.GetComponent<Transform>().scale.x > 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
		if (NetworkSerializationManager::GetInstance()->mP2InputD) {
			if (sCHEFDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT) {
				if (ChefPlayer.GetComponent<Transform>().scale.x < 0.0f)
					ChefPlayer.GetComponent<Transform>().scale.x = -ChefPlayer.GetComponent<Transform>().scale.x;
				sCHEFDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
				ChefPlayer.GetComponent<Physics2D>().acceleration = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.x = 0;
				ChefPlayer.GetComponent<Physics2D>().velocity.y = 0;

			}
		}
	}
#endif
		//if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::SPACE)) { BunnyOrChef = !BunnyOrChef; }

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
