#include "ScoreNumber.h"
#include"Manager/GameManager.h"
#include<Engine/Tool/GlobalVariables.h>
#include"Externals/imgui/imgui.h"

ScoreNumber::ScoreNumber(){
	GlobalSetValues();

	//必須となる情報の読み込み
	texture_.Load("./Resources/Texture/ingame_UI_scoreBack.png");
	scoreBoard_.reset(MLEngine::Resource::Sprite2D::Create(texture_, scoreBoardPos_, baseColor_));

	texture_.Load("./Resources/Texture/ingame_UI_combo.png");
	scoreCombo_.reset(MLEngine::Resource::Sprite2D::Create(texture_, scoreComboPos_, comboColor_));

	texture_.Load("./Resources/Texture/number.png");
	scoreNumTen_.reset(MLEngine::Resource::Sprite2D::Create(texture_, scoreNumTenPos_, scoreNumColor_));
	scoreNumOne_.reset(MLEngine::Resource::Sprite2D::Create(texture_, scoreNumOnePos_, scoreNumColor_));
	comboNumTen_.reset(MLEngine::Resource::Sprite2D::Create(texture_, comboNumTenPos_, comboNumColor_));
	comboNumOne_.reset(MLEngine::Resource::Sprite2D::Create(texture_, comboNumOnePos_, comboNumColor_));

}

void ScoreNumber::Initialize() {
	

}

void ScoreNumber::Update() {
	GlobalGetValues();
	ScoreCulc();
	ComboCulc();
	//ボード
	scoreBoard_->position = scoreBoardPos_;
	scoreBoard_->size = scoreBoardSize_;
	
	//スコアの数字
	scoreNumTen_->position = scoreNumTenPos_;
	scoreNumTen_->size = scoreNumTenSize_;
	scoreNumTen_->uvScale.x = 0.1f;
	scoreNumTen_->uvTranslate.x = 0.1f * scoreTen_;

	scoreNumOne_->position = scoreNumOnePos_;
	scoreNumOne_->size = scoreNumTenSize_;
	scoreNumOne_->uvScale.x = 0.1f;
	scoreNumOne_->uvTranslate.x = 0.1f * scoreOne_;


	//コンボ
	scoreCombo_->position = scoreComboPos_;
	scoreCombo_->size = scoreComboSize_;
	//コンボの数字
	comboNumTen_->position = comboNumTenPos_;
	comboNumTen_->size = comboNumTenSize_;
	comboNumTen_->uvScale.x = 0.1f;
	comboNumTen_->uvTranslate.x = 0.1f * comboTen_;

	comboNumOne_->position = comboNumOnePos_;
	comboNumOne_->size = comboNumTenSize_;
	comboNumOne_->uvScale.x = 0.1f;
	comboNumOne_->uvTranslate.x = 0.1f * comboOne_;

	//コンボが0の場合
	if (comboTen_ == 0 and comboOne_ == 0){
		scoreCombo_->isActive = false;
		comboNumTen_->isActive = false;
		comboNumOne_->isActive = false;
	}
	else {
		scoreCombo_->isActive = true;
		comboNumTen_->isActive = true;
		comboNumOne_->isActive = true;
	}

}

void ScoreNumber::GlobalSetValues(){
	GlobalVariables* global = GlobalVariables::GetInstance();
	//ボードの調整
	global->SetValue("UIState", "ScoreBoardPos", scoreBoardPos_);
	global->SetValue("UIState", "ScoreBoardSize", scoreBoardSize_);
	//コンボの調整
	global->SetValue("UIState", "ComboColor", comboColor_);
	global->SetValue("UIState", "ScoreComboPos", scoreComboPos_);
	global->SetValue("UIState", "ScoreComboSize", scoreComboSize_);
	//スコアの数字の調整
	global->SetValue("UIState", "ScoreNumColor", scoreNumColor_);
	global->SetValue("UIState", "scoreNumTenPos", scoreNumTenPos_);
	global->SetValue("UIState", "scoreNumTenSize", scoreNumTenSize_);
	
	global->SetValue("UIState", "scoreNumOnePos", scoreNumOnePos_);

	//コンボの数字の調整
	global->SetValue("UIState", "comboNumTenPos", comboNumTenPos_);
	global->SetValue("UIState", "comboNumTenSize", comboNumTenSize_);
	
	global->SetValue("UIState", "comboNumOnePos", comboNumOnePos_);

	global->SetValue("UIState", "comboRotate", comboRotate_);
}

void ScoreNumber::GlobalGetValues(){
	GlobalVariables* global = GlobalVariables::GetInstance();
	//ボードの調整
	scoreBoardPos_ = global->GetVector2Value("UIState", "ScoreBoardPos");
	scoreBoardSize_ = global->GetVector2Value("UIState", "ScoreBoardSize");
	//コンボの調整
	comboColor_ = global->GetVector4Value("UIState", "ComboColor");
	scoreComboPos_ = global->GetVector2Value("UIState", "ScoreComboPos");
	scoreComboSize_ = global->GetVector2Value("UIState", "ScoreComboSize");
	//スコアの数字の調整
	scoreNumColor_ = global->GetVector4Value("UIState", "ScoreNumColor");
	scoreNumTenPos_ = global->GetVector2Value("UIState", "scoreNumTenPos");
	scoreNumTenSize_ = global->GetVector2Value("UIState", "scoreNumTenSize");

	scoreNumOnePos_ = global->GetVector2Value("UIState", "scoreNumOnePos");

	//コンボの数字の調整
	comboNumTenPos_ = global->GetVector2Value("UIState", "comboNumTenPos");
	comboNumTenSize_ = global->GetVector2Value("UIState", "comboNumTenSize");

	comboNumOnePos_ = global->GetVector2Value("UIState", "comboNumOnePos");

	comboRotate_ = global->GetFloatValue("UIState", "comboRotate");
}

void ScoreNumber::DebugDraw() {

}

void ScoreNumber::ScoreCulc(){
	scoreTen_ = GameManager::GetInstance()->GetScore() / 10;
	scoreOne_ = GameManager::GetInstance()->GetScore() % 10;

	

}

void ScoreNumber::ComboCulc(){
	comboTen_= GameManager::GetInstance()->GetCombo() / 10;
	comboOne_ = GameManager::GetInstance()->GetCombo() % 10;
}
