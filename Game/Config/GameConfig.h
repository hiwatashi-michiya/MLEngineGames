#pragma once
class GameConfig{

public:
    static GameConfig& Instance() {
        static GameConfig instance;
        return instance;
    }

    void Initialize();  // 読み込み・初期化など

    // ゲーム全体設定
    float timeLimit_ = 180.0f;    // 制限時間(秒)
    int maxLane_ = 5;             // レーンの最大数
    bool enableDebugMode_ = false;

private:
    GameConfig() = default;
};

