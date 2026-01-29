#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

#include <packet_io.h>
#include "DistanceSensor.h"

std::atomic<bool> is_running(true);

void handle_signal(int signal) {
	if (signal == SIGINT) {
		is_running = false;
	}
}

void DistanceSensor::Init() {
	const auto ports = PacketIO::find_ports(PacketIO::DeviceType::PICO);
	if (ports.empty()) {
		std::cerr << "Port not found." << std::endl;
		packet_io = nullptr;
		return;
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

void DistanceSensor::Update()
{


}

void DistanceSensor::Draw()
{
	ImGui::Begin("Distance Sensor");
	ImGui::Text(output.c_str());
	ImGui::End();
	CheckPosition();
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
						status[y][x] = packet.distance_mm[y][x];
					}
					else {
						offset += snprintf(line + offset, sizeof(line) - offset, "    ");
						status[y][x] = -1;
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

position DistanceSensor::CheckPosition()
{
	//intの最大値を代入
	// x軸はセンサーから見た位置になっている
	PosData minLeft, minMid, minRight;


	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			//情報が取れていない場合はスキップする
			if (status[y][x] == -1) { 
				continue;
			}
			// プレイヤーから見た左右にしている
			if (x < 1) {
				minRight.data +=  status[y][x];
				minRight.count++;
			}
			else if (x < 7) {
				minMid.data += status[y][x];
				minMid.count++;
			}
			else if (x < 8) {
				minLeft.data += status[y][x];
				minLeft.count++;

			}
		}
	}
	if (minLeft.count != 0) {
		minLeft.data /= minLeft.count;
	}
	if (minMid.count != 0) {
		minMid.data /= minMid.count;
	}
	if (minRight.count != 0) {
		minRight.data /= minRight.count;
	}

	// 閾値よりも全ての距離が遠い場合は何もしない
	if (DISTANCE_THRESHOLD < minLeft.data &&
		DISTANCE_THRESHOLD < minMid.data &&
		DISTANCE_THRESHOLD < minRight.data
		) {
		return position::None;
	}
	ImGui::Begin("LookFromSencor");
	ImGui::SetWindowFontScale(3.0f);
	// 最小値が一番小さい位置を返す
	// 左
	if (minLeft.data <= minMid.data && minLeft.data <= minRight.data) {
		ImGui::Text("LEFT");
		ImGui::End();
		return position::pLEFT;
	}
	// 中央
	else if (minMid.data <= minLeft.data && minMid.data <= minRight.data) {
		ImGui::Text("MID");
		ImGui::End();
		return position::pMID;
	}
	// 右
	else if (minRight.data <= minLeft.data && minRight.data <= minMid.data) {
		ImGui::Text("RIGHT");
		ImGui::End();
		return position::pRIGHT;
	}
	ImGui::End();
}
