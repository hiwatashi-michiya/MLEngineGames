#pragma once
#include<memory>
#include <cstdint>
#include <string>

class GameManager
{
public:
    // シングルトン取得
    static GameManager* GetInstance();

    // 初期化・終了処理
    void Initialize();
    void Finalize();

    // フレーム更新
    void Update();

    // 状態管理
    enum class GameState {
        Title,
        Playing,
        Result
    };

    void SetState(GameState newState) { state_ = newState; }
    GameState GetState() const { return state_; }

    // スコア管理
    void AddScore(int value);
    int GetScore() const { return score_; }

    // 残り時間（制限時間）管理
    void SetTimeLimit(float t) { timeLimit_ = t; }
    float GetTimeLimit() const { return timeLimit_; }
    float GetRemainingTime() const { return remainingTime_; }
    float GetDeltaTime() const { return deltaTime_; }

    // 設定関連
    void SetDifficulty(int d) { difficulty_ = d; }
    int GetDifficulty() const { return difficulty_; }

private:
    // コンストラクタ
    GameManager() = default;
    ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

private:
    // ゲーム全体の状態
    GameState state_ = GameState::Title;

    // スコア
    int score_ = 0;

    float deltaTime_ = 1.0f / 60.0f;
    
    // 制限時間管理
    float timeLimit_ = 60.0f;       // 秒
    float remainingTime_ = 60.0f;

    // 難易度などの設定
    int difficulty_ = 1;
};
