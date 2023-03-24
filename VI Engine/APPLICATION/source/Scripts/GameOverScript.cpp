/*!*****************************************************************************
\file FPSPrint.cpp
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Course: GAM250
\par Group: Memory Leak Studios
\date 25-01-2023
\brief
Updates the fps count, for the fps printer in entity
*******************************************************************************/


#include "GameOverScript.h"
#include "Bunny_MatchOver.h"

REGISTER_SCRIPT(ScriptComponent,GameOverScript);

namespace {
	Entity winnerBunny;
	Entity winnerChef;
	Entity exitButton;




}



/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void GameOverScript::Alive(Entity const& _e) {

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void GameOverScript::Init(Entity const& _e) {


	winnerBunny = VI::iEntity::GetEntity("winnerBunny", "");

	winnerChef = VI::iEntity::GetEntity("winnerChef", "");

	exitButton = VI::iEntity::GetEntity("exit_button", "");



	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void GameOverScript::EarlyUpdate(Entity const& _e) {


	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void GameOverScript::Update(Entity const& _e) {
	//(void)_e;





	//code to exit
	if (exitButton.GetComponent<Button>().isHover) {

		if (exitButton.GetComponent<Button>().isClick) 
		{

		}
	}

	//if (!Bunny_MatchOver::bunnywin)
	{

		//winnerChef.GetComponent<Sprite>().color = { 255,255,255,0 };
	}
	//else
	{
		winnerBunny.GetComponent<Sprite>().color = { 255,255,255,0 };

	}

	

}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void GameOverScript::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void GameOverScript::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void GameOverScript::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void GameOverScript::Dead(Entity const& _e) {
	(void)_e;
}
