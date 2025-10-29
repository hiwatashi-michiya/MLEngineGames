#pragma once
#include "hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
class Joycon {
public:
	void Init();

private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;
};