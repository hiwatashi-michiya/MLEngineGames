#include "hidManager.h"

std::string WStringToString(const std::wstring& wstr, UINT codePage) {
    int sizeNeeded = WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string str(sizeNeeded - 1, 0);
    WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, &str[0], sizeNeeded, nullptr, nullptr);
    return str;
};

void hidManager::Init() {
    // 接続されているHIDデバイスの連結リストを取得。
    device = hid_enumerate(0, 0);
    while (device)
    {
        // プロダクトID等を指定して、HID deviceをopenする。そうすると、そのhidデバイスの情報が載ったhid_deviceが帰ってくる。
        hid_device* dev = hid_open(device->vendor_id, device->product_id, device->serial_number);
        // 今開いているデバイスのプロダクト名の取得。

        std::string name ="\nproduct_id: " + WStringToString(device->product_string);

        OutputDebugStringA(name.c_str());
        // 次のデバイスへ。　　
        device = device->next;
    }

    hid_free_enumeration(device);
}

hid_device_info* hidManager::Get(unsigned short product_id)
{
    return hid_enumerate(0, product_id);
}
