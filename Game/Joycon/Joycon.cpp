#include "Joycon.h"
#include<ranges>
#include<algorithm>

std::array<uint8_t, 0x40>Joycon::buff;

void Joycon::Init() {
	hidManager_ = std::make_unique<hidManager>();
	hidManager_->Init();

	device_ = hidManager_->Get(JOYCON_L_PRODUCT_ID);
	if (device_ == nullptr) {
		OutputDebugStringA("Joycon is not Conected\n");
		return;
	}
	std::byte arg;
	arg = std::byte(0x1);
	Joycon::SendSubcommand(device_, std::byte(0x40), { &arg,1 });
	arg = std::byte(0x30);
	Joycon::SendSubcommand(device_, std::byte(0x03), { &arg,1 });

}

void Joycon::Update() {

	// 読み込むサイズを指定。
	static constexpr size_t kSize = 49;
	//TODO:ジョイコンがあれば処理をする
	while (device_) {
		std::array<uint8_t, 0x40> tmp;
		// buff に input report が入る。
		int ret = hid_read_timeout(device_, tmp.data(), kSize, 1);
		const std::span<uint8_t> mem(tmp.data(), ret);

		if (mem.empty()) {
			break;
		}
		else {
			// 時間
			int32_t timeDiff = (int32_t(static_cast<int32_t>(0b1'0000'0000 + tmp[1])) - buff[1]) & 0b1111'1111;
			//ImGui::Text("TimeDiff: %s", std::to_string(timeDiff).c_str());

			buff = tmp;
		}
	}

	// ボタンの押し込みがビットフラグで表現されている。
	if (Buttanflag == false) {
		if (buff[5] & Buttan::DOWN) {
			OutputDebugStringA("Down");
		}
		if (buff[5] & Buttan::UP) {
			OutputDebugStringA("Up");
		}
		if (buff[5] & Buttan::RIGHT) {
			OutputDebugStringA("Right");
		}
		if (buff[5] & Buttan::LEFT) {
			OutputDebugStringA("Left");
		}
		Buttanflag = true;
	}
	static constexpr float kRotCalc = (4588.f / 65535) / 360;

	std::array<int16_t, 3> Gyro;

	std::memcpy(Gyro.data(), buff.data() + 19, sizeof(int8_t) * 6);

	//デッドゾーンの設定

	std::array<uint16_t, 3> cal_gyro_offset{ 0x000E, 0xFFDF, 0xFFD0 };
	static constexpr uint16_t cal_gyro_coeff = 13371;

	std::transform(Gyro.begin(), Gyro.end(), cal_gyro_offset.begin(), Gyro_Normalized.begin(), [](int16_t gyro, uint16_t offset) {
		const float gyro_cal_coeff = (float)(936.0f / static_cast<float>(cal_gyro_coeff - std::bit_cast<int16_t>(offset)));
		return (gyro - std::bit_cast<int16_t>(offset)) * (gyro_cal_coeff); });

	Vector3 temp;
	std::transform(Gyro_Normalized.begin(), Gyro_Normalized.end(), &temp.x, [](int16_t v)->float {return v * kRotCalc; });

	if (-0.0002f < temp.x && temp.x < 0.0002f) {
		temp.x = 0.0f;
	}
	if (-0.0004f < temp.y && temp.y < 0.0004f) {
		temp.y = 0.0f;
	}
	if (-0.0006f < temp.z && temp.z < 0.0006f) {
		temp.z = 0.0f;
	}
#ifdef _DEBUG
	ImGui::Begin("Gyro Frame");
	ImGui::Text("GyroX:%f", temp.z);
	ImGui::End();
#endif
	Vrotate_ = temp;
	Vrotate_.y *= -1;
	Vrotate_.x *= -1;
	if (buff[5] == 0) {
		Buttanflag = false;
	}
}
bool Joycon::IsPush(Buttan key)
{
	if (buff[5] & key) {
		return true;
	}
	return false;
}
bool Joycon::SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args)
{
	static uint8_t packetNumber = 0;
	std::array<std::byte, 0x40u> buffer{ std::byte(0) };
	buffer[0] = std::byte(0x01); // Output report ID（固定）

	buffer[1] = std::byte(packetNumber++);	// パケットカウント

	buffer[10] = subcommandId;			// サブコマンドID
	std::copy(args.begin(), args.end(), buffer.begin() + 11); // 引数コピー

	return hid_write(device, std::bit_cast<const uint8_t*>(buffer.data()), buffer.size()) >= 0;
}

direction Joycon::CheakRadius()
{
	//TODO:ジョイコンがなければ抜ける
	if (device_ == nullptr) {
		return no;
	}
#ifdef _DEBUG
	ImGui::Begin("Gyro");
	ImGui::Text("GyroX:%f", test.x);
	ImGui::Text("GyroY:%f", test.y);
	ImGui::Text("GyroZ:%f", test.z);
	ImGui::End();
#endif	
	test += GetVecRotate() * (180 / std::numbers::pi);
	if (std::abs(test.x) <= 180) {
		return front;
	}if (180 <= std::abs(test.x)) {
		return back;
	}

	if (test.x == (std::max)(test.x, 360.0f)) {
		test.x = 0.0f;
	}
	else if (test.x == (std::min)(test.x, 0.0f)) {
		test.x = 360.0f;
	}

}


