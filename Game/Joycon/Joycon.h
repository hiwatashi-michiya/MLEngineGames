#pragma once
#include <array>
#include <span>
#include <bit>

#include "../hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
#include "Quaternion.h"
using namespace MLEngine::Math;
class Joycon {
public:
	void Init();

	void Update();

	bool SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args);
	//bool SendSubCommand(hid_device* device,std::byte subcommandID,uint8_t PacetNumber);

	Quaternion GetRotate() { return rotate_; };
private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;

	std::array<std::byte, 0x40> data_{};

	Quaternion rotate_;
};