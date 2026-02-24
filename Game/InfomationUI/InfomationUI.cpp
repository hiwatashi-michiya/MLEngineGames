#include "InfomationUI.h"
#include"Manager/GameManager.h"
#include<Engine/Tool/GlobalVariables.h>
#include"Externals/imgui/imgui.h"

InfomationUI::InfomationUI() {
	GlobalSetValues();

	//必須となる情報の読み込み
	texture_.Load("./Resources/Texture/ingame_UI_enemyHP.png");
	speech_Bubble_.Initialize(texture_, {}, baseColor_);
	speech_Bubble_.easingTime = 0.4f;
	speech_Bubble_.startToMiddleTime = 0.2f;
	speech_Bubble_.stayMiddleTime = 0.0f;

#ifdef CLIENT_BUILD
	texture_.Load("./Resources/Texture/enemy1_normal.png");
#else
	texture_.Load("./Resources/Texture/enemy2_normal.png");
#endif
	
	enemyTex_.Initialize(texture_, {});
	enemyTex_.easingTime = 0.4f;
	enemyTex_.startToMiddleTime = 0.2f;
	enemyTex_.stayMiddleTime = 0.0f;

	texture_.Load("./Resources/Texture/enemy_hpGage.png");
	hpFrame_.Initialize(texture_, {}, { 0.0f,0.0f,0.0f,1.0f });
	hpFrame_.easingTime = 0.4f;
	hpFrame_.startToMiddleTime = 0.2f;
	hpFrame_.stayMiddleTime = 0.0f;

	texture_.Load("./Resources/white.png");
	hpBar_.Initialize(texture_, {}, hpColor_);
	hpBar_.SetAnchorPoint({ 0.0f,0.5f });
	hpBar_.easingTime = 0.4f;
	hpBar_.startToMiddleTime = 0.2f;
	hpBar_.stayMiddleTime = 0.0f;
}

void InfomationUI::Initialize() {
	startEase_ = false;

	isReverse_ = false;

}

void InfomationUI::SetIsActive(bool flag) {
	speech_Bubble_.SetIsActive(flag);
	enemyTex_.SetIsActive(flag);
	hpFrame_.SetIsActive(flag);
	hpBar_.SetIsActive(flag);

}

void InfomationUI::Update() {
	float deltaTime = 1.0f * MLEngine::Core::FrameTracker::GetInstance()->GetDeltaTimeF();

	if (deltaTime > 1.0f){
		deltaTime = 1.0f / 60.0f;
	}

	GlobalGetValues();


	if (startEase_){
		scaleTime_ += deltaTime;
	}

	if (scaleTime_ >= scaleTimeRimit_){
		isReverse_ = true;
		startEase_ = false;
		scaleTime_ = 0.0f;
		hpBar_.ReStart();
		hpFrame_.ReStart();
		speech_Bubble_.ReStart();
		enemyTex_.ReStart();
	}

	if (hpBar_.GetIsEndEasing() == true and isReverse_ ==true){
		isReverse_ = false;
		hpBar_.Stop();
		hpFrame_.Stop();
		speech_Bubble_.Stop();
		enemyTex_.Stop();
	}

	if (isReverse_){
		//大->小
		SetPosSizeReverse();
	}
	else {
		//小->大
		SetPosSize();	
	}

	speech_Bubble_.Update();
	enemyTex_.Update();
	hpFrame_.Update();
	hpBar_.Update();

	

#ifdef _DEBUG
	DebugDraw();
#endif // _DEBUG


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

void InfomationUI::SetPosSize(){
	//吹き出し
	speech_Bubble_.startPosition = infoStartPos_;
	speech_Bubble_.middlePosition = AddStarthalf(speech_BubblePos_);
	speech_Bubble_.endPosition = speech_BubblePos_;
	speech_Bubble_.startScale = Vector2();
	speech_Bubble_.middleScale = speech_BubbleSize_ / 2.0f;
	speech_Bubble_.endScale = speech_BubbleSize_;

	//敵の画像
	enemyTex_.startPosition = infoStartPos_;
	enemyTex_.middlePosition = AddStarthalf(enemyTexPos_);
	enemyTex_.endPosition = enemyTexPos_;
	enemyTex_.startScale = Vector2();
	enemyTex_.middleScale = enemyTexSize_ / 2.0f;
	enemyTex_.endScale = enemyTexSize_;
	enemyTex_.SetUVScale({ 0.2f,1.0f });

	//HPのフレーム
	hpFrame_.startPosition = infoStartPos_;
	hpFrame_.middlePosition = AddStarthalf(hpFramePos_);
	hpFrame_.endPosition = hpFramePos_;
	hpFrame_.startScale = Vector2();
	hpFrame_.middleScale = hpFrameSize_ / 2.0f;
	hpFrame_.endScale = hpFrameSize_;
	//HPのバー
	hpBar_.startPosition = infoStartPos_;
	hpBar_.middlePosition = AddStarthalf(hpBarPos_);
	hpBar_.endPosition = hpBarPos_;
	hpBar_.startScale = Vector2();
	hpBar_.middleScale.x = (hpBarSize_.x * enHealthRate_) / 2.0f;
	hpBar_.middleScale.y = (hpBarSize_.y) / 2.0f;
	hpBar_.endScale.x = hpBarSize_.x * enHealthRate_;
	hpBar_.endScale.y = hpBarSize_.y;
}

void InfomationUI::SetPosSizeReverse(){
	//吹き出し
	speech_Bubble_.startPosition = speech_BubblePos_;
	speech_Bubble_.endPosition = infoStartPos_ ;
	speech_Bubble_.startScale = speech_BubbleSize_;
	speech_Bubble_.endScale = Vector2();

	//敵の画像
	enemyTex_.startPosition = enemyTexPos_;
	enemyTex_.endPosition = infoStartPos_;
	enemyTex_.startScale = enemyTexSize_;
	enemyTex_.endScale = Vector2();

	//HPのフレーム
	hpFrame_.startPosition = hpFramePos_ ;
	hpFrame_.endPosition = infoStartPos_;
	hpFrame_.startScale = hpFrameSize_;
	hpFrame_.endScale = Vector2();
	//HPのバー
	hpBar_.startPosition = hpBarPos_ ;
	hpBar_.endPosition = infoStartPos_;
	hpBar_.startScale.x = hpBarSize_.x * enHealthRate_;
	hpBar_.startScale.y = hpBarSize_.y;
	hpBar_.endScale = Vector2();
}

void InfomationUI::DebugDraw() {
	ImGui::Begin("お知らせUI");
	if (ImGui::Button("イージングスタート")){

		InfoEase();
	}
	ImGui::End();
}

void InfomationUI::NumberDrawControl() {




}

Vector2 InfomationUI::AddStarthalf(Vector2 end) const{
	Vector2 result{};

	result = (infoStartPos_ + end) / 2.0f;

	return result;
}
