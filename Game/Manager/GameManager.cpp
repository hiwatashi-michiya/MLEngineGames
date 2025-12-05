#include "GameManager.h"

GameManager* GameManager::GetInstance() {
    static GameManager instance;
    return &instance;
}

void GameManager::Initialize() {
    vController_ = &VirtualController::GetInstance();

    score_ = 0;
    remainingTime_ = timeLimit_;
    state_ = GameState::Title;
}

void GameManager::Finalize() {
    // 保存や後処理など
}

void GameManager::Update() {
    switch (state_){
    case GameManager::GameState::Title:
        if (vController_->Decide()) {
            state_ = GameState::Tutorial;
        };


        break;
    case GameManager::GameState::Tutorial:



        break;
    case GameManager::GameState::Playing:



        break;
    case GameManager::GameState::Result:



        break;
    default:
        break;
    }


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