#include "UI.h"
#include "FrameTracker.h"

using namespace MLEngine::Math;
using namespace MLEngine::Resource;

UI::UI()
{
}

UI::~UI()
{
}

void UI::Initialize(Texture texture, Vector2 pos, Vector4 col) {

	sprite_.reset(Sprite2D::Create(texture, pos, col));
	currentEasingTime_ = 0.0f;
	isStartEasing_ = false;
	isEndEasing_ = false;

}

void UI::Stop() {



}

void UI::Update() {



	//イージングをしない場合は開始座標に画像を合わせる。この後の更新は不要
	if (not isStartEasing_) {

		if (isEndEasing_) {
			sprite_->position = endPosition;
			sprite_->size = { sprite_->GetDefaultSize().x * endScale.x, sprite_->GetDefaultSize().y * endScale.y };
		}
		else {
			sprite_->position = startPosition;
			sprite_->size = { sprite_->GetDefaultSize().x * startScale.x, sprite_->GetDefaultSize().y * startScale.y };
		}

		return;
	}

	//総イージング時間に達するまでカウントする
	if (currentEasingTime_ < easingTime) {

		Vector2 easingScale{};
		float easingT;

		currentEasingTime_ += 1.0f *  MLEngine::Core::FrameTracker::GetInstance()->GetDeltaTimeF();

		//最初から中間地点までのイージング
		if (currentEasingTime_ < startToMiddleTime) {

			easingT = currentEasingTime_ / startToMiddleTime;

			sprite_->position = Lerp(startPosition, middlePosition, easingT);
			easingScale = Lerp(startScale, middleScale, easingT);

			sprite_->size = { sprite_->GetDefaultSize().x * easingScale.x, sprite_->GetDefaultSize().y * easingScale.y };

		}
		//中間地点での待機
		else if (currentEasingTime_ < startToMiddleTime + stayMiddleTime) {

			sprite_->position = middlePosition;
			easingScale =middleScale;

			sprite_->size = { sprite_->GetDefaultSize().x * easingScale.x, sprite_->GetDefaultSize().y * easingScale.y };

		}
		//中間から最後までのイージング
		else if (currentEasingTime_ < easingTime) {

			easingT = (currentEasingTime_ - startToMiddleTime - stayMiddleTime) / (easingTime - startToMiddleTime - stayMiddleTime);

			sprite_->position = Lerp(middlePosition, endPosition, easingT);
			easingScale = Lerp(middleScale, endScale, easingT);

			sprite_->size = { sprite_->GetDefaultSize().x * easingScale.x, sprite_->GetDefaultSize().y * easingScale.y };

		}

		if (currentEasingTime_ >= easingTime) {
			isStartEasing_ = false;
			isEndEasing_ = true;
		}

	}

}
