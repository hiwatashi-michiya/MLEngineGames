#pragma once
#include "packet_io.h"
#include "imgui.h"
#include <thread>
class DistanceSensor {
public:
    ~DistanceSensor() {
        End();
	}
    void init();
    void update();
    void Draw();
	void Thread();
	void End();
private:

    std::unique_ptr<PacketIO>packet_io;

    std::string output;
    std::atomic<uint16_t> g_latestValue{ 0 };
    std::thread serialThread;
};