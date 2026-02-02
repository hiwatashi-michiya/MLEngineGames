#include "GameManager.h"

#include"Engine/Scene/SceneManager.h"
#include<Engine/Tool/GlobalVariables.h>
#include"Externals/imgui/imgui.h"


GameManager* GameManager::GetInstance() {
    static GameManager instance;
    return &instance;
}

void GameManager::Initialize() {
    vController_ = &VirtualController::GetInstance();
    GlobalVariables* global = GlobalVariables::GetInstance();

    global->SetValue("Tutorial", "TurnCount", turnCountMax_);
    global->SetValue("Tutorial", "MoveCount", moveCountMax_);
    global->SetValue("Tutorial", "WaitTime", waitTime_);


    score_ = 0;
    remainingTime_ = timeLimit_;
    state_ = GameState::Title;
    nextState_ = GameState::Title;
    tuState_ = TutorialState::LaneMove;
    isTutorialClear_ = false;
    isGameEnd_ = false;
    isClear_ = false;

    moveCount_ = 0;
    turnCount_ = 0;
    time_ = 0;

    ResetCombo();
}

void GameManager::Finalize() {
    // 保存や後処理など
}

void GameManager::Update(bool isJustTurned, bool isJustMoved) {
    GlobalVariables* global = GlobalVariables::GetInstance();

    turnCountMax_ = global->GetIntValue("Tutorial", "TurnCount");
    moveCountMax_ = global->GetIntValue("Tutorial", "MoveCount");
    waitTime_ = global->GetFloatValue("Tutorial", "WaitTime");

    /*ゴミなのでちゃんとstatePatternにします・・・*/
    switch (state_){
    case GameManager::GameState::Title:
        //決定ボタンでチュートリアルに移行
        if (vController_->Decide()) {
            nextState_ = GameState::Tutorial;
        };
        ResetScore();
        ResetCombo();
        break;
    case GameManager::GameState::Tutorial:

        switch (tuState_){
        case GameManager::TutorialState::LaneMove:
            if (isJustMoved){
                moveCount_++;
            }

            if (moveCount_ >= moveCountMax_){
                tuState_ = TutorialState::FlontBack;
            }

            break;
        case GameManager::TutorialState::FlontBack:
            if (isJustTurned){
                turnCount_++;
            }

            if (turnCount_ >= turnCountMax_) {
                tuState_ = TutorialState::Wait;
            }
            break;
        case GameManager::TutorialState::Wait:
            if (!isJustMoved and !isJustTurned) {
                time_ += deltaTime_;
            }

            if (time_ >= waitTime_){
                isTutorialClear_ = true;
            }
            break;
        default:
            break;
        }


        if (isTutorialClear_){
            nextState_ = GameState::Playing;
        }

        break;
    case GameManager::GameState::Playing:
        //ゲームが終わったらリザルトに
        if (isGameEnd_){
            nextState_ = GameState::Result;
        }

        break;
    case GameManager::GameState::Result:
        //決定ボタンでシーンを初期化
        if (vController_->Decide()) {
            MLEngine::Scene::Manager::GetInstance()->ChangeScene("Play");
        };


        break;
    default:
        break;
    }

    
   
}

void GameManager::Debug() {
#ifdef _DEBUG
    ImGui::Begin("チュートリアル");

    ImGui::SliderInt("左右移動した回数", &moveCount_, 0, 10);
    ImGui::SliderInt("反転した回数", &turnCount_, 0, 10);
    ImGui::Text("経過時間：%.1f", time_);

    ImGui::End();

    ImGui::Begin("スコア回り");

    ImGui::Text("スコア：%d", score_);
    ImGui::Text("傷コンボ：%d", scratchCombo_);

    ImGui::End();
#endif // _DEBUG

}

void GameManager::SceneUpdate(){
    if (nextState_ != state_) {
        state_ = nextState_;
    }
}

/// <summary>
/// 条件によって変動するスコアを加算する関数
/// </summary>
/// <param name="isCombo"></param>
void GameManager::AddScore(bool isCombo){
    if (isCombo){
        score_ += scoreBase_ * scoreMagnification_;        
    }
    else {
        score_ += scoreBase_;
    }

    scratchCombo_++;
}

