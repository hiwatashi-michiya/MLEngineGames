#include "Joycon.h"

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
		// buff に input report が入る。
		int ret = hid_read_timeout(device_, buff.data(), kSize, 1);
		const std::span<uint8_t> mem(buff.data(), ret);

		if (mem.empty()) {
			break;
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
	static constexpr float kRotCalc = 4588.f / 65535;
	std::array<int16_t,1> GyroX;
	std::array<int16_t,1> GyroY;
	std::array<int16_t,1> GyroZ;
	for () {
		
		ImGui::Begin("Joycon Bit");
		ImGui::Text("%d", buff.begin());
		ImGui::End();
	}

	std::memcpy(GyroX.data(), buff.data() + 18, sizeof(int8_t) * 2);
	std::memcpy(GyroY.data(), buff.data() + 22, sizeof(int8_t) * 2);
	std::memcpy(GyroZ.data(), buff.data() + 24, sizeof(int8_t) * 2);
	ImGui::Begin("Gyro");
	ImGui::Text("GyroX:%d", GyroX.data());
	ImGui::Text("GyroY:%d", GyroY.data());
	ImGui::Text("GyroZ:%d", GyroZ.data());

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

//bool Joycon::SendSubCommand(hid_device* device, std::byte subcommandID, uint8_t PacetNumber)
//{
//
//
//	uint8_t buf[0x40]; bzero(buf, 0x40);
//
//	//buf[0] = 1; // 0x10 for rumble only
//	//buf[1] = GlobalPacketNumber; // Increment by 1 for each packet sent. It loops in 0x0 - 0xF range.
//	//memcpy(buf + 2, rumbleData, 8);
//	//buf[10] = subcommandID;
//	//memcpy(buf + 11, subcommandData, subcommandDataLen);
//
//
//	return hid_write(handle, buf, 0x40);
//}



