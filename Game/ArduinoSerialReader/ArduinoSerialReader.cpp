#include"ArduinoSerialReader.h"

void ArduinoSerialReader::Init() {

#pragma region
	hSerial = CreateFile(
		L"\\\\.\\COM4",          // COM番号を合わせる
		GENERIC_READ,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (hSerial == INVALID_HANDLE_VALUE) {
		OutputDebugStringA("COM open failed\n");
		return;
	}
#pragma endregion 初期設定

#pragma region
	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(dcb);
	GetCommState(hSerial, &dcb);

	dcb.BaudRate = CBR_115200;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;

	SetCommState(hSerial, &dcb);
	//タイムアウトの設定
	COMMTIMEOUTS timeouts{};
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;

	SetCommTimeouts(hSerial, &timeouts);
#pragma endregion 通信設定

#pragma region
	char buf[64];
	DWORD bytesRead;
	std::string line;

	while (true) {
		ReadFile(hSerial, buf, sizeof(buf), &bytesRead, nullptr);


		break;
		for (DWORD i = 0; i < bytesRead; i++) {
			if (buf[i] == '\n') {
				value = static_cast<uint16_t>(std::stoi(line));
				line.clear();
				break;
			}
			else {
				line += buf[i];
			}

		}
	}
#pragma endregion 受信
}

void ArduinoSerialReader::Update() {

}

void ArduinoSerialReader::Draw() {
	ImGui::Begin("Arduino");
	ImGui::Text("Received:%d", value);
	ImGui::End();
}