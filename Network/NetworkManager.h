#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <iostream>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager {
public:
    // シングルトンインスタンス
    static NetworkManager& GetInstance() {
        static NetworkManager instance;
        return instance;
    }

    // 初期化
    bool Initialize(bool isServer, const std::string& ip = "127.0.0.1", int port = 8000);
    void Finalize();

    // 構造体を送受信（任意の型に対応）
    template <typename T>
    void Send(const T& data);

    template <typename T>
    bool Receive(T& outData);

    // 毎フレーム呼び出す（内部キュー更新など）
    void Update();

private:
    NetworkManager() = default;
    ~NetworkManager() = default;
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    // 内部関数
    void RecvLoop();

    // メンバ変数
    SOCKET sock_ = INVALID_SOCKET;
    std::thread recvThread_;
    std::mutex mutex_;
    std::queue<std::vector<char>> recvQueue_;
    bool running_ = false;
};

