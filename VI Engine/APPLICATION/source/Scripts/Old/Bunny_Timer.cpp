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
namespace {
	Entity INGAMESTATEMANAGER;
	Entity countdown;
}


void Bunny_Timer::Alive(const Entity& _e) {
	(void)_e;
}

void Bunny_Timer::Init(const Entity& _e) {
	(void)_e;
	timer = 60;
	countdowntimer = 4.0f;
	if (_e.HasComponent<Text>())
		_e.GetComponent<Text>().text = "00:60";
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	countdown = VI::iEntity::GetEntity("BUNNYCOUNTDOWN","");

}

void Bunny_Timer::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_Timer::Update(const Entity& _e) {

	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	std::cout << "state: " << INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs << std::endl;
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_COUNTDOWN)
	{
		if (countdowntimer > 0)
		{
			if (countdown.HasComponent<Text>())
				countdown.GetComponent<Text>().color.a = 255;
			countdowntimer -= VI::GetDeltaTime();
		}
		else
		{
			if (countdown.HasComponent<Text>())
				countdown.GetComponent<Text>().color.a = 0;
			INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_INGAME;
		}

		if (countdown.HasComponent<Text>())
		{
			countdown.GetComponent<Text>().text = std::to_string(static_cast<int>(countdowntimer));
			if (countdowntimer < 1)
				countdown.GetComponent<Text>().text = "GO!";
		}

	}
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_INGAME)
	{
		if (timer > 0)
			timer -= VI::GetDeltaTime();
		if (_e.HasComponent<Text>())
		{
			_e.GetComponent<Text>().text = "00:";
			if (timer < 10.0f)
			{
				_e.GetComponent<Text>().text += "0";
			}
			_e.GetComponent<Text>().text += std::to_string(static_cast<int>(timer));
		}
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
