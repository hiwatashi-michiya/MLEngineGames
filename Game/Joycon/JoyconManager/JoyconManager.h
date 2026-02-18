#pragma once

#include "../Joycon/Joycon.h"

class JoyconManager {
public:
	void Init();
	void Update();
	void Draw();
private:
	//joycon
	std::unique_ptr<Joycon> joyconL;
	std::unique_ptr<Joycon> joyconR;
};