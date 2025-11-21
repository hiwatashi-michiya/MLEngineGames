#pragma once
#include "../hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
#include "Quaternion.h"
using namespace MLEngine::Math;
class Joycon {
public:
	void Init();

	void CheckConeect();

	Quaternion GetRotate() { return rotate_; };
private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;

	Quaternion rotate_;
};