#include <algorithm>
#include <format>
#include <Windows.h>

#include <packet_io.h>
#include "WirelessLed.h"


bool WirelessLed::Init() {
	const auto ports = PacketIO::find_ports(PacketIO::DeviceType::NONE);
	if (ports.empty()) {
		OutputDebugStringA("[Wireless LED] Not found\n");
		packet_io = nullptr;
		return false;
	}
	
	packet_io = std::make_unique<PacketIO>(ports[0]);
	if (!packet_io->open()) {
		OutputDebugStringA(std::format("[Wireless LED] Not opened {}\n", packet_io->port.name).c_str());
		packet_io = nullptr;
		return false;
	}

	OutputDebugStringA(std::format("[Wireless LED] Opened {}\n", packet_io->port.name).c_str());
	return true;
}

bool WirelessLed::SetLevel(int level) {
	if (!packet_io) return false;

	level = std::clamp(level, MIN_LEVEL, MAX_LEVEL);
	if (level == lastLevel) return true;
	lastLevel = level;

	OutputDebugStringA(std::format("[Wireless LED] SetLevel({})\n", level).c_str());
	return packet_io->writeByte(static_cast<uint8_t>('0') + static_cast<uint8_t>(level));
}
