#pragma once
#include <array>
#include <span>
#include <bit>
#include <numbers>
#include "imgui.h"
#include "../hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
#include "Quaternion.h"
#include "../Engine/Core/FrameTracker.h"

using namespace MLEngine::Math;
enum direction {
	front = 0,
	back = 1,
	no = 2,
};
enum Buttan {
	DOWN = 0x01,
	UP = 0x02,
	RIGHT = 0x04,
	LEFT = 0x08,
};
struct GyroData {
	int16_t x;
	int16_t y;
	int16_t z;
};
class Joycon {
public:
	void Init();

	void Update();

	bool IsPush(Buttan key);

	bool SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args);

	direction CheakRadius();
	void ResetRotate() {
		rotate_.x = 90.0f;
	};

	Quaternion GetQuaRotate() { return Qrotate_; };
	Vector3 GetVecRotate() { return Vrotate_; };

private:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;
	bool Buttanflag = false;
	std::array<std::byte, 0x40> data_{};
	// read input report
	static std::array<uint8_t, 0x40> buff;

	Quaternion Qrotate_;
	std::array<uint16_t, 3> Gyro_Normalized;
	Vector3 Vrotate_;
	Vector3 rotate_;
	float second;

	direction nowDir;
	direction preDir;
	Vector3 Prerotate_;
	float count;

};