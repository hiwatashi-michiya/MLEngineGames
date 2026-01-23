#pragma once
#include "packet_io.h"
#include "imgui.h"
#include <thread>
#include <numbers>

#pragma pack(push, 1)
struct Packet {
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;

    uint8_t status[HEIGHT][WIDTH];
    int16_t distance_mm[HEIGHT][WIDTH];
};
#pragma pack(pop)

struct PosData{
    uint8_t data = 0;
    int count = 0;
};

enum position { pLEFT, pRIGHT, pMID,None };
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
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;
    // -1はエラーを表す
    int16_t status[HEIGHT][WIDTH];
	std::string output;
    std::atomic<uint16_t> g_latestValue{ 0 };
    std::thread serialThread;

    static constexpr int DISTANCE_THRESHOLD = 3000;
};