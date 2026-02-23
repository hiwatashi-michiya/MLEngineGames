#include "BackgroundMove.h"
#include "Utility/Easing.h"
#include "FrameTracker.h"
#include "MLMath.h"

using namespace MLEngine::Utility;
using namespace MLEngine::Math;
using namespace MLEngine::Core;

BackgroundMove::BackgroundMove()
{
}

BackgroundMove::~BackgroundMove()
{
}

void BackgroundMove::Initialize() {

	cloudTextureName_ = "./Resources/Texture/ingame_back_cloud.png";

#ifdef CLIENT_BUILD
	bgState_.isEnteredClient = false;
	bgState_.isEnteredServer = false; 
#else
	bgState_.isEnteredClient = false;
	bgState_.isEnteredServer = true;
#endif // CLIENT_BUILD

	defaultCloudOffset_ = { 0.0f,5.0f,12.0f };

	cloudOffset_[0] = { 0.0f,0.0f,0.0f };
	cloudOffset_[1] = { 6.0f,2.0f,10.0f };
	cloudOffset_[2] = { 10.0f,1.0f,0.0f };
	cloudOffset_[3] = { 15.0f,3.0f,20.0f };
	cloudOffset_[4] = { 23.0f,0.0f,0.0f };

	currentCloudMoveTime_ = 0.0f;

	for (int32_t i = 0; i < kMaxClouds_; i++) {

		clouds_[i].Initialize(cloudTextureName_, 1);
		clouds_[i].transform.translate = defaultCloudOffset_ + cloudOffset_[i];
		clouds_[i].transform.translate.x += cloudPositionX_;
		clouds_[i].transform.scale = { 2.0f,2.0f,2.0f };
		clouds_[i].isActive = false;

	}


}

void BackgroundMove::Update() {

	SyncFromNetwork();

	//managerを介してクライアントに送る
	NetworkManager::BackgroundPacket bgPacket{};
	bgPacket.head.type = 6;
	bgPacket.head.size = sizeof(NetworkManager::BackgroundPacket);
	bgPacket.state = bgState_;
	NetworkManager::GetInstance().Send(bgPacket);

#ifdef CLIENT_BUILD

	if (bgState_.isEnteredClient and not isStartMoveClouds_) {
		cloudPositionX_ = startCloudPositionX_;
		isStartMoveClouds_ = true;
	}

	if (isStartMoveClouds_) {

		currentCloudMoveTime_ += FrameTracker::GetInstance()->GetDeltaTimeF();

		cloudPositionX_ = Lerp(startCloudPositionX_, endCloudPositionX_, currentCloudMoveTime_ / maxCloudMoveTime_);

		if (cloudPositionX_ <= moveNextScreenX_ and not bgState_.isEnteredServer) {
			bgState_.isEnteredServer = true;
		}

		if (currentCloudMoveTime_ >= maxCloudMoveTime_) {
			bgState_.isEnteredClient = false;
			isStartMoveClouds_ = false;
			currentCloudMoveTime_ = 0.0f;
		}

		for (int32_t i = 0; i < kMaxClouds_; i++) {
			clouds_[i].isActive = true;
			clouds_[i].transform.translate = defaultCloudOffset_ + cloudOffset_[i];
			clouds_[i].transform.translate.x += cloudPositionX_;
		}

	}
	else {

		for (int32_t i = 0; i < kMaxClouds_; i++) {

			clouds_[i].isActive = false;

		}

	}

#else

	if (bgState_.isEnteredServer and not isStartMoveClouds_) {
		cloudPositionX_ = startCloudPositionX_;
		isStartMoveClouds_ = true;
	}

	if (isStartMoveClouds_) {

		currentCloudMoveTime_ += FrameTracker::GetInstance()->GetDeltaTimeF();

		cloudPositionX_ = Lerp(startCloudPositionX_, endCloudPositionX_, currentCloudMoveTime_ / maxCloudMoveTime_);

		if (cloudPositionX_ <= moveNextScreenX_ and not bgState_.isEnteredClient) {
			bgState_.isEnteredClient = true;
		}

		if (currentCloudMoveTime_ >= maxCloudMoveTime_) {
			bgState_.isEnteredServer = false;
			isStartMoveClouds_ = false;
			currentCloudMoveTime_ = 0.0f;
		}

		for (int32_t i = 0; i < kMaxClouds_; i++) {
			clouds_[i].isActive = true;
			clouds_[i].transform.translate = defaultCloudOffset_ + cloudOffset_[i];
			clouds_[i].transform.translate.x += cloudPositionX_;
		}

	}
	else {

		for (int32_t i = 0; i < kMaxClouds_; i++) {

			clouds_[i].isActive = false;

		}

	}

#endif // CLIENT_BUILD


}

void BackgroundMove::SyncFromNetwork() {

	NetworkManager::BackgroundState netState{};

	if (NetworkManager::GetInstance().GetLatestBGState(netState)) {

		bgState_.isEnteredClient = netState.isEnteredClient;
		bgState_.isEnteredServer = netState.isEnteredServer;

	}

}

void BackgroundMove::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Background");
	ImGui::DragFloat("moveNextScreen", &moveNextScreenX_);
	ImGui::DragFloat("startCloudPosition", &startCloudPositionX_);
	ImGui::DragFloat("endCloudPosition", &endCloudPositionX_);
	ImGui::DragFloat("maxCloudMoveTime", &maxCloudMoveTime_, 0.5f, 1.0f, 1000.0f);
	ImGui::DragFloat3("DefaultCloudOffset", &defaultCloudOffset_.x);
	
	if (ImGui::Checkbox("isStartMoveClouds", &isStartMoveClouds_)) {
		cloudPositionX_ = startCloudPositionX_;
	}
	
	for (int32_t i = 0; i < kMaxClouds_; i++) {

		std::string name = "cloudOffset" + std::to_string(i);

		ImGui::DragFloat3(name.c_str(), &cloudOffset_[i].x);

	}

	ImGui::Checkbox("isEnteredClient", &bgState_.isEnteredClient);

	ImGui::End();

#endif // _DEBUG


}
