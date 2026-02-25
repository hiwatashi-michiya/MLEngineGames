#pragma once
#include <array>
#include <span>
#include <bit>
#include <numbers>
#include "imgui.h"
#include "../../hidapi/hidManager.h"
#include "Externals/hidapi/include/hidapi.h"
#include "Quaternion.h"
#include "../Engine/Core/FrameTracker.h"

using namespace MLEngine::Math;
enum direction {
	front = 0,
	back = 1,
	Left = 2,
	Right = 3,
	no = 5,
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
struct Angle {
	float right = 0.0f;
	float back = 90.0f;
	float left = 180.0f;
	float front = 270.0f;
};
enum JoyconType {
JOYCON_L = 8198,
JOYCON_R = 8199
};
class Joycon {
public:
	void Init(unsigned short Type);
	virtual void addInit() = 0;
	void Update();

	void SetRotate(Vector3 input) { rotate_ = input; };

	Vector3 GetRotate() { return rotate_; };

	bool SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args);

	void ImGui(std::string title);

	virtual direction CheakRadius();


	Vector3 GetVecRotate() { return Vrotate_; };

protected:
	std::unique_ptr<hidManager> hidManager_;
	hid_device* device_;
	// read input report
	static std::array<uint8_t, 0x40> buff;

	std::array<uint16_t, 3> Gyro_Normalized;
	Vector3 Vrotate_;
	Vector3 rotate_;

	direction nowDir;
	direction preDir;
	Vector3 Prerotate_;
	float count;
	const Angle angle;
};