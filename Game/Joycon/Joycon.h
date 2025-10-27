#pragma once
#include "hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
class Joycon {
public:
	void Init();
	void CheakInput();
	bool SendSubcommand(hid_device* device, std::byte subcommandId);

private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;
};