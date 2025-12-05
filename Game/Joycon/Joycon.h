#pragma once
#include <array>
#include <span>
#include <bit>
#include <numbers>
#include "imgui.h"

#include "../hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
#include "Quaternion.h"
using namespace MLEngine::Math;
struct GyroData {
	int16_t x;
	int16_t y;
	int16_t z;
};
class Joycon {
public:
	void Init();

	void Update();

	bool SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args);

	Quaternion GetQuaRotate() { return Qrotate_; };
	Vector3 GetVecRotate() { return Vrotate_; };

private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;
	bool Buttan = false;
	std::array<std::byte, 0x40> data_{};

	Quaternion Qrotate_;
	std::array<uint16_t, 3> Gyro_Normalized;
	Vector3 Vrotate_;
};