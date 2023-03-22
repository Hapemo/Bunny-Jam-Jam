

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


#include "Bunny_GameSceneTransition.h"
#include "Menu_Button.h"

static bool updateOnce{ false };
//bool ConnectionButtonControl::startMenu_{ false };
REGISTER_SCRIPT(ScriptComponent, Bunny_GameSceneTransition);

namespace {
	Entity loadicon_;
	Entity INGAMESTATEMANAGER;
	// menu effect variables
	bool posap_ = false;

	bool player1_ = false;
	bool player2_ = false;

	// scene transition variables
	float acc_ = 300;
	float scaling_ = 1000;
	int zoom_ = 0;

	// text input variables
	std::string ipstring_ = "";
	bool textin_ = false;
	int iplen_ = 0;

}


int Bunny_GameSceneTransition::Transit(int zoom, Entity loadicon, float& acc, float& scaling) {


	if (zoom_ == 1) {
		loadicon.GetComponent<Transform>().scale.x += scaling * (float)FUNC->GetDeltaTime();
		loadicon.GetComponent<Transform>().scale.y += scaling * (float)FUNC->GetDeltaTime();
		if (scaling < 5000) {
			acc += 2000.f * (float)FUNC->GetDeltaTime();
			scaling += acc * (float)FUNC->GetDeltaTime() * 60;
		}
	}

	else if (zoom == 0 && loadicon.GetComponent<Transform>().scale.x >= 0) {


		//std::cout << acc_ << " acceleration \n";
		//std::cout << scaling_ << " scaling \n";
		//std::cout << loadicon_.GetComponent<Transform>().scale.x << " scale \n";

		loadicon.GetComponent<Transform>().scale.x -= scaling * (float)FUNC->GetDeltaTime();
		loadicon.GetComponent<Transform>().scale.y -= scaling * (float)FUNC->GetDeltaTime();


		if (scaling < 5000) {
			acc += 2000.f * (float)FUNC->GetDeltaTime();
			scaling += acc * (float)FUNC->GetDeltaTime() * 60;
		}
	}

	if (loadicon.GetComponent<Transform>().scale.x <= 0) {
		loadicon.GetComponent<Sprite>().color.a = 0;
	}
	else {
		loadicon.GetComponent<Sprite>().color.a = 255;
	}

	if (loadicon_.GetComponent<Transform>().scale.x > 2000 && zoom == 1) {

		return -1;
	}

	if (loadicon_.GetComponent<Transform>().scale.x <= 0 && zoom == 0) {
		INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_COUNTDOWN;
		return -1;
	}

	return zoom;
}



/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void Bunny_GameSceneTransition::Alive(Entity const& _e) {

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void Bunny_GameSceneTransition::Init(Entity const& _e) {
	// acc_ = 1;
	// scaling_ = 1000;
	loadicon_ = VI::iEntity::GetEntity("BunnyLoadGame", "");
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER","");
	//if (zoom_ == 1) {
	{
		if (!INGAMESTATEMANAGER.HasComponent<Bunny_InGameStateComponent>())
			INGAMESTATEMANAGER.AddComponent(Bunny_InGameStateComponent{});
		INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_ZOOMING;
	}

	//	loadicon_.GetComponent<Transform>().scale.x =0.f;
	//	loadicon_.GetComponent<Transform>().scale.y =0.f;
	//}
	//else if (zoom_ == 0) {
	zoom_ = 0;
	loadicon_.GetComponent<Transform>().scale.x = 5000.0f;
	loadicon_.GetComponent<Transform>().scale.y = 5000.0f;
	//}
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void Bunny_GameSceneTransition::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void Bunny_GameSceneTransition::Update(Entity const& _e) {
	//(void)_e;

	if (!updateOnce)
	{
		loadicon_ = VI::iEntity::GetEntity("BunnyLoadGame", "");
	}

	zoom_ = Transit(zoom_, loadicon_, acc_, scaling_);

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void Bunny_GameSceneTransition::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void Bunny_GameSceneTransition::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void Bunny_GameSceneTransition::Exit(Entity const& _e) {

	acc_ = 300;
	scaling_ = 1000;
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void Bunny_GameSceneTransition::Dead(Entity const& _e) {
	loadicon_.GetComponent<Transform>().scale.x = 0;
	loadicon_.GetComponent<Transform>().scale.y = 0;
	zoom_ = true;
	acc_ = 1;
	scaling_ = 1000;
	(void)_e;
}