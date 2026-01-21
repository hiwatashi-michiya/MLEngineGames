#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include <packet_io.h>
#include "DistanceSensor.h"

std::atomic<bool> is_running(true);

#pragma pack(push, 1)
struct Packet {
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;

    uint8_t status[HEIGHT][WIDTH];
    int16_t distance_mm[HEIGHT][WIDTH];
};
#pragma pack(pop)

void handle_signal(int signal) {
    if (signal == SIGINT) {
        is_running = false;
    }
}

void DistanceSensor::init() {
    const auto ports = PacketIO::find_ports(PacketIO::DeviceType::PICO);
    if (ports.empty()) {
        std::cerr << "Port not found." << std::endl;
        packet_io = nullptr;
    }

    std::cout << "Found " << ports.size() << " ports:" << std::endl;
    for (const auto& port : ports) {
        std::cout << "  " << port.to_string() << std::endl;
    }
    
    auto port = ports[0];
    std::cout << "Connect to:" << std::endl << "  " << port.to_string() << std::endl;
    packet_io = std::make_unique<PacketIO>(port);
    if (!packet_io->open()) {
        std::cerr << "Failed to open" << std::endl;
        packet_io = nullptr;
    }

    serialThread = std::thread(&DistanceSensor::Thread, this);
}

void DistanceSensor::update()
{

   
}

void DistanceSensor::Draw()
{
	ImGui::Begin("Distance Sensor");
	ImGui::Text(output.c_str());
	ImGui::End();
}

void DistanceSensor::Thread() {
    std::signal(SIGINT, handle_signal);
    Packet packet;
    while (is_running) {
        while (packet_io && packet_io->read(&packet, sizeof(packet))) {
			output.clear();
            char line[120];

            for (int y = 0; y < Packet::HEIGHT; y++) {
                int offset = snprintf(line, sizeof(line), "Y=%d: ", y);
                for (int x = 0; x < Packet::WIDTH; x++) {
                    if (packet.status[y][x] == 5 || packet.status[y][x] == 6 ||
                        packet.status[y][x] == 9 || packet.status[y][x] == 10) {
                        offset += snprintf(line + offset, sizeof(line) - offset, "%4d", packet.distance_mm[y][x]);
                    }
                    else {
                        offset += snprintf(line + offset, sizeof(line) - offset, "    ");
                    }
                    offset += snprintf(line + offset, sizeof(line) - offset, "%s", x < Packet::WIDTH - 1 ? ", " : "");
                }
                output += line;
                output += "\n";
            }
            std::cout << output << "--------------------" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

}

void DistanceSensor::End() {
    is_running = false;
    serialThread.join();
}