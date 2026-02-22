#include "JoyconL.h"

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

	if (std::abs(rotate_.x) >= 180.0f) {
		preDir = Left;
		return Left;
	}
	else {
		preDir = Right;
		return Right;
	}
};
