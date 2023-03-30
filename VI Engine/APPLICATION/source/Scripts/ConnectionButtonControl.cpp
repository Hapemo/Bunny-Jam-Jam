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
#include "Menu_Button.h"
#include "../../../VI/include/ServerManager.h" 
#include "../../../VI/include/ClientManager.h" 
#include "NetworkSerialization.h"


static bool updateOnce{ false };
//bool ConnectionButtonControl::startMenu_{ false };
REGISTER_SCRIPT(ScriptComponent, ConnectionButtonControl);

namespace {
    Entity join_; // THIS SHOULD REMOVE WHEN REAL PARTICLE SYSTEM IS IMPLEMENTED
    Entity ip_;
    Entity bgeff_;
    Entity bgeff2_;
    Entity loadicon_;
    Entity back_;
    Entity p1_;
    Entity p2_;
    Entity you_;

    // menu effect variables
    bool posap_ = false;

    bool player1_ = false;
    bool player2_ = false;
    bool isConnected{ false };
    bool ifFound{ false };
    bool bothPlayers{ false };
    // scene transition variables
    float acc_ = 300;
    float scaling_ = 1000;
    int zoom_ = 0;
    int* numClients{};

    // text input variables
    std::string ipstring_ = "";
    bool textin_ = false;
    int iplen_ = 0;

    //int counter = 0;
}


int ConnectionButtonControl::Transit(int zoom, Entity loadicon, float& acc, float& scaling) {


    if (zoom == 1) {
        loadicon.GetComponent<Transform>().scale.x += scaling * (float)FUNC->GetDeltaTime();
        loadicon.GetComponent<Transform>().scale.y += scaling * (float)FUNC->GetDeltaTime();
        if (scaling < 5000) {
            acc += 2000.f * (float)FUNC->GetDeltaTime();
            scaling += acc * (float)FUNC->GetDeltaTime() * 60;
        }
        //  counter++;
        //  std::cout << counter << " counter for menu_button \n";

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
        return -1;
    }

    return zoom;
}



/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity is activated.
*******************************************************************************/
void ConnectionButtonControl::Alive(Entity const& _e) {

    (void)_e;
}

