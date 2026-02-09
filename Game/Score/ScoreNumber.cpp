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
	scoreCombo_.Initialize(texture_, {});

	texture_.Load("./Resources/Texture/number.png");
	scoreNumTen_.Initialize(texture_, {});
	scoreNumOne_.Initialize(texture_, {});
	comboNumTen_.Initialize(texture_, {});
	comboNumOne_.Initialize(texture_, {});

}

void ScoreNumber::Initialize() {
	/*ingameStartUI_.Initialize(ingameStartTex_, {});
	ingameStartUI_.startPosition = { 2920.0f, 540.0f };
	ingameStartUI_.middlePosition = { 960.0f, 540.0f };
	ingameStartUI_.endPosition = { -1000.0f, 540.0f };
	ingameStartUI_.easingTime = 4.0f;
	ingameStartUI_.startToMiddleTime = 1.0f;
	ingameStartUI_.stayMiddleTime = 2.0f;*/
	
}

void ScoreNumber::SetIsActive(bool flag) {

	scoreBoard_->isActive = flag;
	scoreCombo_.SetIsActive(flag);
	scoreNumTen_.SetIsActive(flag);
	scoreNumOne_.SetIsActive(flag);
	comboNumTen_.SetIsActive(flag);
	comboNumOne_.SetIsActive(flag);

}

void ScoreNumber::Update() {
	GlobalGetValues();
	ScoreCulc();
	ComboCulc();

	//ボード
	scoreBoard_->position = scoreBoardPos_;
	scoreBoard_->size = scoreBoardSize_;
	

	//スコアの数字十の位
	scoreNumTen_.startPosition = scoreNumTenPos_;
	scoreNumTen_.middlePosition = scoreNumTenPos_;
	scoreNumTen_.endPosition = scoreNumTenPos_;
	scoreNumTen_.startScale = scoreNumTenSize_;
	scoreNumTen_.middleScale = scoreNumTenSize_ * magnification_;
	scoreNumTen_.endScale = scoreNumTenSize_;
	scoreNumTen_.SetUVScale({ 0.1f,1.0f });

	//スコアの数字一の位
	scoreNumOne_.startPosition = scoreNumOnePos_;
	scoreNumOne_.middlePosition = scoreNumOnePos_;
	scoreNumOne_.endPosition = scoreNumOnePos_;
	scoreNumOne_.startScale = scoreNumTenSize_;
	scoreNumOne_.middleScale = scoreNumTenSize_ * magnification_;
	scoreNumOne_.endScale = scoreNumTenSize_;
	scoreNumOne_.SetUVScale({ 0.1f,1.0f });

	//コンボのボード
	scoreCombo_.startPosition = scoreComboPos_;
	scoreCombo_.middlePosition = scoreComboPos_;
	scoreCombo_.endPosition = scoreComboPos_;
	scoreCombo_.startScale = scoreComboSize_;
	scoreCombo_.middleScale = scoreComboSize_ * magnification_;
	scoreCombo_.endScale = scoreComboSize_;
	scoreCombo_.SetRotate(comboRotate_);
	//コンボの十の位
	comboNumTen_.startPosition = comboNumTenPos_;
	comboNumTen_.middlePosition = comboNumTenPos_;
	comboNumTen_.endPosition = comboNumTenPos_;
	comboNumTen_.startScale = comboNumTenSize_;
	comboNumTen_.middleScale = comboNumTenSize_ * magnification_;
	comboNumTen_.endScale = comboNumTenSize_;
	comboNumTen_.SetUVScale({ 0.1f,1.0f });
	comboNumTen_.SetRotate(comboRotate_);
	//コンボの一の位
	comboNumOne_.startPosition = comboNumOnePos_;
	comboNumOne_.middlePosition = comboNumOnePos_;
	comboNumOne_.endPosition = comboNumOnePos_;
	comboNumOne_.startScale = comboNumTenSize_;
	comboNumOne_.middleScale = comboNumTenSize_ * magnification_;
	comboNumOne_.endScale = comboNumTenSize_;
	comboNumOne_.SetUVScale({ 0.1f,1.0f });
	comboNumOne_.SetRotate(comboRotate_);

	NumberDrawControl();
	
	scoreNumTen_.Update();
	scoreNumOne_.Update();
	scoreCombo_.Update();
	comboNumTen_.Update();
	comboNumOne_.Update();

	//スコアの数字
	scoreNumTen_.SetUVTrans({ 0.1f * scoreTen_,0.0f });
	scoreNumTen_.SetColor(scoreNumColor_);
	scoreNumOne_.SetUVTrans({ 0.1f * scoreOne_,0.0f });
	scoreNumOne_.SetColor(scoreNumColor_);
	//コンボの数字
	comboNumTen_.SetUVTrans({ 0.1f * comboTen_,0.0f });
	comboNumTen_.SetColor(comboNumColor_);
	comboNumOne_.SetUVTrans({ 0.1f * comboOne_,0.0f });
	comboNumOne_.SetColor(comboNumColor_);
	

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

void ScoreNumber::NumberDrawControl(){
		//コンボが０の場合
	if (comboTen_ == 0 and comboOne_ == 0){
		scoreCombo_.startScale = Vector2{};
		comboNumTen_.startScale = Vector2{};
		comboNumOne_.startScale = Vector2{};

		scoreCombo_.endScale = Vector2{};
		comboNumTen_.endScale = Vector2{};
		comboNumOne_.endScale = Vector2{};

	}
	//コンボが０の場合
	if (comboTen_ == 0 and comboOne_ == 1) {
		scoreCombo_.startScale = Vector2{};
		comboNumTen_.startScale = Vector2{};
		comboNumOne_.startScale = Vector2{};
	}

	if (scoreTen_ > 0) {
		scoreNumTen_.SetIsActive(true);
	}
	else {
		scoreNumTen_.SetIsActive(false);

		scoreNumOne_.startPosition = (scoreNumOnePos_ + scoreNumTenPos_) / 2.0f;
		scoreNumOne_.middlePosition = (scoreNumOnePos_ + scoreNumTenPos_) / 2.0f;
		scoreNumOne_.endPosition = (scoreNumOnePos_ + scoreNumTenPos_) / 2.0f;
	}

	if (comboTen_ > 0) {
		comboNumTen_.SetIsActive(true);
	}
	else {
		comboNumTen_.SetIsActive(false);

		comboNumOne_.startPosition = (comboNumOnePos_ + comboNumTenPos_) / 2.0f;
		comboNumOne_.middlePosition = (comboNumOnePos_ + comboNumTenPos_) / 2.0f;
		comboNumOne_.endPosition = (comboNumOnePos_ + comboNumTenPos_) / 2.0f;

	}




}
