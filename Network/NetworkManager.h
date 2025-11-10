#pragma once
#include <Engine/Core/WindowManager.h>
#include <process.h>
#include <mmsystem.h>
#include <fstream>
#include <iostream>
#include <atomic>


#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "winmm.lib")

class NetworkManager {
public:
#pragma pack(push, 1)	
    //通信で送る情報をまとめた構造体
    struct SendPlayerState {
        //前を向いているか
        bool isForwardFlug;
        //攻撃をくらったか
        bool isDamagedFlug;
        //体力
        int life;
        //現在いるラインの番号
        int nowLine;
    };
#pragma pack(pop)

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

    void RecvLoop();

    static DWORD WINAPI RecvLoopWrapper(void* param) {
        NetworkManager* self = reinterpret_cast<NetworkManager*>(param);
        self->RecvLoop();
        return 0;
    }

    HANDLE hThread_;
    DWORD dwID_;

    SOCKET sWait_, sConnect_;                // 待機用と接続用
    struct sockaddr_in saConnect_, recvConnect_ {};
    WORD wPort_ = 8000;
    int iLen_;
    int fromlen_ = 0;
    char addr_[20]; //IPアドレス用文字列を設定

    int waitSecond_ = 5;

    // 接続状態を管理するフラグ
    std::atomic<bool> isRunning_;

    SendPlayerState playerState_{};

};

