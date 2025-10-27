#pragma once
#define JOYCON_L_PRODUCT_ID 8198
#define JOYCON_R_PRODUCT_ID 8199
#include <memory>
#include <string>
#include <Windows.h>
#include "Externals/hidapi/include/hidapi.h"

std::string WStringToString(const std::wstring& wstr, UINT codePage = CP_UTF8);

class hidManager {
public:
	void Init();
	hid_device* Get(unsigned short product_id);
private:
	hid_device_info* device;
};