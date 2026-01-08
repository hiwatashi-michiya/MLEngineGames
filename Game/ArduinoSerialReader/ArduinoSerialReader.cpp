#include"ArduinoSerialReader.h"

void ArduinoSerialReader::Init() {

#pragma region
	hSerial = CreateFile(
		L"\\\\.\\COM4",          // COM番号を合わせる
		GENERIC_READ | GENERIC_WRITE,
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

	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;

	SetCommState(hSerial, &dcb);
#pragma endregion 通信設定

	serialThread = std::thread(&ArduinoSerialReader::SerialReceiveThread, this);
}

void ArduinoSerialReader::Update() {
	value = g_latestValue.load();
}
#ifdef _DEBUG
void ArduinoSerialReader::Draw() {
	ImGui::Begin("Arduino");
	ImGui::Text("Received:%d", value);
	ImGui::End();
}
#endif
void ArduinoSerialReader::SerialReceiveThread() {

#pragma region
	char buf[64];
	DWORD bytesRead;
	std::string line;

	while (g_running) {
		if (ReadFile(hSerial, buf, sizeof(buf), &bytesRead, nullptr) == false) {
			OutputDebugStringA("ReadFile failed\n");
		}
		if (bytesRead == 0) {
			OutputDebugStringA("timeOut\n");
			continue;//タイムアウト
		}
		for (DWORD i = 0; i < bytesRead; i++) {
			if (buf[i] == '\n') {
				//lineが空いていないなら処理をする
				if (!line.empty()) {
					value = static_cast<uint16_t>(std::stoi(line));
					g_latestValue.store(value);
				}
				line.clear();

				//returnから絶対変更するように(別スレッドが理想)
				//return;
			}
			//\r以外を読む
			else if (buf[i] != '\r') {
				line += buf[i];
			}
		}
	}
#pragma endregion 受信
}

void ArduinoSerialReader::End() {
	g_running = false;
	CloseHandle(hSerial);
	serialThread.join();
}