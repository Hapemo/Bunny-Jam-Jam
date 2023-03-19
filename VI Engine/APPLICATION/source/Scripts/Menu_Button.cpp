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


#include "Menu_Button.h"
static bool updateOnce{ false };

REGISTER_SCRIPT(ScriptComponent, Menu_Button);

namespace {
	Entity playButton; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED

}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void Menu_Button::Alive(Entity const& _e) {

	//bgeff_ = VI::iEntity::GetEntity("BGEffect", "Game");
	//bgeff2_ = VI::iEntity::GetEntity("BGEffect2", "Game");
	//ip_ = VI::iEntity::GetEntity("IP", "RequestIP");
	playButton = VI::iEntity::GetEntity("Play_Button", "");

	if (playButton.id == 0) return;

	/*textinput_ =*/

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void Menu_Button::Init(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void Menu_Button::EarlyUpdate(Entity const& _e) {

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void Menu_Button::Update(Entity const& _e) {
	//(void)_e;
	if (!updateOnce)
	{
		playButton = VI::iEntity::GetEntity("Play_Button", "");
		std::cout << "HELLO" << std::endl;
		updateOnce = true;
	}

	//If PlayButton is clicked
	if (playButton.GetComponent<Button>().isHover) {

		if (playButton.GetComponent<Button>().isClick) {
			VI::iScene::Play("Game");
			VI::iScene::Pause("Bunny_Menu");
		}
	}
	
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void Menu_Button::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void Menu_Button::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void Menu_Button::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void Menu_Button::Dead(Entity const& _e) {
	(void)_e;
}
