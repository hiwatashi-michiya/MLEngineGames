#include "Joycon.h"
#include<ranges>
#include<algorithm>

void Joycon::Init() {
	hidManager_ = std::make_unique<hidManager>();
	hidManager_->Init();

	device_ = hidManager_->Get(JOYCON_L_PRODUCT_ID);


	std::byte arg;
	arg = std::byte(0x1);
	Joycon::SendSubcommand(device_, std::byte(0x40), { &arg,1 });
	arg = std::byte(0x30);
	Joycon::SendSubcommand(device_, std::byte(0x03), { &arg,1 });

}

void Joycon::Update() {

	// read input report
	std::array<uint8_t, 0x40> buff;
	memset(buff.data(), 0x40, size_t(0x40));
	// 読み込むサイズを指定。
	static constexpr size_t kSize = 49;
	while (true) {
		std::array<uint8_t, 0x40> tmp;
		// buff に input report が入る。
		int ret = hid_read_timeout(device_, tmp.data(), kSize, 1);
		const std::span<uint8_t> mem(tmp.data(), ret);

		if (mem.empty()) {
			break;
		}
		else {
			buff = tmp;
		}
	}

	// ボタンの押し込みがビットフラグで表現されている。
	if (Buttan == false) {
		if (buff[5] & 0x01) {
			OutputDebugStringA("Down");
		}
		if (buff[5] & 0x02) {
			OutputDebugStringA("Up");
		}
		if (buff[5] & 0x04) {
			OutputDebugStringA("Right");
		}
		if (buff[5] & 0x08) {
			OutputDebugStringA("Left");
		}
		Buttan = true;
	}
	static constexpr float kRotCalc = (4588.f / 65535) / 360;

	std::array<int16_t,3> Gyro;


	std::memcpy(Gyro.data(), buff.data() + 19, sizeof(int8_t) * 6);

	//std::transform(Gyro.begin(), Gyro.end(), Gyro_Normalized.begin(), [](int16_t v) {return v; });

	//デッドゾーンの設定
	
	uint16_t cal_gyro_coeff = 590;
	uint16_t cal_gyro_offset = 1;

	float gyro_cal_coeff = (float)(816.0 / (cal_gyro_coeff - (cal_gyro_offset)));

	std::transform(Gyro.begin(), Gyro.end(), Gyro_Normalized.begin(), [&](int16_t v)->float {return v - ((int16_t)cal_gyro_offset * gyro_cal_coeff * 0.0027777778); });

	std::transform(Gyro_Normalized.begin(), Gyro_Normalized.end(), &Vrotate_.x, [](float v) {return v * kRotCalc; });

	ImGui::Begin("Gyro");
	ImGui::Text("GyroX:%f", Vrotate_.x);
	ImGui::Text("GyroY:%f", Vrotate_.y);
	ImGui::Text("GyroZ:%f", Vrotate_.z);

	ImGui::End();

	if (buff[5] == 0) {
		Buttan = false;
	}
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


