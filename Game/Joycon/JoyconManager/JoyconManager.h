#pragma once

#include "../Joycon/L/JoyconL.h"
#include "../Joycon/R/JoyconR.h"

class JoyconManager {
public:
	void Init();
	void Update();
	void Draw();
	direction CheakRadius();
	void ResetRotate(direction dir) {
		if (dir == front) {
			joyconL->ResetRotate(Right);
			joyconR->ResetRotate(front);
		}
		else if (dir == back) {
			joyconL->ResetRotate(Left);
			joyconR->ResetRotate(back);
		};
	};
private:
	//joycon
	std::unique_ptr<JoyconL> joyconL;
	std::unique_ptr<JoyconR> joyconR;
	direction direction_;
};