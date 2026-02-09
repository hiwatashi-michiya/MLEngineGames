#pragma once
#include <serial/serial.h>
#include <string>
#include <vector>

class PacketIO {
public:
    enum class DeviceType : uint32_t {
        ALL = 0x0000'0000,
        PICO = 0x2E8A'F00A,
    };
    struct Port {
        std::string name;
        std::string device_name;
        std::string device_id;
        uint16_t vendor_id = 0;
        uint16_t product_id = 0;
        bool operator==(DeviceType device_type) const;
        bool operator!=(DeviceType device_type) const;
        void parse_device_id();
        std::string to_string() const;
    };
    static std::vector<Port> find_ports(DeviceType device_type);

    PacketIO(Port port);
    bool open();
    void close();
    bool read(void* packet, size_t packet_size);
    const Port port;

private:
    static constexpr uint8_t HEADER[2] = { 0xAA, 0x55 };
    std::unique_ptr<serial::Serial> device;
    std::vector<uint8_t> buffer;
};
