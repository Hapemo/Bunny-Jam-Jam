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

REGISTER_SCRIPT(ScriptComponent, BunnyChef_PlayerMovement);

namespace 
{
	Entity BunnyPlayer;
	Entity ChefPlayer;
}
bool isPlayer{ false };


void BunnyChef_PlayerMovement::Alive(const Entity& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Init(const Entity& _e) {
	(void)_e;
	inited = false;
	sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_NONE;
	Carrots = 0;
	if (_e.ReadComponent<General>().name == "Bunny")
	{
		isPlayer = false;
		BunnyPlayer = VI::iEntity::GetEntity("Bunny", "");

	}
	else
	{
		isPlayer = true;
		BunnyPlayer = VI::iEntity::GetEntity("Chef", "");

	}
	//ChefPlayer = VI::iEntity::GetEntity("Bunny", "");


}

void BunnyChef_PlayerMovement::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Update(const Entity& _e) {
	(void)_e;

	if (!BunnyPlayer.HasComponent<Bunny>())
		BunnyPlayer.AddComponent(Bunny{});


	
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
