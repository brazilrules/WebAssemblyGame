#pragma once
#include "SDL.h"
#include "State.h"

class StartGameState:State
{
public:
	StartGameState();
	~StartGameState();

	// Inherited via State
	void enter() override;

	void update() override;

	void exit() override;

private:
	
};