#include "GameManager.h"

#include"Engine/Scene/SceneManager.h"
#include<Engine/Tool/GlobalVariables.h>
#include"Externals/imgui/imgui.h"
#include "Core/FrameTracker.h"
#include "Utility/Easing.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Utility;

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

    titleBGM_.Load("BGM/BGM_title.mp3");
    tutorialBGM_.Load("BGM/BGM_tutorial.mp3");
    ingameBGM_.Load("BGM/BGM_ingame.mp3");
    resultBGM_.Load("BGM/BGM_result.mp3");

    titleStartSE_.Load("SE/title_start.mp3");
    tutorialClearSE_.Load("SE/tutorial_clear.mp3");

    sceneChangeTex_.Load("./Resources/Texture/sceneChange_player.png");
    sceneChangeSprite_.reset(MLEngine::Resource::Sprite2D::Create(sceneChangeTex_, {960.0f,540.0f}, {1.0f,1.0f,1.0f,1.0f}));
    sceneChangeSprite_->isActive = false;
    sceneChangeSprite_->SetDrawID(1);

    score_ = 0;
    remainingTime_ = timeLimit_;
    state_ = GameState::Title;
    nextState_ = GameState::Title;
    tuState_ = TutorialState::LaneMove;
    isTutorialClear_ = false;
    isGameEnd_ = false;
    isSceneChange_ = false;
    isClear_ = false;

    moveCount_ = 0;
    turnCount_ = 0;
    time_ = 0;
}

void GameManager::Finalize() {
    // 保存や後処理など
}

void GameManager::Update(bool isJustTurned, bool isJustMoved) {
    
    //シーン切り替え中は更新しない
    

    GlobalVariables* global = GlobalVariables::GetInstance();

    turnCountMax_ = global->GetIntValue("Tutorial", "TurnCount");
    moveCountMax_ = global->GetIntValue("Tutorial", "MoveCount");
    waitTime_ = global->GetFloatValue("Tutorial", "WaitTime");

    /*ゴミなのでちゃんとstatePatternにします・・・*/
    switch (state_){
    case GameManager::GameState::Title:

        if (not titleBGM_.IsPlaying()) {
            titleBGM_.Play(Audio::BGMVolume, true);
            tutorialBGM_.Stop();
            ingameBGM_.Stop();
            resultBGM_.Stop();
        }

        //決定ボタンでチュートリアルに移行
        if (vController_->Decide()) {
            titleStartSE_.Play(Audio::SEVolume);
            nextState_ = GameState::Tutorial;
        };
        ResetScore();
        ResetCombo();
        break;
    case GameManager::GameState::Tutorial:

        if (not tutorialBGM_.IsPlaying()) {
            tutorialBGM_.Play(Audio::BGMVolume, true);
            titleBGM_.Stop();
            ingameBGM_.Stop();
            resultBGM_.Stop();
        }

        switch (tuState_){
        case GameManager::TutorialState::LaneMove:
            if (isJustMoved){
                moveCount_++;
            }

            if (moveCount_ >= moveCountMax_){
                tutorialClearSE_.Play(Audio::SEVolume);
                tuState_ = TutorialState::FlontBack;
            }

            break;
        case GameManager::TutorialState::FlontBack:
            if (isJustTurned){
                turnCount_++;
            }

            if (turnCount_ >= turnCountMax_) {
                tutorialClearSE_.Play(Audio::SEVolume);
                tuState_ = TutorialState::Wait;
            }
            break;
        //シーン切り替え実装したら削除
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

        if (not ingameBGM_.IsPlaying()) {
            ingameBGM_.Play(Audio::BGMVolume, true);
            tutorialBGM_.Stop();
            titleBGM_.Stop();
            resultBGM_.Stop();
        }

        //ゲームが終わったらリザルトに
        if (isGameEnd_){
            nextState_ = GameState::Result;
        }

        break;
    case GameManager::GameState::Result:

        if (not resultBGM_.IsPlaying()) {
            resultBGM_.Play(Audio::BGMVolume, true);
            tutorialBGM_.Stop();
            ingameBGM_.Stop();
            titleBGM_.Stop();
        }

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

    //次のシーンが更新されていたら、切り替えをはじめる
    if (nextState_ != state_) {
        isSceneChange_ = true;
    }

    if (not isSceneChange_) {
        return;
    }

    if (sceneChangeCounter_ <= sceneChangeTime_) {

        sceneChangeSprite_->isActive = true;
        sceneChangeCounter_ += 1.0f * MLEngine::Core::FrameTracker::GetInstance()->GetDeltaTimeF();

        Vector2 minSize = { 0.0f,0.0f };
        Vector2 maxSize = sceneChangeSprite_->GetDefaultSize();
        maxSize = { maxSize.x * maxSpriteScale_.x, maxSize.y * maxSpriteScale_.y };

        //半分より超えたら縮小
        if (sceneChangeCounter_ >= sceneChangeTime_ * 0.5f) {
            sceneChangeSprite_->size = Lerp(maxSize, minSize, (sceneChangeCounter_ - sceneChangeTime_ * 0.5f) / (sceneChangeTime_ * 0.5f));

            //次のシーンに切り替え
            if (nextState_ != state_) {
                state_ = nextState_;
            }

        }
        //半分以下なら拡大
        else {
            sceneChangeSprite_->size = Lerp(minSize, maxSize, sceneChangeCounter_ / (sceneChangeTime_ * 0.5f));
        }

    }

    if (sceneChangeCounter_ >= sceneChangeTime_) {
        sceneChangeCounter_ = 0.0f;
        isSceneChange_ = false;
        sceneChangeSprite_->isActive = false;
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

