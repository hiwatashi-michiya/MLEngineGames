#include "NetworkManager.h"
#include <thread>
#include <chrono>


bool NetworkManager::Initialize(bool isServer, const std::string& ip, int port) {
    isRunning_ = false;
    playerState_.life = -1;
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return false;
    }
    if (isServer){
        /*サーバー側の処理*/
        iLen_ = sizeof(saConnect_);

        // 待機ソケット作成
        sWait_ = socket(AF_INET, SOCK_STREAM, 0);

        // 待機ソケットにポート8000番紐づけるbind関数に
        // 引数で渡すSOCKADDR_IN構造体を設定
        ZeroMemory(&recvConnect_, sizeof(recvConnect_));
        recvConnect_.sin_family = AF_INET;
        recvConnect_.sin_addr.s_addr = INADDR_ANY;
        recvConnect_.sin_port = htons(wPort_);

        if (bind(sWait_, (LPSOCKADDR)&recvConnect_, sizeof((sockaddr&)(recvConnect_))) == SOCKET_ERROR) {
            closesocket(sWait_);
            WSACleanup();
            return false;
        }

        if (listen(sWait_, 2) == SOCKET_ERROR) {
            closesocket(sWait_);
            WSACleanup();
            return false;
        }

        std::cout << "[Server] Waiting for client connection..." << std::endl;

        // タイムアウト付きで接続待ち
        fd_set readfds{};
        FD_ZERO(&readfds);
        FD_SET(sWait_, &readfds);

        timeval timeout{};
        timeout.tv_sec = waitSecond_;   // 5秒待つ
        timeout.tv_usec = 0;

        int result = select(0, &readfds, NULL, NULL, &timeout);
        if (result > 0 && FD_ISSET(sWait_, &readfds)) {
            // 接続あり
            sConnect_ = accept(sWait_, (LPSOCKADDR)&saConnect_, &iLen_);
            if (sConnect_ == INVALID_SOCKET) {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                closesocket(sWait_);
                WSACleanup();
                return false;
            }
            std::cout << "[Server] Client connected!" << std::endl;
        }
        else if (result == 0) {
            // タイムアウト（接続なし）
            std::cout << "[Server] No client detected. Running standalone mode." << std::endl;
            sConnect_ = INVALID_SOCKET;  // 通信を行わないモード
        }
        else {
            // select() エラー
            std::cerr << "[Server] select() failed: " << WSAGetLastError() << std::endl;
            closesocket(sWait_);
            WSACleanup();
            return false;
        }

        // 接続待ちソケット解放
        shutdown(sWait_, 2);
        closesocket(sWait_);
    }
    else {
        /*クライアント側の処理*/

        // ipアドレスは外部テキストファイルから読むようにする
        //学校の場合は"schoolip.txt"(net名"kmt-neec")それぞれのネットは"ip.txt"でアドレスを読む 
        std::ifstream ifs("schoolip.txt");
        ifs.getline(addr_, sizeof(addr_));

        sConnect_ = socket(AF_INET, SOCK_STREAM, 0);

        // サーバーを名前で取得する
        HOSTENT* lpHost;

        lpHost = gethostbyname(addr_);

        if (lpHost == NULL) {
            /* サーバーをIPアドレスで取得する */
            iLen_ = inet_addr(addr_);
            lpHost = gethostbyaddr((char*)&iLen_, 4, AF_INET);
        }

        ZeroMemory(&saConnect_, sizeof(sockaddr_in));
        saConnect_.sin_family = lpHost->h_addrtype;
        saConnect_.sin_addr.s_addr = *((u_long*)lpHost->h_addr);
        saConnect_.sin_port = htons(wPort_);

        // --- ここからタイムアウト設定 ---
        u_long mode = 1;
        ioctlsocket(sConnect_, FIONBIO, &mode);

        int result = connect(sConnect_, (SOCKADDR*)&saConnect_, sizeof(saConnect_));
        if (result == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK) {
            fd_set writefds;
            FD_ZERO(&writefds);
            FD_SET(sConnect_, &writefds);

            TIMEVAL timeout{};
            timeout.tv_sec = waitSecond_; // 10秒
            timeout.tv_usec = 0;

            result = select(0, NULL, &writefds, NULL, &timeout);
            if (result == 0) {
                std::cout << "サーバーに接続できません（10秒タイムアウト）" << std::endl;
                closesocket(sConnect_);
                WSACleanup();
                return false;
            }
        }

        // ブロッキングモードに戻す
        mode = 0;
        ioctlsocket(sConnect_, FIONBIO, &mode);
    }
   
    if (sConnect_ == INVALID_SOCKET) {
        std::cout << "[Server] Running without network." << std::endl;
        WSACleanup();
        return false;
    }
   
    

    // クライアント接続があるときのみスレッド起動
    isRunning_ = true;
    hThread_ = (HANDLE)CreateThread(NULL, 0, &RecvLoopWrapper, this, 0, &dwID_);
    return true;
}

void NetworkManager::Finalize() {
    if (sConnect_ != INVALID_SOCKET) {
        shutdown(sConnect_, 2);
        closesocket(sConnect_);
    }

    WSACleanup();
}

void NetworkManager::RecvLoop() {
    /*確認するだけなので現時点では空*/
    while (isRunning_){
        SendPlayerState tmp;
        // テンプレート受信関数
        if (!Receive(tmp)) {

            isRunning_ = false;
           
        }
        else {
            playerState_ = tmp;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));

    }
  

}

template <typename T>
void NetworkManager::Send(const T& data) {
    if (sConnect_ == INVALID_SOCKET) return;
    send(sConnect_, reinterpret_cast<const char*>(&data), sizeof(T), 0);
}

template <typename T>
bool NetworkManager::Receive(T& outData) {
    if (sConnect_ == INVALID_SOCKET) return false;
    // データ受信
    int nRcv = recv(sConnect_, reinterpret_cast<char*>(&outData), sizeof(T), 0);

    if (nRcv == SOCKET_ERROR)return false;

    return true;
}

void NetworkManager::Update() {
   
}

bool NetworkManager::GetLatestPlayerState(SendPlayerState& out) const{

    if (playerState_.life < 0){
        return false;
    }

    out = playerState_;

    return true;
}

// 明示的なテンプレートインスタンス化
template void NetworkManager::Send(const struct SendPlayerState& data);
template bool NetworkManager::Receive(struct SendPlayerState& outData);