/*!*****************************************************************************
\brief
Function will run on initialisation of the entity.
*******************************************************************************/
void ConnectionButtonControl::Init(Entity const& _e) {
    // acc_ = 1;
    // scaling_ = 1000;
    you_ = VI::iEntity::GetEntity("You", "Game");
    loadicon_ = VI::iEntity::GetEntity("BunnyLoad", "Game");
    back_ = VI::iEntity::GetEntity("Back", "Game");
    p1_ = VI::iEntity::GetEntity("Player1", "Game");
    p2_ = VI::iEntity::GetEntity("Player2", "Game");

    //if (zoom_ == 1) {

    //  loadicon_.GetComponent<Transform>().scale.x =0.f;
    //  loadicon_.GetComponent<Transform>().scale.y =0.f;
    //}
    //else if (zoom_ == 0) {
    zoom_ = 0;
    you_.GetComponent<Transform>().scale.x = 0.f;
    you_.GetComponent<Transform>().scale.y = 0.f;
    p1_.GetComponent<Transform>().scale.x = 0.f;
    p1_.GetComponent<Transform>().scale.y = 0.f;
    p2_.GetComponent<Transform>().scale.x = 0.f;
    p2_.GetComponent<Transform>().scale.y = 0.f;
    loadicon_.GetComponent<Transform>().scale.x = 2000.f;
    loadicon_.GetComponent<Transform>().scale.y = 2000.f;
    //}
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

    if (!updateOnce)
    {
        you_ = VI::iEntity::GetEntity("You", "Game");
        p1_ = VI::iEntity::GetEntity("Player1", "Game");
        p2_ = VI::iEntity::GetEntity("Player2", "Game");
        loadicon_ = VI::iEntity::GetEntity("BunnyLoad", "Game");
        back_ = VI::iEntity::GetEntity("Back", "Game");
        bgeff_ = VI::iEntity::GetEntity("BGEffect", "Game");
        bgeff2_ = VI::iEntity::GetEntity("BGEffect2", "Game");
        ip_ = VI::iEntity::GetEntity("IP", "RequestIP");
        join_ = VI::iEntity::GetEntity("Join", "");
    }


    // default p1 pos -400 / 8
    // default p2 pos -400 / 220
    // default you_ size 335 / 150
    // default player1/2 button size 260 / 180




    if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::Q) == true) {
        player1_ = true;
    }
    if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::E) == true) {
        player2_ = true;
    }


    if (player1_ == true) {
        you_.GetComponent<Transform>().scale.x = 335.f;
        you_.GetComponent<Transform>().scale.y = 150.f;
        p1_.GetComponent<Transform>().scale.x = 260.f;
        p1_.GetComponent<Transform>().scale.y = 180.f;
    }
    if (player2_ == true) {
        you_.GetComponent<Transform>().scale.x = 335.f;
        you_.GetComponent<Transform>().scale.y = 150.f;
        p2_.GetComponent<Transform>().scale.x = 260.f;
        p2_.GetComponent<Transform>().scale.y = 180.f;
    }

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


    if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE))
    {
        iplen_ = 0;
        ipstring_ = "";
        ip_.GetComponent<Text>().text = ipstring_;
        VI::iScene::Pause("RequestIP");
        textin_ = false;
    }
    

    if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::L)) {

    }

    if (textin_ == true && iplen_ < 16) {
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



        //Connect Address
        if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ENTER))
        {
            if (!isConnected)
            {

                if (ClientManager::GetInstance()->clientInit(ipstring_, 5051, "gay"))
                {
                    //Connected Successfully
                    VI::iScene::Pause("RequestIP");
                    isConnected = true;
                }
                else
                {
                    //Connection failed
                    VI::iScene::Play("wrongipScene");
                    VI::iScene::Pause("RequestIP");
                    //Pop up error Message
                }
            }
        }
    
    //Out of wrong ip scene
    if (!VI::iScene::IsPaused("wrongipScene")) 
    {
        if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::ESCAPE)) { VI::iScene::Pause("wrongipScene"); }
    }

    if (NetworkSerializationManager::GetInstance()->mPlayerID == 1)
    {
        player1_ = true;
        if (!bothPlayers)
        {
            bothPlayers = true;
            you_.GetComponent<Transform>().translation.x = -400.f;
            you_.GetComponent<Transform>().translation.y = 220.f;
        }
    }
    else if (NetworkSerializationManager::GetInstance()->mPlayerID == 2)
    {
        player2_ = true;
        if (!bothPlayers)
        {
            bothPlayers = true;
            you_.GetComponent<Transform>().translation.x = -400.f;
            you_.GetComponent<Transform>().translation.y = 8.f;
        }
    }


    if (NetworkSerializationManager::GetInstance()->mNumberOfClientConnected == 2)
    {
        player1_ = true;
        player2_ = true;
    }

    if (zoom_ == -1 && loadicon_.GetComponent<Transform>().scale.x >= 2000) {
        VI::iGameState::ChangeGameState("Bunny_MainMenu");
    }


    if (VI::iInput::CheckKey(E_STATE::PRESS, E_KEY::SPACE) == true)
    {
        VI::iGameState::ChangeGameState("Bunny_GameLevel1");
    }


    if (NetworkSerializationManager::GetInstance()->mGameStarted) {
        VI::iGameState::ChangeGameState("Bunny_GameLevel1");
    }


    //------------------------------------------------------------------------// Buttons


    if (back_.GetComponent<Button>().isHover) {
        //if (join_.GetComponent<Transform>().scale.x < 200) {
        //  join_.GetComponent<Transform>().scale.x += 1000.f * (float)FUNC->GetDeltaTime();
        //}
        if (back_.GetComponent<Button>().isClick) {
            zoom_ = 1;
            Menu_Button::startMenu_ = 0;
        }
    }
    zoom_ = Transit(zoom_, loadicon_, acc_, scaling_);

    if (join_.GetComponent<Button>().isHover) {
        //if (join_.GetComponent<Transform>().scale.x < 200) {
        //  join_.GetComponent<Transform>().scale.x += 1000.f * (float)FUNC->GetDeltaTime();
        //}
        if (join_.GetComponent<Button>().isClick) {
            textin_ = true;
            if(!isConnected)
            VI::iScene::Play("RequestIP");
        }
    }


    // get game started timing
    NetworkSerializationManager* manager = NetworkSerializationManager::GetInstance();


    if (manager->mGameCountDown > 0) {
      manager->mGameCountDown -= static_cast<float>(FPSManager::dt);
      if (manager->mGameCountDown <= 0) VI::iGameState::ChangeGameState("Bunny_GameLevel1");
    }

    (void)_e;
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

    acc_ = 300;
    scaling_ = 1000;
    (void)_e;
    //LOG_INFO("How to Play button script end works!!!");
}

/*!*****************************************************************************
\brief
Function will run when the gamestate of the entity exits.
*******************************************************************************/
void ConnectionButtonControl::Dead(Entity const& _e) {
    loadicon_.GetComponent<Transform>().scale.x = 0;
    loadicon_.GetComponent<Transform>().scale.y = 0;
    zoom_ = true;
    acc_ = 1;
    scaling_ = 1000;
    (void)_e;
}
