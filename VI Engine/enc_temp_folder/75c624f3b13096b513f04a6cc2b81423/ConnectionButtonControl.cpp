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


#include "ConnectionButtonControl.h"

REGISTER_SCRIPT(ScriptComponent, ConnectionButtonControl);

namespace {
	Entity join_; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
	Entity ip_;

	bool textin_ = false;
	int iplen_ = 0;

	//Scene textinput_;
	std::string ipstring_ = "";
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void ConnectionButtonControl::Alive(Entity const& _e) {

	ip_ = VI::iEntity::GetEntity("IP", "RequestIP");
	join_ = VI::iEntity::GetEntity("Join", "");
	if (join_.id == 0) return;

	/*textinput_ =*/ 

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void ConnectionButtonControl::Init(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active.
*******************************************************************************/
void ConnectionButtonControl::EarlyUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void ConnectionButtonControl::Update(Entity const& _e) {
	//(void)_e;

	//if (join_.GetComponent<Transform>().scale.x < 100 && enlarge == false) {
	//	enlarge = true;
	//}
	//if (join_.GetComponent<Transform>().scale.x > 200 && enlarge == true) {
	//	enlarge = false;
	//}

	//if (enlarge == true) {
	//	join_.GetComponent<Transform>().scale.x += 100.f* (float)FUNC->GetDeltaTime();
	//}
	//else {
	//	join_.GetComponent<Transform>().scale.x -= 100.f * (float)FUNC->GetDeltaTime();
	//}

	//_e.GetComponent<Text>().text = "sgdsdadas"; 

	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE)) {
		iplen_ = 0;
		ipstring_ = "";
		textin_ = false;
	}

	if (textin_ == true&& iplen_<16) {
		for (int i = 48, j = 1; i < 58; ++i, ++j) {

			if (j > 9) {
				j = 0;
			}
			if (VI::iInput::CheckKey(E_STATE::PRESS, static_cast<E_KEY>(i)) == true) {

				++iplen_;
				ipstring_ += std::to_string(j);
				ip_.GetComponent<Text>().text = ipstring_;
			}
		}
	}


	if (join_.GetComponent<Button>().isHover) {
		//if (join_.GetComponent<Transform>().scale.x < 200) {
		//	join_.GetComponent<Transform>().scale.x += 1000.f * (float)FUNC->GetDeltaTime();
		//}

		if (join_.GetComponent<Button>().isClick) {
			textin_ = true;
			VI::iScene::Play("RequestIP");
		}
	}
	else {
		//if (join_.GetComponent<Transform>().scale.x > 100) {
		//	join_.GetComponent<Transform>().scale.x -= 1000.f * (float)FUNC->GetDeltaTime();
		//}
	}
}

/*!*****************************************************************************
\brief
Function will run on fixed delta time after running all EarlyUpdate and Update
functions from all other active scripts.
*******************************************************************************/
void ConnectionButtonControl::FixedUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
*******************************************************************************/
void ConnectionButtonControl::LateUpdate(Entity const& _e) {
	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on exit or when the entity is destroyed.
*******************************************************************************/
void ConnectionButtonControl::Exit(Entity const& _e) {
	(void)_e;
	//LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void ConnectionButtonControl::Dead(Entity const& _e) {
	(void)_e;
}
