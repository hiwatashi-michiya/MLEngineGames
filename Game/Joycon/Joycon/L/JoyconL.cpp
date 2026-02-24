#include "JoyconL.h"

void JoyconL::addInit()
{
	rotate_.x = angle.right;
	preDir = Right;
}

direction JoyconL::CheakRadius()
{
	//ジョイコンがなければ抜ける
	if (device_ == nullptr) {
		return no;
	}

	rotate_ += GetVecRotate() * (180 / std::numbers::pi) / 2;

	if (360.0f < rotate_.x) {
		rotate_.x = 0.0f;
	}
	else if (rotate_.x < 0.0f) {
		rotate_.x = 360.0f;
	}
	if (angle.left - 25.0f < std::abs(rotate_.x) && std::abs(rotate_.x) < angle.left + 25.0f) {
		preDir = Left;
		return Left;
	}
	else if (std::abs(rotate_.x) < angle.right + 25.0f && std::abs(rotate_.x) < 360.0f - 25.0f) {
		preDir = Right;
		return Right;
	}
	return preDir;
};
