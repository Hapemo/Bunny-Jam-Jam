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
static bool updateOnce{ false };

REGISTER_SCRIPT(ScriptComponent, ConnectionButtonControl);

namespace {
	Entity join_; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
	Entity ip_;
	Entity bgeff_;
	Entity bgeff2_;

	bool posap_ = false;

	bool textin_ = false;
	int iplen_ = 0;


	bool player1_ = false;
	bool player2_ = false;
	



	//Scene textinput_;
	std::string ipstring_ = "";
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void ConnectionButtonControl::Alive(Entity const& _e) {





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
	if (!updateOnce)
	{
		bgeff_ = VI::iEntity::GetEntity("BGEffect", "");
		bgeff2_ = VI::iEntity::GetEntity("BGEffect2", "");
		ip_ = VI::iEntity::GetEntity("IP", "RequestIP");
		join_ = VI::iEntity::GetEntity("Join", "");
	}

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on every update while the entity is active after running all
EarlyUpdate functions from all other active scripts.
*******************************************************************************/
void ConnectionButtonControl::Update(Entity const& _e) {
	//(void)_e;


	//2764 position 

	if (bgeff_.GetComponent<Transform>().translation.x >= -500 && posap_ == false) {
		bgeff2_.GetComponent<Transform>().translation.x = -2764;
		bgeff2_.GetComponent<Transform>().translation.y = 2764;
		posap_ = true;
	}
	if (bgeff2_.GetComponent<Transform>().translation.x >= -500 && posap_ == true) {
		bgeff_.GetComponent<Transform>().translation.x = -2764;
		bgeff_.GetComponent<Transform>().translation.y = 2764;
		posap_ = false;
	}

	bgeff_.GetComponent<Transform>().translation.x += 100.f * (float)FUNC->GetDeltaTime();
	bgeff_.GetComponent<Transform>().translation.y -= 100.f * (float)FUNC->GetDeltaTime();

	bgeff2_.GetComponent<Transform>().translation.x += 100.f * (float)FUNC->GetDeltaTime();
	bgeff2_.GetComponent<Transform>().translation.y -= 100.f * (float)FUNC->GetDeltaTime();

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
		ip_.GetComponent<Text>().text = ipstring_;
		VI::iScene::Pause("RequestIP");
		textin_ = false;

	}

	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::L)) {

	}

	if (textin_ == true&& iplen_<16) {
		for (int i = 48, j = 0; i < 59; ++i, ++j) {

			if (j > 9) {
				j = 0;
			}
			if (VI::iInput::CheckKey(E_STATE::PRESS, static_cast<E_KEY>(i)) == true) {

				++iplen_;
				ipstring_ += std::to_string(j);
				ip_.GetComponent<Text>().text = ipstring_;
			}
		}

		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::PERIOD) == true) {
			++iplen_;
			ipstring_ += ".";
			ip_.GetComponent<Text>().text = ipstring_;
		}
	}

	if (iplen_ > 0) {
		if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::BACKSPACE) == true) {
			--iplen_;
			ipstring_.pop_back();
			ip_.GetComponent<Text>().text = ipstring_;

		}
	}

	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ENTER) == true) {
		// submit the ip address
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
