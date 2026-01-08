#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include "imgui.h"

class ArduinoSerialReader {
public:
	~ArduinoSerialReader() {
		End();
	}
	void Init();
	void Update();
#ifdef _DEBUG
	void Draw();
#endif



private:
	void SerialReceiveThread();
	void End();

	HANDLE hSerial;
	uint16_t value;

	std::atomic<bool> g_running{ true };
	std::atomic<uint16_t> g_latestValue{ 0 };
	std::thread serialThread;
};