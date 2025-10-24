#include "GameManager.h"

GameManager* GameManager::GetInstance() {
    static GameManager instance;
    return &instance;
}

void GameManager::Initialize() {
    score_ = 0;
    remainingTime_ = timeLimit_;
    state_ = GameState::Title;
}

void GameManager::Finalize() {
    // 保存や後処理など
}

void GameManager::Update() {
    if (state_ == GameState::Playing) {
        remainingTime_ -= deltaTime_;
        if (remainingTime_ <= 0.0f) {
            remainingTime_ = 0.0f;
            state_ = GameState::Result;
        }
    }
}

void GameManager::AddScore(int value) {
    score_ += value;
}