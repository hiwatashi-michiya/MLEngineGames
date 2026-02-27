#include <iostream>
#include <regex>

#include <packet_io.h>

bool PacketIO::Port::operator==(DeviceType device_type) const {
    if (device_type == DeviceType::ALL) return true;
    const uint32_t vid_pid = static_cast<uint32_t>(this->vendor_id) << 16 | this->product_id;
    return vid_pid == static_cast<uint32_t>(device_type);
}

bool PacketIO::Port::operator!=(DeviceType device_type) const {
    return !(*this == device_type);
}

void PacketIO::Port::parse_device_id() {
    // Windows: USB\VID_2E8A&PID_F00A&REV_0100&MI_00
    // macOS: USB VID:PID=2e8a:f00a SNR=XXXXXXXXXXXXXXXX
    static const std::regex pattern(R"(VID(?:_|:PID=)([0-9A-Fa-f]{4})(?:&PID_|:)([0-9A-Fa-f]{4}))", std::regex::icase);

    std::smatch match;
    if (std::regex_search(this->device_id, match, pattern)) {
        this->vendor_id = static_cast<uint16_t>(std::stoi(match[1].str(), nullptr, 16));
        this->product_id = static_cast<uint16_t>(std::stoi(match[2].str(), nullptr, 16));
    } else {
        this->vendor_id = this->product_id = 0;
    }
}

std::string PacketIO::Port::to_string() const {
    char buf[256];
    snprintf(buf, sizeof(buf), "Port(name=\"%s\", device_name=\"%s\", vendor_id=0x%04X, product_id=0x%04X)",
        this->name.c_str(), this->device_name.c_str(), this->vendor_id, this->product_id);
    return std::string(buf);
}

std::vector<PacketIO::Port> PacketIO::find_ports(DeviceType device_type) {
    std::vector<Port> ports;
    for (const auto& serial_port : serial::list_ports()) {
        Port port = { serial_port.port, serial_port.description, serial_port.hardware_id };
        port.parse_device_id();
        if (port == device_type) {
            ports.push_back(port);
        }
    }
    std::sort(ports.begin(), ports.end(),
        [](const Port& a, const Port& b) {
            return a.name < b.name;
        }
    );
    return ports;
}

PacketIO::PacketIO(Port port) : port(std::move(port)) {
    this->buffer.reserve(1024);
}

bool PacketIO::open() {
    try {
        this->device = std::make_unique<serial::Serial>(this->port.name, 115200);
    } catch (const std::exception& e) {
        std::cerr << "PacketIO.open(): " << e.what() << std::endl;
        return false;
    }
    if (!this->device->isOpen()) {
        this->device.reset();
        return false;
    }
    this->device->setDTR(true);
    this->device->setRTS(true);
    return true;
}

void PacketIO::close() {
    this->device.reset();
}

bool PacketIO::read(void* packet, size_t packet_size) {
    if (!this->device) return false;

    const size_t chunk_size = this->device->available();
    if (chunk_size) {
        const size_t buffer_size = this->buffer.size();
        this->buffer.resize(buffer_size + chunk_size);
        this->device->read(this->buffer.data() + buffer_size, chunk_size);
    }

    const size_t total_size = sizeof(this->HEADER) + packet_size;
    while (this->buffer.size() >= total_size) {
        if (std::memcmp(this->buffer.data(), this->HEADER, sizeof(this->HEADER)) == 0) {
            std::memcpy(packet, this->buffer.data() + sizeof(this->HEADER), packet_size);
            this->buffer.erase(this->buffer.begin(), this->buffer.begin() + total_size);
            return true;
        } else {
            this->buffer.erase(this->buffer.begin());
        }
    }
    return false;
}

bool PacketIO::writeByte(uint8_t value) {
    if (!this->device) return false;

    return this->device->write(&value, 1) == 1;
}

constexpr uint8_t PacketIO::HEADER[];
