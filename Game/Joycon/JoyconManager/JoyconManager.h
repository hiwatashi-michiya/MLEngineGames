#pragma once

#include "../Joycon/L/JoyconL.h"
#include "../Joycon/R/JoyconR.h"

class JoyconManager {
public:
	void Init();
	void Update();
	void Draw();
	direction CheakRadius();
private:
	//joycon
	std::unique_ptr<JoyconL> joyconL;
	std::unique_ptr<JoyconR> joyconR;
	direction direction_;
};