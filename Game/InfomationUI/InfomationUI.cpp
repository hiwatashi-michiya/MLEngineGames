#include "InfomationUI.h"
#include"Manager/GameManager.h"
#include<Engine/Tool/GlobalVariables.h>
#include"Externals/imgui/imgui.h"

InfomationUI::InfomationUI() {
	GlobalSetValues();

	//必須となる情報の読み込み
	texture_.Load("./Resources/Texture/ingame_UI_enemyHP.png");
	speech_Bubble_.Initialize(texture_, {}, baseColor_);

	texture_.Load("./Resources/Texture/enemy1_normal.png");
	enemyTex_.Initialize(texture_, {});

	texture_.Load("./Resources/Texture/enemy_hpGage.png");
	hpFrame_.Initialize(texture_, {});

	texture_.Load("./Resources/white.png");
	hpBar_.Initialize(texture_, {});

}

void InfomationUI::Initialize() {
	

}

void InfomationUI::SetIsActive(bool flag) {
	speech_Bubble_.SetIsActive(flag);
	enemyTex_.SetIsActive(flag);
	hpFrame_.SetIsActive(flag);
	hpBar_.SetIsActive(flag);

}

void InfomationUI::Update() {
	GlobalGetValues();

	//スコアの数字十の位
	speech_Bubble_.startPosition = speech_BubblePos_;
	speech_Bubble_.middlePosition = speech_BubblePos_;
	speech_Bubble_.endPosition = speech_BubblePos_;
	speech_Bubble_.startScale = speech_BubbleSize_;
	speech_Bubble_.middleScale = speech_BubbleSize_;
	speech_Bubble_.endScale = speech_BubbleSize_;

	//スコアの数字一の位
	enemyTex_.startPosition = enemyTexPos_;
	enemyTex_.middlePosition = enemyTexPos_;
	enemyTex_.endPosition = enemyTexPos_;
	enemyTex_.startScale = enemyTexSize_;
	enemyTex_.middleScale = enemyTexSize_;
	enemyTex_.endScale = enemyTexSize_;
	enemyTex_.SetUVScale({ 0.2f,1.0f });

	//コンボのボード
	hpFrame_.startPosition = hpFramePos_;
	hpFrame_.middlePosition = hpFramePos_;
	hpFrame_.endPosition = hpFramePos_;
	hpFrame_.startScale = hpFrameSize_;
	hpFrame_.middleScale = hpFrameSize_;
	hpFrame_.endScale = hpFrameSize_;
	//コンボの十の位
	hpBar_.startPosition = hpBarPos_;
	hpBar_.middlePosition = hpBarPos_;
	hpBar_.endPosition = hpBarPos_;
	hpBar_.startScale = hpBarSize_;
	hpBar_.middleScale = hpBarSize_;
	hpBar_.endScale = hpBarSize_;

	NumberDrawControl();

	speech_Bubble_.Update();
	enemyTex_.Update();
	hpFrame_.Update();
	hpBar_.Update();

}

void InfomationUI::GlobalSetValues() {
	GlobalVariables* global = GlobalVariables::GetInstance();
	//ボードの調整
	global->SetValue("InfoUIState", "speechPos", speech_BubblePos_);
	global->SetValue("InfoUIState", "speechSize", speech_BubbleSize_);
	//コンボの調整
	global->SetValue("InfoUIState", "enemyTexPos", enemyTexPos_);
	global->SetValue("InfoUIState", "enemyTexSize", enemyTexSize_);
	//スコアの数字の調整
	global->SetValue("InfoUIState", "hpFranePos", hpFramePos_);
	global->SetValue("InfoUIState", "hpFrameSize", hpFrameSize_);

	//コンボの数字の調整
	global->SetValue("InfoUIState", "hpBarPos", hpBarPos_);
	global->SetValue("InfoUIState", "hpBarSize", hpBarSize_);

}

void InfomationUI::GlobalGetValues() {
	GlobalVariables* global = GlobalVariables::GetInstance();
	//ボードの調整
	speech_BubblePos_ = global->GetVector2Value("InfoUIState", "speechPos");
	speech_BubbleSize_ = global->GetVector2Value("InfoUIState", "speechSize");
	//コンボの調整
	enemyTexPos_ = global->GetVector2Value("InfoUIState", "enemyTexPos");
	enemyTexSize_ = global->GetVector2Value("InfoUIState", "enemyTexSize");
	//スコアの数字の調整
	hpFramePos_ = global->GetVector2Value("InfoUIState", "hpFranePos");
	hpFrameSize_ = global->GetVector2Value("InfoUIState", "hpFrameSize");

	//コンボの数字の調整
	hpBarPos_ = global->GetVector2Value("InfoUIState", "hpBarPos");
	hpBarSize_ = global->GetVector2Value("InfoUIState", "hpBarSize");
}

void InfomationUI::DebugDraw() {

}

void InfomationUI::NumberDrawControl() {




}