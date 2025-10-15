#include "Particle.h"
#include <random>
#include "Rand.h"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include "ParticleEditor.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

Particle::Particle()
{
	particle_.reset(Particle3D::Create("./Resources/model/plane/plane.obj", kMaxParticle_));
	particle_->SetInstanceCount(instanceCount_);
}

Particle::~Particle()
{
}

void Particle::Initialize() {

}

void Particle::Update() {

	//パーティクル全体の生存時間
	if (particleLifeTime_ > 0 and not isLoop_) {
		particleLifeTime_--;
	}

	for (int32_t i = 0; i < instanceCount_; i++) {
		//非アクティブのパーティクルを出現させる
		if ((isLoop_ or particleLifeTime_ > 0) and not particle_->isActive_[i]) {

			particle_->transforms_[i]->translate_ = RandomVector3(spawnPoint_ + minSpawnPoint_, spawnPoint_ + maxSpawnPoint_);
			//追従対象がいたら対象の座標分加算
			if (followPoint_) {
				particle_->transforms_[i]->translate_ += *followPoint_;
			}

			particle_->transforms_[i]->rotateQuaternion_ = ConvertFromEuler(RandomVector3(-3.14f, 3.14f));
			particle_->transforms_[i]->scale_ = Vector3{ 1.0f,1.0f,1.0f } * RandomFloat(minScale_, maxScale_);

			particle_->velocities_[i] = RandomVector3(minSpeed_, maxSpeed_);
			particle_->lifeTimes_[i] = RandomInt(minLifeTime_, maxLifeTime_);
			particle_->colors_[i] = RandomVector4(minStartColor_, maxStartColor_);

			particle_->isActive_[i] = true;

			break;

		}

	}

	for (int32_t i = 0; i < instanceCount_; i++) {

		//アクティブ状態のパーティクルを更新
		if (particle_->isActive_[i]) {

			//ターゲットがある場合、ターゲットに向かう
			if (targetPoint_) {

				particle_->transforms_[i]->translate_ = Lerp(particle_->transforms_[i]->translate_, *targetPoint_, float(maxLifeTime_ + 1 - particle_->lifeTimes_[i]) / float(maxLifeTime_ + 1));
				particle_->transforms_[i]->scale_ += {changeScale_, changeScale_, changeScale_};

			}
			//ターゲットが無い場合は速度に合わせる
			else {

				particle_->transforms_[i]->translate_ += particle_->velocities_[i];
				particle_->transforms_[i]->scale_ += {changeScale_, changeScale_, changeScale_};
				/*particle_->colors_[i] = Lerp(endColor_, minStartColor_, float(particle_->transforms_[i]->scale_.x) / maxScale_);*/

				particle_->velocities_[i] += changeSpeed_;

			}

			if (particle_->transforms_[i]->scale_.x <= 0.0f or 
				particle_->transforms_[i]->scale_.y <= 0.0f or 
				particle_->transforms_[i]->scale_.z <= 0.0f or
				particle_->lifeTimes_[i] <= 0) {
				particle_->transforms_[i]->scale_ = Vector3::Zero();

				particle_->isActive_[i] = false;

			}

			particle_->lifeTimes_[i]--;

		}

	}

}

void Particle::Draw(Camera* camera) {

	particle_->Draw(camera);

}

void Particle::SetDefault()
{
	//ターゲット外す
	followPoint_ = nullptr;
	targetPoint_ = nullptr;
	//その他の情報をリセット
	particleLifeTime_ = 0;

	//名前
	name_ = "";
	//出現時の色範囲
	minStartColor_ = { 1.0f,1.0f,1.0f,1.0f };
	maxStartColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//消滅時の色
	endColor_ = { 1.0f,1.0f,1.0f,1.0f };
	//速度最低値
	minSpeed_ = { 0.0f,0.0f,0.0f };
	//速度最大値
	maxSpeed_ = { 0.0f,0.0f,0.0f };
	//スポーン地点
	spawnPoint_ = { 0.0f,0.0f,0.0f };
	//スポーン範囲最小
	minSpawnPoint_ = { 0.0f,0.0f,0.0f };
	//スポーン範囲最大
	maxSpawnPoint_ = { 0.0f,0.0f,0.0f };
	//スピードの変化量(一定)
	changeSpeed_ = { 0.0f,0.0f,0.0f };
	//回転変化量
	changeRotate_ = { 0.0f,0.0f,0.0f };
	//スケール変化量(一定)
	changeScale_ = 0.0f;
	//最小スケール
	minScale_ = 0.1f;
	//最大スケール
	maxScale_ = 1.0f;
	//最小生存時間
	minLifeTime_ = 60;
	//最大生存時間
	maxLifeTime_ = 60;
	//インスタンス数
	instanceCount_ = 32;
	//パーティクル全体の生存時間
	particleLifeTime_ = 120;
	//パーティクル全体の最大生存時間
	maxParticleLifeTime_ = 120;
	//アクティブ状態かどうか
	isActive_ = false;
	//ループするかどうか
	isLoop_ = false;

	//全てを初期値にリセット
	for (int32_t i = 0; i < kMaxParticle_; i++) {
		particle_->transforms_[i]->translate_ = Vector3::Zero();
		particle_->transforms_[i]->rotateQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };
		particle_->transforms_[i]->scale_ = Vector3::Zero();

		particle_->velocities_[i] = Vector3::Zero();
		particle_->lifeTimes_[i] = 0;
		particle_->colors_[i] = { 1.0f,1.0f,1.0f,1.0f };
		particle_->isActive_[i] = false;
	}

}

