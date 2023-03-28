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
#include "NetworkSerialization.h"
static bool updateOne{ false };
int Menu_Button::startMenu_{ -1 };
REGISTER_SCRIPT(ScriptComponent, Menu_Button);

namespace {
	Entity playButton; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
	Entity h2pButton;
	Entity toObjectives;
	Entity toControls;
	Entity loadicon_;


	float acc_ = 300;
	float scaling_ = 1000;


	bool firstLoad_ = true;

	int clickCount{};
	bool atControls{ false };

	int counter = 0;
}


int Menu_Button::Transit(int zoom, Entity loadicon, float& acc, float& scaling) {


	if (zoom == 1) {
		
		loadicon.GetComponent<Transform>().scale.x += scaling * (float)FUNC->GetDeltaTime();
		loadicon.GetComponent<Transform>().scale.y += scaling * (float)FUNC->GetDeltaTime();
		if (scaling < 3000) {
			acc += 2000.f * (float)FUNC->GetDeltaTime();
			scaling += acc * (float)FUNC->GetDeltaTime() * 60;
		}


	//	counter++;
		//std::cout << counter << " counter for menu_button \n";


	}

	else if (zoom == 0 && loadicon.GetComponent<Transform>().scale.x >= 0) {

			


		loadicon.GetComponent<Transform>().scale.x -= scaling * (float)FUNC->GetDeltaTime();
		loadicon.GetComponent<Transform>().scale.y -= scaling * (float)FUNC->GetDeltaTime();


		if (scaling < 3000) {
			acc += 2000.f * (float)FUNC->GetDeltaTime();
			scaling += acc * (float)FUNC->GetDeltaTime() * 60;
		}
	}

	//std::cout << acc_ << " acceleration \n";
	//std::cout << scaling_ << " scaling \n";
	//std::cout << loadicon_.GetComponent<Transform>().scale.x << " scale \n";

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
		return -1;
	}

	return zoom;
}


/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void Menu_Button::Alive(Entity const& _e) {

	(void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void Menu_Button::Init(Entity const& _e) {


	loadicon_ = VI::iEntity::GetEntity("BunnyLoadM", "");


	if (startMenu_ == 1) {

		loadicon_.GetComponent<Transform>().scale.x = 0.f;
		loadicon_.GetComponent<Transform>().scale.y = 0.f;
	}
	else if (startMenu_ == 0) {

		loadicon_.GetComponent<Transform>().scale.x = 2000.f;
		loadicon_.GetComponent<Transform>().scale.y = 2000.f;
	}
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
	if (!updateOne)
	{
		playButton = VI::iEntity::GetEntity("Play_Button", "");
		loadicon_ = VI::iEntity::GetEntity("BunnyLoadM", "");
		h2pButton = VI::iEntity::GetEntity("H2P_Button", "");
		updateOne = true;

	}


	playButton = VI::iEntity::GetEntity("Play_Button", "");
	loadicon_ = VI::iEntity::GetEntity("BunnyLoadM", "");
	h2pButton = VI::iEntity::GetEntity("H2P_Button", "");



	//If PlayButton is clicked
	if (playButton.GetComponent<Button>().isHover) {

		if (playButton.GetComponent<Button>().isClick) {
			//VI::iScene::Play("Game");
			//VI::iScene::Pause("Bunny_Menu");

			
			startMenu_ = 1;
		//	if (startMenu_ == 1) {
			loadicon_.GetComponent<Transform>().scale.x = 0.f;
			loadicon_.GetComponent<Transform>().scale.y = 0.f;

			//std::cout << "lMAMEEEEEEEEEEEEEEEE\n";

		//	}
			/*else if (startMenu_ == 0) {

				loadicon_.GetComponent<Transform>().scale.x = 2000.f;
				loadicon_.GetComponent<Transform>().scale.y = 2000.f;
			}*/

		}
	}


	//Main Menu -> Controls
	if (h2pButton.GetComponent<Button>().isHover) 
	{
		if (h2pButton.GetComponent<Button>().isClick) 
		{
			VI::iScene::Play("How2PlayControls");
			VI::iScene::Pause("Bunny_Menu");
		}
	}

	if (!VI::iScene::IsPaused("How2PlayControls")) 
	{
		//toControls.GetComponent<Button>().isClick = false;
	}


	//Back to main menu
	if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE))
	{

		//Back to menu
		if (VI::iScene::IsPaused("Bunny_Menu"))
		{
			VI::iScene::Pause("How2PlayObjectives");
			VI::iScene::Pause("How2PlayControls");
			VI::iScene::Play("Bunny_Menu");
		}
	}


	if (startMenu_ ==-1 && loadicon_.GetComponent<Transform>().scale.x >=2000) {
		VI::iGameState::ChangeGameState("Connection_Menu");
	}
	startMenu_ = Transit(startMenu_, loadicon_, acc_, scaling_);
	
	NetworkSerializationManager* manager = NetworkSerializationManager::GetInstance();

	if (manager->mGameStarted) {
		manager->mGameCountDown -= static_cast<float>(FPSManager::dt);
		manager->SerialiseAndSend(NetworkSerializationManager::NETWORKDATATYPE::S2CGameStarted);
		
		if (manager->mGameCountDown <= 0) VI::iGameState::ChangeGameState("Bunny_GameLevel1");
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
