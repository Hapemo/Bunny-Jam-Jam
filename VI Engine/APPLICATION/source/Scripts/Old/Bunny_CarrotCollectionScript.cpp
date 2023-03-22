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

#include "Bunny_CarrotCollectionScript.h"

REGISTER_SCRIPT(ScriptComponent, Bunny_CarrotCollectionScript);

namespace {
	Entity BUNNY_Player; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
}

void Bunny_CarrotCollectionScript::Alive(const Entity& _e) {
	(void)_e;
}

void Bunny_CarrotCollectionScript::Init(const Entity& _e) {
	(void)_e;
	BUNNY_Player = VI::iEntity::GetEntity("Bunny", "");
	
}

void Bunny_CarrotCollectionScript::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_CarrotCollectionScript::Update(const Entity& _e) {

	if (VI::iPhysics::CheckCollision(BUNNY_Player, _e, false))
	{
		BUNNY_Player.GetComponent<Bunny>().carrots++;
		_e.Deactivate();
	}
}

void Bunny_CarrotCollectionScript::FixedUpdate(const Entity& _e) {
	(void)_e;
}

void Bunny_CarrotCollectionScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_CarrotCollectionScript::Exit(const Entity& _e) {
	(void)_e;
}

void Bunny_CarrotCollectionScript::Dead(const Entity& _e) {
	(void)_e;
}
