

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


#include "Bunny_MatchOver.h"
#include "Menu_Button.h"
#include "Bunny_PlayerMovement.h"

static bool updateOnce{ false };
//bool ConnectionButtonControl::startMenu_{ false };
REGISTER_SCRIPT(ScriptComponent, Bunny_MatchOver);

namespace {
	Entity bunny_at_matchover;
	Entity chef_at_matchover;
	Entity INGAMESTATEMANAGER;
	float const Bunny_CollidedlimitDOWN = 1111.0f;
}


int Bunny_MatchOver::Transit(int zoom, Entity loadicon, float& acc, float& scaling) {
	(void)zoom;
	(void)loadicon;
	(void)acc;
	(void)scaling;

	//if (zoom == 1) {
	//	loadicon.GetComponent<Transform>().scale.x += scaling * (float)FUNC->GetDeltaTime();
	//	loadicon.GetComponent<Transform>().scale.y += scaling * (float)FUNC->GetDeltaTime();
	//	if (scaling < 5000) {
	//		acc += 2000.f * (float)FUNC->GetDeltaTime();
	//		scaling += acc * (float)FUNC->GetDeltaTime() * 60;
	//	}
	//}

	//else if (zoom == 0 && loadicon.GetComponent<Transform>().scale.x >= 0) {


	//	//std::cout << acc_ << " acceleration \n";
	//	//std::cout << scaling_ << " scaling \n";
	//	//std::cout << loadicon_.GetComponent<Transform>().scale.x << " scale \n";

	//	loadicon.GetComponent<Transform>().scale.x -= scaling * (float)FUNC->GetDeltaTime();
	//	loadicon.GetComponent<Transform>().scale.y -= scaling * (float)FUNC->GetDeltaTime();


	//	if (scaling < 5000) {
	//		acc += 2000.f * (float)FUNC->GetDeltaTime();
	//		scaling += acc * (float)FUNC->GetDeltaTime() * 60;
	//	}
	//}

	//if (loadicon.GetComponent<Transform>().scale.x <= 0) {
	//	loadicon.GetComponent<Sprite>().color.a = 0;
	//}
	//else {
	//	loadicon.GetComponent<Sprite>().color.a = 255;
	//}

	//if (loadicon_.GetComponent<Transform>().scale.x > 2000 && zoom == 1) {
	//	return -1;
	//}

	//if (loadicon_.GetComponent<Transform>().scale.x <= 0 && zoom == 0) {
	//	return -1;
	//}

	return zoom;
}



/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void Bunny_MatchOver::Alive(Entity const& _e) {

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void Bunny_MatchOver::Init(Entity const& _e) {
	bunny_at_matchover = VI::iEntity::GetEntity("Bunny", "");
	chef_at_matchover= VI::iEntity::GetEntity("Chef", "");
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void Bunny_MatchOver::EarlyUpdate(Entity const& _e) {
	(void)_e;
	
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void Bunny_MatchOver::Update(Entity const& _e) {
	(void)_e;

	bunny_at_matchover = VI::iEntity::GetEntity("Bunny", "");
	chef_at_matchover = VI::iEntity::GetEntity("Chef", "");
	INGAMESTATEMANAGER = VI::iEntity::GetEntity("INGAMESTATEMANAGER", "");
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_INGAME)
	{
		if (VI::iPhysics::CheckCollision(bunny_at_matchover, chef_at_matchover))
		{
			Bunny_PlayerMovement::stopPlayer(chef_at_matchover);
			Bunny_PlayerMovement::stopPlayer(bunny_at_matchover);
			bunny_at_matchover.GetComponent<RectCollider>().isTrigger = true;
			VI::iPhysics::ApplyImpulse(bunny_at_matchover, { 0.0f,500.0f }, { 0.0f });
			INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_COLLIDED;
		}
		if (bunny_at_matchover.HasComponent<Bunny>())
		{
			if (bunny_at_matchover.GetComponent<Bunny>().carrots == 25)
				INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_WIN;
		}
	}
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_COLLIDED)
	{
		if (bunny_at_matchover.GetComponent<Physics2D>().velocity.y < 40.0f)
			VI::iPhysics::ApplyImpulse(bunny_at_matchover, { 0.0f,-40.0f }, { 0.0f });
		if (bunny_at_matchover.GetComponent<Transform>().translation.y < -Bunny_CollidedlimitDOWN)
			INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs = BUNNY_MATCHOVER;
	}
	
	// jh here
	if (INGAMESTATEMANAGER.GetComponent<Bunny_InGameStateComponent>().bigs == BUNNY_MATCHOVER)
	{

	}

	

}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void Bunny_MatchOver::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void Bunny_MatchOver::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void Bunny_MatchOver::Exit(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void Bunny_MatchOver::Dead(Entity const& _e) {
	(void)_e;
}