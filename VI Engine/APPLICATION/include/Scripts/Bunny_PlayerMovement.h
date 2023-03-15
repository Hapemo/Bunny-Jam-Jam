/*!*****************************************************************************
\file	MoveMinimapScript.h
\author Kew Yu Jun
\par	DP email: k.yujun@digipen.edu
\par	Course: GAM200
\par	Group: Memory Leak Studios
\date	26-11-2022
\brief  This file contains the declaration of the MoveMinimapScript class
		that defines the game logic used to move the minimap
*******************************************************************************/
#pragma once

#include "ECS_tools.h"
#include "ScriptManager.h"
#include "ScriptComponent.h"

/*!*****************************************************************************
\brief MoveMinimapScript class that handles movement of the minimap
*******************************************************************************/
enum BUNNY_PLAYER_DIRECTION
{
	BUNNY_DIRECTION_NONE = 0,
	BUNNY_DIRECTION_UP,
	BUNNY_DIRECTION_DOWN,
	BUNNY_DIRECTION_LEFT,
	BUNNY_DIRECTION_RIGHT 
};

class Bunny_PlayerMovement : public ScriptComponent {
private:
	std::string name{ "Bunny_PlayerMovement" };
	Scene* currScene;
	Transform* currCamera;
	BUNNY_PLAYER_DIRECTION sPLAYERDIRECTION = BUNNY_PLAYER_DIRECTION::BUNNY_DIRECTION_NONE;
	unsigned int Carrots = 0;
	bool inited;
	Math::Vec2 originalTranslate;
	Math::Vec2 originalScale;

public:
	/*!*****************************************************************************
	\brief
	Default constructor
	*******************************************************************************/
	Bunny_PlayerMovement() = default;
	/*!*****************************************************************************
	\brief
	Default destructor.
	*******************************************************************************/
	~Bunny_PlayerMovement() override = default;

	/*!*****************************************************************************
	\brief
	Function will run when the gamestate of the entity is activated.
	*******************************************************************************/
	void Alive(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run on script start
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void Init(const Entity& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run during each update frame while the entity
	is active.
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void EarlyUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run during each update frame while the entity
	is active after running all EarlyUpdate functions from all other active scripts.
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void Update(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run on fixed delta time after running all
	EarlyUpdate and Update functions from all other active scripts.
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void FixedUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run while the entity is active after running all
	EarlyUpdate, Update, and FixedUpdate functions from all other active scripts.
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void LateUpdate(Entity const& _e) override;

	/*!*****************************************************************************
	\brief
	Function that contains logic to run on script end
	\param const Entity &
	A reference to read-only entity to update
	\return void
	NULL
	*******************************************************************************/
	void Exit(const Entity& _e) override;

	/*!*****************************************************************************
	\brief
	Function will run when the gamestate of the entity exits.
	*******************************************************************************/
	void Dead(Entity const& _e) override;
};