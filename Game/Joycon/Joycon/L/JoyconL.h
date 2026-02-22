#pragma once
#include "../Joycon.h"

class JoyconL : public Joycon
{
public:
	void addInit()override;
	direction CheakRadius()override;
	void ResetRotate(direction dir) {
		if (dir == front) {
			rotate_.x = 90.0f;
		}
		else if (dir == back) {
			rotate_.x = 270.0f;
		}
	};
private:


};