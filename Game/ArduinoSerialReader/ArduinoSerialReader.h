#pragma once

#include <windows.h>
#include <iostream>
#include <string>
#include "imgui.h"

class ArduinoSerialReader {
public:

	void Init();
	void Update();
	void Draw();

private:
	HANDLE hSerial;
	uint16_t value;
};