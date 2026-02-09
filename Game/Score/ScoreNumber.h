#pragma once
#include<Manager/PlayerManager.h>
#include <UI/UI.h>

/*スコアのUIを管理するクラス*/
class ScoreNumber {
public:
	ScoreNumber();

	void Initialize();

	void Update();

	void ScoreEase() {
		scoreNumTen_.ReStart();
		scoreNumOne_.ReStart();
	}

	void ComboEase() {
		comboNumTen_.ReStart();
		comboNumOne_.ReStart();
		scoreCombo_.ReStart();
	}

private:
	void GlobalSetValues();

	void GlobalGetValues();

	//デバック描画(ラインとか)
	void DebugDraw();

	void ScoreCulc();

	void ComboCulc();

private:

	MLEngine::Resource::Texture texture_;
	//スコアの裏のボード
	std::unique_ptr<MLEngine::Resource::Sprite2D> scoreBoard_;

	MLEngine::Math::Vector2 scoreBoardPos_{};

	MLEngine::Math::Vector2 scoreBoardSize_{};
	//コンボ表示
	UI scoreCombo_;

	MLEngine::Math::Vector2 scoreComboPos_{};

	MLEngine::Math::Vector2 scoreComboSize_{};
	//スコアの十の位
	UI scoreNumTen_;

	MLEngine::Math::Vector2 scoreNumTenPos_{};

	MLEngine::Math::Vector2 scoreNumTenSize_{};
	//スコアの一の位
	UI scoreNumOne_;

	MLEngine::Math::Vector2 scoreNumOnePos_{};

	MLEngine::Math::Vector2 scoreNumOneSize_{};


	//コンボの十の位
	UI comboNumTen_;

	MLEngine::Math::Vector2 comboNumTenPos_{};

	MLEngine::Math::Vector2 comboNumTenSize_{};
	//コンボの一の位
	UI comboNumOne_;

	MLEngine::Math::Vector2 comboNumOnePos_{};

	MLEngine::Math::Vector2 comboNumOneSize_{};

	//拡大率
	float magnification_ = 1.2f;

	//コンボなどの傾き
	float comboRotate_ = 0.0f;

	//スコアの十の位
	int scoreTen_ = 0;
	//スコアの一の位
	int scoreOne_ = 0;
	//コンボの十の位
	int comboTen_ = 0;
	//コンボの一の位
	int comboOne_ = 0;

	//基本色
	MLEngine::Math::Vector4 baseColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//コンボUIの色
	MLEngine::Math::Vector4 comboColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//スコアの数字の色
	MLEngine::Math::Vector4 scoreNumColor_ = { 0.1f,1.0f,0.1f,1.0f };
	//スコアの数字の色
	MLEngine::Math::Vector4 comboNumColor_ = { 0.0f,0.0f,0.0f,1.0f };

};

