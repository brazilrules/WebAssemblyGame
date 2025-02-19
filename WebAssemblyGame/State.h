#pragma once

class State
{
public:
	State();
	~State();

	virtual void enter() = 0;
	virtual void update() = 0;
	virtual void exit() = 0;

private:

};

State::State()
{
}

State::~State()
{
}