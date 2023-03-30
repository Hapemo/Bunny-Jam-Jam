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
#include "ServerManager.h"
#include "Bunny_MatchOver.h"
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
	ServerManager::GetInstance()->mTimeRemaining = 120;
	countdowntimer = 4.0f;
	if (_e.HasComponent<Text>())
		_e.GetComponent<Text>().text = "00:60";
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	countdown = VI::iEntity::GetEntity("BUNNYCOUNTDOWN", "");

}

void Bunny_Timer::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

void Bunny_Timer::Update(const Entity& _e) {

	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_COUNTDOWN)
	{
		_e.GetComponent<Text>().offset.x = -190.0f;
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
		_e.GetComponent<Text>().offset.x = -190.0f;
		if (ServerManager::GetInstance()->mTimeRemaining > 0)
			ServerManager::GetInstance()->mTimeRemaining -= VI::GetDeltaTime();
		else
		{
			INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_TIMEUP;
			ServerManager::GetInstance()->mTimeRemaining = 3.0f;
		}
		if (_e.HasComponent<Text>())
		{
			_e.GetComponent<Text>().text = "00:";
			if (ServerManager::GetInstance()->mTimeRemaining < 10.0f)
			{
				_e.GetComponent<Text>().text += "0";
				_e.GetComponent<Text>().color.r = 255;
				_e.GetComponent<Text>().color.g = 0;
				_e.GetComponent<Text>().color.b = 0;
			}
			else
			{
				_e.GetComponent<Text>().color.r = 255;
				_e.GetComponent<Text>().color.g = 255;
				_e.GetComponent<Text>().color.b = 255;
			}

			_e.GetComponent<Text>().text += std::to_string(static_cast<int>(ServerManager::GetInstance()->mTimeRemaining));
		}
	}
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_TIMEUP)
	{
		_e.GetComponent<Text>().offset.x = -108.0f;
		_e.GetComponent<Text>().text = "TIMES UP!";
		if (ServerManager::GetInstance()->mTimeRemaining > 0)
			ServerManager::GetInstance()->mTimeRemaining -= VI::GetDeltaTime();
		else
			INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_WIN;

	}
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_WIN)
	{
		Bunny_MatchOver::getwin() = true;
		VI::iGameState::ChangeGameState("Bunny_GameOver");
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
