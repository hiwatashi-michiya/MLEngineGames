#pragma once
#include "packet_io.h"
#include "imgui.h"
#include <thread>

#pragma pack(push, 1)
struct Packet {
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;

    uint8_t status[HEIGHT][WIDTH];
    int16_t distance_mm[HEIGHT][WIDTH];
};
#pragma pack(pop)

enum position { pLEFT, pRIGHT, pMID };
class DistanceSensor {
public:
    ~DistanceSensor() {
        End();
	}
    void Init();
    void Update();
    void Draw();

private:

    void Thread();
    void End();
    position CheckPosition();


    std::unique_ptr<PacketIO>packet_io;

    int16_t status[8][8];
	std::string output;
    std::atomic<uint16_t> g_latestValue{ 0 };
    std::thread serialThread;
};