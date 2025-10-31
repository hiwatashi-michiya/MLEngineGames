#include "NetworkManager.h"
#include"Player/Player.h"

bool NetworkManager::Initialize(bool isServer, const std::string& ip, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return false;
    }

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ == INVALID_SOCKET) {
        std::cerr << "socket creation failed." << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (isServer) {
        // サーバーとして待機
        if (bind(sock_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            std::cerr << "bind failed." << std::endl;
            closesocket(sock_);
            WSACleanup();
            return false;
        }

        listen(sock_, 1);
        std::cout << "Waiting for client..." << std::endl;
        SOCKET client = accept(sock_, nullptr, nullptr);
        if (client == INVALID_SOCKET) {
            std::cerr << "accept failed." << std::endl;
            closesocket(sock_);
            WSACleanup();
            return false;
        }

        closesocket(sock_);
        sock_ = client;
        std::cout << "Client connected!" << std::endl;
    }
    else {
        // クライアントとして接続
        if (connect(sock_, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
            std::cerr << "connect failed." << std::endl;
            closesocket(sock_);
            WSACleanup();
            return false;
        }
        std::cout << "Connected to server!" << std::endl;
    }

    running_ = true;
    recvThread_ = std::thread(&NetworkManager::RecvLoop, this);
    return true;
}

void NetworkManager::Finalize() {
    running_ = false;
    if (recvThread_.joinable()) recvThread_.join();

    if (sock_ != INVALID_SOCKET) {
        shutdown(sock_, SD_BOTH);
        closesocket(sock_);
    }

    WSACleanup();
}

void NetworkManager::RecvLoop() {
    while (running_) {
        std::vector<char> buffer(1024);
        int size = recv(sock_, buffer.data(), (int)buffer.size(), 0);

        if (size > 0) {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer.resize(size);
            recvQueue_.push(buffer);
        }
        else if (size == 0) {
            std::cout << "Connection closed by remote host." << std::endl;
            running_ = false;
        }
        else {
            int err = WSAGetLastError();
            if (err != WSAEWOULDBLOCK) {
                std::cerr << "recv failed with error: " << err << std::endl;
                running_ = false;
            }
        }
    }
}

template <typename T>
void NetworkManager::Send(const T& data) {
    if (sock_ == INVALID_SOCKET) return;
    send(sock_, reinterpret_cast<const char*>(&data), sizeof(T), 0);
}

template <typename T>
bool NetworkManager::Receive(T& outData) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (recvQueue_.empty()) return false;

    auto buffer = recvQueue_.front();
    recvQueue_.pop();

    if (buffer.size() == sizeof(T)) {
        std::memcpy(&outData, buffer.data(), sizeof(T));
        return true;
    }
    return false;
}

void NetworkManager::Update() {
   
}

// 明示的なテンプレートインスタンス化
template void NetworkManager::Send(const struct Player::SendPlayerState& data);
template bool NetworkManager::Receive(struct Player::SendPlayerState& outData);
