#pragma once
#include "../Joycon.h"

class JoyconL : public Joycon
{
public:
	void addInit()override;
	direction CheakRadius()override;
	void ResetRotate(direction dir) {
		if (dir == Left) {
			rotate_.x = 180.0f;
		}
		else if (dir == Right) {
			rotate_.x = 0.0f;
		}
	};
private:


};