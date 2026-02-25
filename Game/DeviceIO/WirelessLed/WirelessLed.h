#pragma once
#include <packet_io.h>

class WirelessLed {
public:
	bool Init();
	bool SetLevel(int level);

private:
	static constexpr int MIN_LEVEL = 0;
	static constexpr int MAX_LEVEL = 6;

	int lastLevel = -1;
	std::unique_ptr<PacketIO>packet_io;
};
