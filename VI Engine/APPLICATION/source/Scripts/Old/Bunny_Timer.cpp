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

#include "Bunny_Timer.h"

REGISTER_SCRIPT(ScriptComponent, Bunny_Timer);


void Bunny_Timer::Alive(const Entity& _e) {
	(void)_e;
}

void Bunny_Timer::Init(const Entity& _e) {
	(void)_e;
	timer = 60;
	if (_e.HasComponent<Text>())
		_e.GetComponent<Text>().text = "00:60";
}

void Bunny_Timer::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_Timer::Update(const Entity& _e) {
	if (timer > 0)
		timer -= VI::GetDeltaTime();
	if (_e.HasComponent<Text>())
	{
		_e.GetComponent<Text>().text = "00:";
		_e.GetComponent<Text>().text += std::to_string(static_cast<int>(timer));
	}
	
}

void Bunny_Timer::FixedUpdate(const Entity& _e) {
	(void)_e;
}

void Bunny_Timer::LateUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_Timer::Exit(const Entity& _e) {
	(void)_e;
}

void Bunny_Timer::Dead(const Entity& _e) {
	(void)_e;
}
