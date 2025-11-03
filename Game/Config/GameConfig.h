#pragma once
class GameConfig{

public:
    static GameConfig* GetInstance() {
        static GameConfig instance;
        return &instance;
    }

    void Initialize();  // 読み込み・初期化など

    void Update();//ゲーム中での変更に対応するため

    void Debug();//imgui

    // ゲーム全体設定
    float timeLimit_ = 180.0f;    // 制限時間(秒)
    const int centerPos_ = 640;//中心座標
    int maxLane_ = 3;             // レーンの最大数
    int centerLane_;//中心のライン
    int laneDistancePlayer_ = 350;//プレイヤー側のレーンの幅
    bool enableDebugMode_ = false;//デバックモードにするかどうか

private:
    GameConfig() = default;
};

