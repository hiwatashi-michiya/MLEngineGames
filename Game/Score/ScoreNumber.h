#pragma once
#include<Manager/PlayerManager.h>

/*スコアのUIを管理するクラス*/
class ScoreNumber {
public:
	ScoreNumber();

	void Initialize();

	void Update();

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
	std::unique_ptr<MLEngine::Resource::Sprite2D> scoreCombo_;

	MLEngine::Math::Vector2 scoreComboPos_{};

	MLEngine::Math::Vector2 scoreComboSize_{};
	//スコアの十の位
	std::unique_ptr<MLEngine::Resource::Sprite2D> scoreNumTen_;

	MLEngine::Math::Vector2 scoreNumTenPos_{};

	MLEngine::Math::Vector2 scoreNumTenSize_{};
	//スコアの一の位
	std::unique_ptr<MLEngine::Resource::Sprite2D> scoreNumOne_;

	MLEngine::Math::Vector2 scoreNumOnePos_{};

	MLEngine::Math::Vector2 scoreNumOneSize_{};


	//コンボの十の位
	std::unique_ptr<MLEngine::Resource::Sprite2D> comboNumTen_;

	MLEngine::Math::Vector2 comboNumTenPos_{};

	MLEngine::Math::Vector2 comboNumTenSize_{};
	//コンボの一の位
	std::unique_ptr<MLEngine::Resource::Sprite2D> comboNumOne_;

	MLEngine::Math::Vector2 comboNumOnePos_{};

	MLEngine::Math::Vector2 comboNumOneSize_{};

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

