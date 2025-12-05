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
    nextState_ = GameState::Playing;
    tuState_ = TutorialState::LaneMove;
    isTutorialClear_ = false;
}

void GameManager::Finalize() {
    // 保存や後処理など
}

void GameManager::Update() {
    switch (state_){
    case GameManager::GameState::Title:
        if (vController_->Decide()) {
            nextState_ = GameState::Tutorial;
        };


        break;
    case GameManager::GameState::Tutorial:

        switch (tuState_){
        case GameManager::TutorialState::LaneMove:
            break;
        case GameManager::TutorialState::FlontBack:
            break;
        case GameManager::TutorialState::Wait:
            break;
        default:
            break;
        }


        if (isTutorialClear_){
            nextState_ = GameState::Playing;
        }

        break;
    case GameManager::GameState::Playing:



        break;
    case GameManager::GameState::Result:
        if (vController_->Decide()) {
            nextState_ = GameState::Title;
        };


        break;
    default:
        break;
    }

    
   
}

void GameManager::SceneUpdate(Player* player){
    if (nextState_ != state_) {
        state_ = nextState_;
    }
}

void GameManager::AddScore(int value) {
    score_ += value;
}