#include "JoyconR.h"

void JoyconR::addInit()
{
	rotate_.x = angle.front;
	preDir = front;
}

direction JoyconR::CheakRadius()
{
	//ジョイコンがなければ抜ける
	if (Joycon::device_ == nullptr) {
		return no;
	}

	rotate_ += GetVecRotate() * (180 / std::numbers::pi) / 2;

	if (360.0f < rotate_.x) {
		rotate_.x = 0.0f;
	}
	else if (rotate_.x < 0.0f) {
		rotate_.x = 360.0f;
	}
	if (preDir == front) {
		if (angle.back - 45.0f < std::abs(rotate_.x) && std::abs(rotate_.x) < angle.back + 45.0f) {
			preDir = back;
			return back;
		}
	}
	else if (preDir == back) {
		if (angle.front - 45.0f < std::abs(rotate_.x) && std::abs(rotate_.x) < angle.front + 45.0f ) {
			preDir = front;
			return front;
		}
	}
	return preDir;
}
