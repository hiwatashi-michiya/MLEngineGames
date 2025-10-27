#include "Joycon.h"

void Joycon::Init() {
	hidManager_ = std::make_unique<hidManager>();
	hidManager_->Init();

	device_ = hidManager_->Get(JOYCON_L_PRODUCT_ID);
}

void Joycon::CheakInput()
{
    uint8_t data[0x01];

    data[0] = 0x01;
    SendSubcommand(device_, 0x30);

    //            read input report
    uint8_t buff[0x40]; memset(buff, 0x40, size_t(0x40));
    // 読み込むサイズを指定。
    size_t size = 49;
    // buff に input report が入る。
    int ret = hid_read(device_, buff, size);
    printf("\ninput report id: %d\n", *buff);
    // ボタンの押し込みがビットフラグで表現されている。
    printf("input report id: %d\n", buff[5]);
}

bool Joycon::SendSubcommand(hid_device* device, std::byte subcommandId)
{
    uint8_t buf[0x40];
    buf[0] = 1; // 0x10 for rumble only
    buf[10] = subcommandId;

    return hid_write(device, buf, 0x40);
}

