#pragma once
#include<memory>
#include <cstdint>
#include <string>
#include"VirtualController.h"
#include "Audio/Audio.h"
#include "Sprite/Sprite2D.h"

class GameManager
{
public:
    // シングルトン取得
    static GameManager* GetInstance();

    // 初期化・終了処理
    void Initialize();
    void Finalize();

    // フレーム更新
    void Update(bool isJustTurned, bool isJustMoved);

    //debug
    void Debug();

    // シーン更新
    void SceneUpdate();

    // 状態管理
    enum class GameState : uint8_t {
        Title,      //タイトル
        Tutorial,   //チュートリアル
        Playing,    //本編
        Result      //リザルト
    };

    // 状態管理
    enum class TutorialState : uint8_t {
        LaneMove,      //レーン移動
        FlontBack,   //振り向き
        Wait      //待機
    };
    

    void SetState(GameState newState) { nextState_ = newState; }
    GameState GetState() const { return state_; }

    TutorialState GetTutorialState() const { return tuState_; }
    
    bool GetIsClear()const { return isClear_; }
    bool GetIsGameOver() const { return isGameOver_; }

    void SetIsClear(bool isClear) { isClear_ = isClear; }
    void SetGameEnd(bool isGameEnd) { isGameEnd_ = isGameEnd; }
    void SetIsGameOver(bool flag) { isGameOver_ = flag; }

    // スコア管理
    void AddScore(bool isCombo);
    int GetScore() const { return score_; }

    int GetCombo() const { return scratchCombo_; }

    // 残り時間（制限時間）管理
    void SetTimeLimit(float t) { timeLimit_ = t; }
    float GetTimeLimit() const { return timeLimit_; }
    float GetRemainingTime() const { return remainingTime_; }
    float GetDeltaTime() const { return deltaTime_; }

    // 設定関連
    void SetDifficulty(int d) { difficulty_ = d; }
    int GetDifficulty() const { return difficulty_; }

    //リセット関数
    void ResetScore() { score_ = 0; }
    void ResetCombo() { scratchCombo_ = 0; }

private:
    // コンストラクタ
    GameManager() = default;
    ~GameManager() = default;
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;



private:

    //入力デバイス
    VirtualController* vController_ = nullptr;

    // ゲーム全体の状態
    GameState state_ = GameState::Title;
    GameState nextState_ = GameState::Title;

    // チュートリアルの状態
    TutorialState tuState_ = TutorialState::LaneMove;

    //チュートリアルクリアしたかどうか
    bool isTutorialClear_ = false;

    //シーン切り替えフラグ
    bool isSceneChange_ = false;
    //ゲームクリアしたかどうか
    bool isClear_ = false;
    bool isGameEnd_ = false;
    bool isGameOver_ = false;
    bool isReset_ = false;

    //チュートリアルでのカウント
    int moveCount_ = 0;
    int turnCount_ = 0;
    int moveCountMax_ = 0;
    int turnCountMax_ = 0;

    float waitTime_ = 1.0f;
    float time_ = 0.0f;
    //後で正式なものと交換
    float deltaTime_ = 1.0f / 60.0f;
    //基礎点数
    int scoreBase_ = 1;
    //コンボによる倍率
    int scoreMagnification_ = 2;

    // スコア
    int score_ = 0;
    //傷コンボ
    int scratchCombo_ = 0;
    
    // 制限時間管理
    float timeLimit_ = 60.0f;       // 秒
    float remainingTime_ = 60.0f;

    // 難易度などの設定
    int difficulty_ = 1;

    //シーン切り替え秒数
    float sceneChangeTime_ = 1.0f;
    //シーン切り替えカウント
    float sceneChangeCounter_ = 0.0f;
    //ゲームオーバー後の待機時間
    float gameOverWaitTime_ = 3.0f;
    //ゲームオーバー待機カウント
    float gameOverWaitCounter_ = 0.0f;
    //ゲームクリア後の待機時間
    float gameClearWaitTime_ = 1.5f;
    //ゲームクリア待機カウント
    float gameClearWaitCounter_ = 0.0f;
    //画像の最大スケール
    MLEngine::Math::Vector2 maxSpriteScale_ = { 10.0f,10.0f };

    //BGM
    MLEngine::Resource::Audio titleBGM_;
    MLEngine::Resource::Audio tutorialBGM_;
    MLEngine::Resource::Audio ingameBGM_;
    MLEngine::Resource::Audio resultBGM_;
    //SE
    MLEngine::Resource::Audio titleStartSE_;
    MLEngine::Resource::Audio tutorialClearSE_;
    MLEngine::Resource::Audio gameOverSE_;
    MLEngine::Resource::Audio clearSE_;
    //シーン切り替え用画像
    MLEngine::Resource::Texture sceneChangeTex_;
    std::unique_ptr<MLEngine::Resource::Sprite2D> sceneChangeSprite_;

};