bool Particle::IsEnd() {

	if (particleLifeTime_ <= 0 and not particle_->IsAnyActive() and not isLoop_) {
		return true;
	}

	return false;

}

void Particle::Load(const std::string& filePath)
{

	//読み込み用ファイルストリーム
	std::ifstream ifs;
	//ファイルを読み込み用に開く
	ifs.open(filePath);

	//ファイルオープン失敗したら表示
	if (ifs.fail()) {
		MessageBox(nullptr, L"指定したファイルは存在しません。", L"Particle - Load", 0);
		return;
	}

	nlohmann::json root;

	//json文字列からjsonのデータ構造に展開
	ifs >> root;
	//ファイルを閉じる
	ifs.close();
	//グループを検索
	nlohmann::json::iterator itGroup = root.find(ParticleEditor::kParticleName_);
	//未登録チェック
	if (itGroup == root.end()) {
		MessageBox(nullptr, L"ファイルの構造が正しくありません。", L"Particle - Load", 0);
	}

	//保険
	assert(itGroup != root.end());

	//各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		//アイテム名を取得
		const std::string& itemName = itItem.key();

		//グループを検索
		nlohmann::json::iterator itObject = itGroup->find(itemName);

		//未登録チェック
		if (itObject == itGroup->end()) {
			MessageBox(nullptr, L"ファイルの構造が正しくありません。", L"Particle - Load", 0);
		}

		//保険
		assert(itObject != itGroup->end());

		//アイテム名がパーティクルデータだった場合、登録
		if (itemName == "ParticleData") {

			//パーティクルデータについて
			for (nlohmann::json::iterator itParticleData = itObject->begin(); itParticleData != itObject->end(); ++itParticleData) {

				//アイテム名を取得
				const std::string& dataName = itParticleData.key();

				//グループを検索
				nlohmann::json::iterator itData = itObject->find(dataName);

				//未登録チェック
				if (itData == itObject->end()) {
					MessageBox(nullptr, L"ファイルの構造が正しくありません。", L"Particle - Load", 0);
				}

				//保険
				assert(itData != itObject->end());

				//Speedを登録
				if (dataName == "Speed") {

					for (nlohmann::json::iterator itDetailData = itData->begin(); itDetailData != itData->end(); ++itDetailData) {

						//アイテム名を取得
						const std::string& detailName = itDetailData.key();

						if (detailName == "Min") {
							minSpeed_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2) };
						}
						if (detailName == "Max") {
							maxSpeed_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2) };
						}
						if (detailName == "Delta") {
							changeSpeed_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2) };
						}

					}

				}
				//SpawnPointを登録
				else if (dataName == "SpawnPoint") {

					for (nlohmann::json::iterator itDetailData = itData->begin(); itDetailData != itData->end(); ++itDetailData) {

						//アイテム名を取得
						const std::string& detailName = itDetailData.key();

						if (detailName == "Min") {
							minSpawnPoint_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2) };
						}
						if (detailName == "Max") {
							maxSpawnPoint_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2) };
						}

					}

				}
				else if (dataName == "Scale") {

					for (nlohmann::json::iterator itDetailData = itData->begin(); itDetailData != itData->end(); ++itDetailData) {

						//アイテム名を取得
						const std::string& detailName = itDetailData.key();

						if (detailName == "Min") {
							minScale_ = itDetailData->get<float>();
						}
						if (detailName == "Max") {
							maxScale_ = itDetailData->get<float>();
						}
						if (detailName == "Delta") {
							changeScale_ = itDetailData->get<float>();
						}

					}

				}
				else if (dataName == "LifeTime") {

					for (nlohmann::json::iterator itDetailData = itData->begin(); itDetailData != itData->end(); ++itDetailData) {

						//アイテム名を取得
						const std::string& detailName = itDetailData.key();

						if (detailName == "Min") {
							minLifeTime_ = itDetailData->get<int32_t>();
						}
						if (detailName == "Max") {
							maxLifeTime_ = itDetailData->get<int32_t>();
						}
						if (detailName == "All") {
							particleLifeTime_ = itDetailData->get<int32_t>();
							maxParticleLifeTime_ = itDetailData->get<int32_t>();
						}

					}

				}
				else if (dataName == "Color") {

					for (nlohmann::json::iterator itDetailData = itData->begin(); itDetailData != itData->end(); ++itDetailData) {

						//アイテム名を取得
						const std::string& detailName = itDetailData.key();

						if (detailName == "Start") {
							minStartColor_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2),itDetailData->at(3) };
						}
						if (detailName == "MinStart") {
							minStartColor_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2),itDetailData->at(3) };
						}
						if (detailName == "MaxStart") {
							maxStartColor_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2),itDetailData->at(3) };
						}
						if (detailName == "End") {
							endColor_ = { itDetailData->at(0),itDetailData->at(1) ,itDetailData->at(2),itDetailData->at(3) };
						}

					}

				}
				else if (dataName == "Name") {
					name_ = itData.value();
				}
				else if (dataName == "Instance") {
					SetInstanceCount(itData->get<int32_t>());
				}
				else if (dataName == "IsLoop") {
					isLoop_ = itData->get<bool>();
				}
				else if (dataName == "Texture") {
					particle_->SetTexture(itData.value());
				}

			}

		}

	}

}
