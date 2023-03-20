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


void BunnyChef_PlayerMovement::Alive(const Entity& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Init(const Entity& _e) {
	(void)_e;
	inited = false;
	sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_NONE;
	 Carrots = 0;

}

void BunnyChef_PlayerMovement::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void BunnyChef_PlayerMovement::Update(const Entity& _e) {

	//I am CHEF
	if (_e.ReadComponent<General>().name == "Chef") { }

	if (!_e.HasComponent<Bunny>())
		_e.AddComponent(Bunny{});

	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::W))
	{
		if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP)
		{
			sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP;
			_e.GetComponent<Physics2D>().acceleration = 0;
			_e.GetComponent<Physics2D>().velocity.x = 0;
			_e.GetComponent<Physics2D>().velocity.y = 0;
		}
	}
	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::S))
	{
		if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN)
		{
			sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN;
			_e.GetComponent<Physics2D>().acceleration = 0;
			_e.GetComponent<Physics2D>().velocity.x = 0;
			_e.GetComponent<Physics2D>().velocity.y = 0;
		}
	}
	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::A))
	{
		if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT)
		{
			if (_e.GetComponent<Transform>().scale.x > 0.0f)
				_e.GetComponent<Transform>().scale.x = -_e.GetComponent<Transform>().scale.x;
			sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT;
			_e.GetComponent<Physics2D>().acceleration = 0;
			_e.GetComponent<Physics2D>().velocity.x = 0;
			_e.GetComponent<Physics2D>().velocity.y = 0;
			
		}
	}
	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::D))
	{
		if (sPLAYERDIRECTION != BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT)
		{
				if (_e.GetComponent<Transform>().scale.x < 0.0f)
					_e.GetComponent<Transform>().scale.x = -_e.GetComponent<Transform>().scale.x;
			sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT;
			_e.GetComponent<Physics2D>().acceleration = 0;
			_e.GetComponent<Physics2D>().velocity.x = 0;
			_e.GetComponent<Physics2D>().velocity.y = 0;
			
		}
	}
	
	switch (sPLAYERDIRECTION)
	{
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_UP:
		VI::iPhysics::ApplyImpulse(_e, { 0.0f,40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_DOWN:
		VI::iPhysics::ApplyImpulse(_e, { 0.0f,-40.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_LEFT:
		VI::iPhysics::ApplyImpulse(_e, { -40.0f,0.0f }, 0.0f);
		break;
	case BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_RIGHT:
		VI::iPhysics::ApplyImpulse(_e, { 40.0f,0.0f }, 0.0f);
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
