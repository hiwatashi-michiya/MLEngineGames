#pragma once
#include"Input.h"

//入力をまとめる仮想コントローラー
class VirtualController{
public:
    // シングルトンの取得
    static VirtualController& GetInstance() {
        static VirtualController instance;
        return instance;
    }

    void Initialize();

    //上下左右(押した瞬間)
    bool UpTriger() const;
    bool DownTriger() const;
    bool RightTriger() const;
    bool LeftTriger() const;

    //上下左右(押している間)
    bool UpPush() const;
    bool DownPush() const;
    bool RightPush() const;
    bool LeftPush() const;

    //上下左右(離した瞬間)
    bool UpRelease() const;
    bool DownRelease() const;
    bool RightRelease() const;
    bool LeftRelease() const;
    //決定
    bool Decide() const;

private:
    VirtualController() = default;
    //入力デバイス
    MLEngine::Input::Manager* input_ = nullptr;

};

