#include "Joycon.h"

void Joycon::Init() {
	hidManager_ = std::make_unique<hidManager>();
	hidManager_->Init();

	device_ = hidManager_->Get(JOYCON_L_PRODUCT_ID);

	std::byte arg;
	arg = std::byte(0x1);
	Joycon::SendSubcommand(device_, std::byte(0x40u), { &arg,1 });
}

void Joycon::Update() {
	//            read input report
	uint8_t buff[0x40]; memset(buff, 0x40, size_t(0x40));
	// 読み込むサイズを指定。
	size_t size = 49;
	// buff に input report が入る。
	int ret = hid_read(device_, buff, size);
	printf("\ninput report id: %d\n", *buff);
	// ボタンの押し込みがビットフラグで表現されている。
	if (buff[0x05] != 0) {
		OutputDebugStringA("A");
	}
	printf("input report id: %d\n", buff[5]);
}

bool Joycon::SendSubcommand(hid_device* device, std::byte subcommandId, const std::span<std::byte>& args)
{
	static uint8_t packetNumber = 0;
	std::array<std::byte, 0x40u> buffer{ std::byte(0) };
	buffer[0] = std::byte(0x01); // Output report ID（固定）

	buffer[1] = std::byte(packetNumber++);	// パケットカウント

	// 振動データ(8バイト)を全て0に（今回は無効化）
	std::fill(buffer.begin() + 2, buffer.begin() + 10, std::byte(0x00));

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



