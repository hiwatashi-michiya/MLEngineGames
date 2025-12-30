#include "DebugScene.h"
#include "ImguiManager.h"
#include"Externals/imgui/imgui.h"
#include"SceneManager.h"
#include"PlayScene.h"
#include "InstancingModel.h"
#include "FrameTracker.h"

using namespace MLEngine::Math;
using namespace MLEngine::Resource;
using namespace MLEngine::Object::Collision;

DebugScene::DebugScene(){
	
	input_ = MLEngine::Input::Manager::GetInstance();
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{
	//お試しプッシュ
	vController_ = &VirtualController::GetInstance();

#pragma region
	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-30.0f };
	debugCamera_.Initialize();

	tex_.Load("./Resources/white.png");

	sprite3D_.Initialize("./Resources/texture/player_back.png", 1);
	model_.Initialize("./Resources/EngineResources/testObjects/axis.obj");
	model2_.Initialize("./Resources/model/block/glassBlock.obj");
	model3_.Initialize("./Resources/model/block/glassBlock.obj");
	model3_.SetTexture("./Resources/EngineResources/paperMask.png");
	particle_.reset(Particle3D::Create("./Resources/model/plane/plane.obj", 32));
	sprite_.reset(Sprite2D::Create(tex_, { 200.0f,200.0f }, { 0.0f,1.0f,0.0f,1.0f }));
	sprite_->size = { 200.0f,200.0f };
	//読み込み("./Resources/audio/"以降のパスでOK)
	se1_.Load("SE/test.mp3");

	box_.SetCollisionAttribute(0x00000002);
	//当たった瞬間の呼び出し関数
	box_.SetEnterFunction([this](Collider* collider) {lineBox_.SetColor({ 1.0f,0.0f,0.0f,1.0f }); });
	//当たっている時の呼び出し関数
	box_.SetStayFunction([this](Collider* collider) {lineBox_.SetColor({ 0.0f,1.0f,0.0f,1.0f }); });
	//離れた瞬間の呼び出し関数
	box_.SetExitFunction([this](Collider* collider) {lineBox_.SetColor({ 1.0f,1.0f,1.0f,1.0f }); });

	lineBox_.SetOBB(&box_.collider_);
	sphere_.SetCollisionAttribute(0x00000001);
	lineSphere_.SetSphere(&sphere_.collider_);
#pragma endregion

#pragma region
	joyconInput = std::make_unique<Joycon>();
	joyconInput->Init();
#pragma endregion ジョイコン
#pragma region
	arduinoSerialReader = std::make_unique<ArduinoSerialReader>();
	arduinoSerialReader->Init();
#pragma endregion アルディーノ

	dLight_.cbData->direction = MLEngine::Math::Normalize(dLight_.cbData->direction);

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{
	joyconInput->Update();
	arduinoSerialReader->Update();
	{

#ifdef _DEBUG

		ImGui::Begin("テスト");

		MLEngine::Core::FrameTracker::GetInstance()->Debug();
		ImGui::Text("isPlayingSE: %d", se1_.IsPlaying());

		if (ImGui::TreeNode("Box")) {
			box_.Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere")) {
			sphere_.Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sprite3D")) {
			sprite3D_.Debug();
			ImGui::DragInt("toon shading", &sprite3D_.materialData.enableToonshading, 0.1f, 0, 1);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Transform2")) {
			transform2_.Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Transform3")) {
			transform3_.Debug();
			ImGui::TreePop();
		}

		if (ImGui::Checkbox("debug Camera", &isDebugCamera_)) {

			if (isDebugCamera_) {
				sceneManager_->SetMainCamera(debugCamera_.GetCamera());
			}
			else {
				sceneManager_->SetMainCamera(&camera_);
			}

		}

		if (ImGui::DragInt("use normal map", &model2_.materialData.enableNormalMap, 0.1f,0, 1)) {

		}

		if (ImGui::Checkbox("show model", &sprite3D_.isActive)) {

		}

		if (ImGui::Checkbox("show sprite", &sprite_->isActive)) {

		}

		if (ImGui::Checkbox("show particle", &particle_->isActive)) {

		}

		if (ImGui::Checkbox("show box", &showBox_)) {
			lineBox_.SetIsActive(showBox_);
		}

		if (ImGui::Checkbox("show sphere", &showSphere_)) {
			lineSphere_.SetIsActive(showSphere_);
		}

		ImGui::End();
	Quaternion hoge;
	modelRot_ *= ConvertFromEuler(joyconInput->GetVecRotate());

	test += joyconInput->GetVecRotate() * (180/std::numbers::pi);
	ImGui::Begin("Gyro");
	ImGui::Text("DeX:%f", test.x);
	ImGui::Text("DeY:%f", test.y);
	ImGui::Text("DeZ:%f", test.z);
	ImGui::End();

#endif // _DEBUG

	}

	if (input_->GetKeyboard()->Trigger(DIK_Q)) {
		//SE再生
		se1_.Play(0.5f, false);
	}
	if (joyconInput->IsPush(UP)) {
		modelRot_ = modelRot_.IdentityQuaternion();
		test = { 0.0f,0.0f,0.0f };
	}

	if (input_->GetKeyboard()->Trigger(DIK_W)) {
		//一時停止
		se1_.Pause();
	}

	if (input_->GetKeyboard()->Trigger(DIK_E)) {
		//再開
		se1_.ReStart();
	}

	if (input_->GetKeyboard()->Trigger(DIK_R)) {
		//停止
		se1_.Stop();
	}

	if (input_->GetKeyboard()->Trigger(DIK_1)) {
		//ボリューム調整
		se1_.SetVolume(0.1f);
	}

	if (input_->GetKeyboard()->Trigger(DIK_SPACE)) {
		sceneManager_->ChangeScene(new PlayScene());
	}

	//Particle3D
	{

		for (int32_t i = 0; i < 32; i++) {
			//ビルボードフラグ
			particle_->isBillboard_ = true;
			//モデル一つ一つのアクティブフラグ
			particle_->particleData[i].isActive = true;
			//トランスフォーム
			particle_->particleData[i].transform.translate = { i * 0.1f, 0.0f,0.0f };
			particle_->particleData[i].transform.scale = { 1.0f,1.0f,1.0f };
			particle_->particleData[i].transform.rotateQuaternion = MLEngine::Math::IdentityQuaternion();
			//色
			particle_->particleData[i].color = { 1.0f, i / 32.0f, 1.0f, 1.0f };
		}

	}

	//トランスフォーム
	Matrix4x4 result;
	modelRot_ *= ConvertFromEuler(joyconInput->GetVecRotate());

	if (joyconInput->IsPush(UP)) {
		modelRot_ = modelRot_.IdentityQuaternion();
		test = { 0.0f,0.0f,0.0f };
	}

	//ジョイコンの回転情報からアフィン行列を作成
	result = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), modelRot_, Vector3(0.0f, 0.0f, 0.0f));
	//モデルに行列をセット
	model_.SetWorldMatrix(result);

	if (vController_->Decide()) {
		sceneManager_->ChangeScene(new PlayScene());
	}
	if (input_->GetKeyboard()->Trigger(DIK_SPACE)) {
		sceneManager_->ChangeScene(new PlayScene());
	}

	if (isDebugCamera_) {
		debugCamera_.Update();
	}
	else {
		camera_.Update();
	}
	camera_.Update();

		lineBox_.Update();
		lineSphere_.Update();

	transform2_.UpdateMatrix();
	transform3_.UpdateMatrix();

	sprite3D_.UpdateAnimation();
	model2_.SetWorldMatrix(transform2_.worldMatrix);
	model3_.SetWorldMatrix(transform3_.worldMatrix);

	lineBox_.Update();
	lineSphere_.Update();
}

void DebugScene::Draw()
{
	arduinoSerialReader->Draw();
	//model_.GetInstancingModel().Draw(&camera_);

	//particle_->Draw(&camera_);

	//lineSphere_.Draw(&camera_);

	//lineBox_.Draw(&camera_);

}


void DebugScene::DrawImgui() {
#ifdef _DEBUG


	ImGui::Begin("お試し");

	ImGui::Text("テスト");

	ImGui::End();

#endif // _DEBUG

}
