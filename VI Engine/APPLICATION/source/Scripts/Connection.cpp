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


#include "Connection.h"

REGISTER_SCRIPT(ScriptComponent, Connection);

namespace {
	Entity board; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
	bool enlarge = true;
	//std::string apple = "dasdasd";
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void Connection::Alive(Entity const& _e) {
	board = VI::iEntity::GetEntity("Board", "");
	if (board.id == 0) return;

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void Connection::Init(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void Connection::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void Connection::Update(Entity const& _e) {
	//(void)_e;

	if (_e.GetComponent<Transform>().scale.x < 100 && enlarge == false) {
		enlarge = true;
	}
	if (_e.GetComponent<Transform>().scale.x > 200 && enlarge == true) {
		enlarge = false;
	}

	if (enlarge == true) {
		_e.GetComponent<Transform>().scale.x += 100.f* (float)FUNC->GetDeltaTime();
	}
	else {
		_e.GetComponent<Transform>().scale.x -= 100.f * (float)FUNC->GetDeltaTime();

	}

	//_e.GetComponent<Text>().text = "sgdsdadas";
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void Connection::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void Connection::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void Connection::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void Connection::Dead(Entity const& _e) {
	(void)_e;
}
