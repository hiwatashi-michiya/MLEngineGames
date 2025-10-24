#include "LineDrawer.h"
#include <cassert>
#include "Core/Render/ShaderManager.h"
#include "Core/Render/RootSignatureManager.h"
#include "Core/Render/PipelineManager.h"
#include "Buffer/BufferResource.h"
#include "Convert.h"
#include "RenderManager.h"
#include <cmath>
#include <numbers>
#include "SceneManager.h"
#include "WindowManager.h"

using namespace MLEngine::Resource;
using namespace MLEngine::Core::Render;
using namespace MLEngine::Core;
using namespace MLEngine::Math;
using namespace MLEngine::Object;

ID3D12Device* Line::device_ = nullptr;
ID3D12GraphicsCommandList* Line::commandList_ = nullptr;
ID3D12RootSignature* Line::rootSignature_ = nullptr;
ID3D12PipelineState* Line::pipelineState_ = nullptr;
#ifdef _DEBUG
bool Line::showCollisionLine_ = true;
#else
bool Line::showCollisionLine_ = false;
#endif // _DEBUG



void Line::Initialize(ID3D12Device* device) {

	assert(device);

	HRESULT hr;

	device_ = device;

	IDxcBlob* vsBlob = Shader::Manager::GetInstance()->CompileShader(L"./Resources/shaders/Line.VS.hlsl", Shader::Type::kVS, "VSLine");

	IDxcBlob* psBlob = Shader::Manager::GetInstance()->CompileShader(L"./Resources/shaders/Line.PS.hlsl", Shader::Type::kPS, "PSLine");

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "COLOR";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートパラメータ作成
	D3D12_ROOT_PARAMETER rootParameters[1]{};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	descriptionRootSignature.pParameters = rootParameters; //ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters); //ルートパラメータの長さ

	//シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	RootSignature::Manager::GetInstance()->CreateRootSignature(signatureBlob, "RootSignature3DLine");

	rootSignature_ = RootSignature::Manager::GetInstance()->GetRootSignature("RootSignature3DLine");

	//Blendstateの設定
	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//RasterizerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_; //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { vsBlob->GetBufferPointer(),
	vsBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),
	psBlob->GetBufferSize() }; //PixelShader
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; //RasterizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ、線
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	//どのように画面に色を打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	//Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;
	//書き込み
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	//近ければ描画
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//通常
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState

	Pipeline::Manager::GetInstance()->CreatePipeLine(graphicsPipelineStateDesc, "Pipeline3DLine");

	pipelineState_ = Pipeline::Manager::GetInstance()->GetPipeline("Pipeline3DLine");

}

void Line::PreDraw(ID3D12GraphicsCommandList* commandList) {

	assert(commandList_ == nullptr);

	commandList_ = commandList;

	//PSO設定
	commandList_->SetPipelineState(pipelineState_);

	commandList_->SetGraphicsRootSignature(rootSignature_);

	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

}

void Line::PostDraw() {

	commandList_ = nullptr;

}

Line::Line()
{

	start_ = Vector3::Zero();
	end_ = Vector3::Identity();

	color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


	{

		vertexBuff_ = CreateBufferResource(device_, sizeof(LineVertexData) * kVertexNum_);

		//頂点バッファビュー設定
		vbView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();
		vbView_.SizeInBytes = UINT(sizeof(LineVertexData) * kVertexNum_);
		vbView_.StrideInBytes = sizeof(LineVertexData);

		vertexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertexMap_));

		vertexMap_[0].position = Vector4(start_.x, start_.y, start_.z, 1.0f);
		vertexMap_[0].color = color_;

		vertexMap_[1].position = Vector4(end_.x, end_.y, end_.z, 1.0f);
		vertexMap_[1].color = color_;

		vertexBuff_->Unmap(0, nullptr);

	}


	{

		wvpBuff_ = CreateBufferResource(device_, sizeof(Matrix4x4));

		wvpBuff_->Map(0, nullptr, reinterpret_cast<void**>(&wvpMatrix_));

		*wvpMatrix_ = MakeIdentity4x4();

		wvpBuff_->Unmap(0, nullptr);

	}
}

Line::~Line()
{
}

void Line::Draw(Camera* camera, const Matrix4x4& matrix) {

	vertexMap_[0].position = Vector4(start_.x, start_.y, start_.z, 1.0f);
	vertexMap_[0].color = color_;

	vertexMap_[1].position = Vector4(end_.x, end_.y, end_.z, 1.0f);
	vertexMap_[1].color = color_;

	*wvpMatrix_ = matrix * camera->matViewProjection_;

	//当たり判定ラインじゃない場合か、当たり判定ライン表示がONの場合に描画
	if (not isCollisionLine_ or showCollisionLine_) {
		Render::Manager::GetInstance()->AddLine(this);
	}

}

void Line::Render()
{

	commandList_->IASetVertexBuffers(0, 1, &vbView_);

	commandList_->SetGraphicsRootConstantBufferView(0, wvpBuff_->GetGPUVirtualAddress());

	commandList_->DrawInstanced(kVertexNum_, 1, 0, 0);

}

LineBox::LineBox()
{

	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i] = std::make_unique<Line>();
	}

}

LineBox::~LineBox()
{
}

void LineBox::SetIsCollisionLine(bool flag)
{
	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i]->SetIsCollisionLine(flag);
	}
}

void LineBox::Update() {

	Vector3 point[8]{};

	Matrix4x4 rotateMatrix = MakeIdentity4x4();

	//OBBのポインタがある場合のみ更新
	if (obb_) {

		rotateMatrix = {
	   {
		   {obb_->orientations[0].x,obb_->orientations[0].y,obb_->orientations[0].z,0.0f},
		   {obb_->orientations[1].x,obb_->orientations[1].y,obb_->orientations[1].z,0.0f},
		   {obb_->orientations[2].x,obb_->orientations[2].y,obb_->orientations[2].z,0.0f},
		   {0.0f,0.0f,0.0f,1.0f}
	   }
		};

		//左下前
		point[0] = CoordTransform(Vector3{ -obb_->size.x, -obb_->size.y, -obb_->size.z }, rotateMatrix);
		//右下前
		point[1] = CoordTransform(Vector3{ obb_->size.x, -obb_->size.y, -obb_->size.z }, rotateMatrix);
		//左上前
		point[2] = CoordTransform(Vector3{ -obb_->size.x, obb_->size.y, -obb_->size.z }, rotateMatrix);
		//右上前
		point[3] = CoordTransform(Vector3{ obb_->size.x, obb_->size.y, -obb_->size.z }, rotateMatrix);
		//左下奥
		point[4] = CoordTransform(Vector3{ -obb_->size.x, -obb_->size.y, obb_->size.z }, rotateMatrix);
		//右下奥
		point[5] = CoordTransform(Vector3{ obb_->size.x, -obb_->size.y, obb_->size.z }, rotateMatrix);
		//左上奥
		point[6] = CoordTransform(Vector3{ -obb_->size.x, obb_->size.y, obb_->size.z }, rotateMatrix);
		//右上奥
		point[7] = CoordTransform(Vector3{ obb_->size.x, obb_->size.y, obb_->size.z }, rotateMatrix);

		for (int32_t i = 0; i < 8; i++) {
			point[i] += obb_->center;
		}

		lines_[0]->start_ = point[0];
		lines_[0]->end_ = point[1];
		lines_[1]->start_ = point[0];
		lines_[1]->end_ = point[2];
		lines_[2]->start_ = point[2];
		lines_[2]->end_ = point[3];
		lines_[3]->start_ = point[1];
		lines_[3]->end_ = point[3];
		lines_[4]->start_ = point[0];
		lines_[4]->end_ = point[4];
		lines_[5]->start_ = point[1];
		lines_[5]->end_ = point[5];
		lines_[6]->start_ = point[2];
		lines_[6]->end_ = point[6];
		lines_[7]->start_ = point[3];
		lines_[7]->end_ = point[7];
		lines_[8]->start_ = point[4];
		lines_[8]->end_ = point[5];
		lines_[9]->start_ = point[4];
		lines_[9]->end_ = point[6];
		lines_[10]->start_ = point[6];
		lines_[10]->end_ = point[7];
		lines_[11]->start_ = point[5];
		lines_[11]->end_ = point[7];

	}

}

void LineBox::SetColor(Vector4 color)
{

	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i]->color_ = color;
	}

}

void LineBox::Draw(Camera* camera, const Matrix4x4& matrix) {

#ifdef _DEBUG

	if (obb_) {

		for (int32_t i = 0; i < kMaxLine_; i++) {
			lines_[i]->Draw(camera, matrix);
		}

	}

#endif // _DEBUG

}

LineSphere::LineSphere()
{

	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i] = std::make_unique<Line>();
	}

}

LineSphere::~LineSphere()
{
}

void LineSphere::SetIsCollisionLine(bool flag)
{
	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i]->SetIsCollisionLine(flag);
	}
}

void LineSphere::Update() {

	/*Camera* camera = MLEngine::Scene::Manager::GetInstance()->GetMainCamera();

	Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(Window::Manager::GetInstance()->GetClientWidth()),
		float(Window::Manager::GetInstance()->GetClientHeight()), 0.0f, 1.0f);*/

	if (sphere_) {

		const float  kLonEvery = float(2 * std::numbers::pi / kSubdivision_); //経度分割1つ分の角度
		const float kLatEvery = float(2 * std::numbers::pi / kSubdivision_); //緯度分割1つ分の角度
		//緯度の方向に分割　-π/2 ～ π/2
		for (uint32_t latIndex = 0; latIndex < kSubdivision_; ++latIndex) {
			float lat = -(float(std::numbers::pi) / 2.0f) + kLatEvery * latIndex; //現在の緯度
			//経度の方向に分割　-π/2 ～ π/2
			for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex) {
				float lon = lonIndex * kLonEvery;//現在の経度
				float thetaD = float(std::numbers::pi / kSubdivision_);
				float phiD = float(2 * std::numbers::pi / kSubdivision_);
				//world座標系でのa,b,cを求める
				Vector3 a, b, c;
				a = { sphere_->radius * cosf(lat) * cosf(lon) + sphere_->center.x,
					sphere_->radius * sinf(lat) + sphere_->center.y,
					sphere_->radius * cosf(lat) * sinf(lon) + sphere_->center.z };

				b = { sphere_->radius * cosf(lat + thetaD) * cosf(lon) + sphere_->center.x,
					sphere_->radius * sinf(lat + thetaD) + sphere_->center.y,
				sphere_->radius * cosf(lat + thetaD) * sinf(lon) + sphere_->center.z };

				c = { sphere_->radius * cosf(lat) * cosf(lon + phiD) + sphere_->center.x,
				sphere_->radius * sinf(lat) + sphere_->center.y,
				sphere_->radius * cosf(lat) * sinf(lon + phiD) + sphere_->center.z };

				////スクリーン座標に変換
				//Vector3 ndcVertex = CoordTransform(a, camera->matViewProjection_);
				//a = CoordTransform(ndcVertex, viewportMatrix);
				//ndcVertex = CoordTransform(b, camera->matViewProjection_);
				//b = CoordTransform(ndcVertex, viewportMatrix);
				//ndcVertex = CoordTransform(c, camera->matViewProjection_);
				//c = CoordTransform(ndcVertex, viewportMatrix);

				//ラインを設定
				lines_[lonIndex * 2 + latIndex * kSubdivision_ * 2]->start_ = a;
				lines_[lonIndex * 2 + latIndex * kSubdivision_ * 2]->end_ = b;
				lines_[lonIndex * 2 + latIndex * kSubdivision_ * 2 + 1]->start_ = a;
				lines_[lonIndex * 2 + latIndex * kSubdivision_ * 2 + 1]->end_ = c;

			}
		}

	}

}

void LineSphere::SetColor(Vector4 color)
{

	for (int32_t i = 0; i < kMaxLine_; i++) {
		lines_[i]->color_ = color;
	}

}

void LineSphere::Draw(Camera* camera, const Matrix4x4& matrix) {

#ifdef _DEBUG

	if (sphere_) {

		for (int32_t i = 0; i < kMaxLine_; i++) {
			lines_[i]->Draw(camera, matrix);
		}

	}

#endif // _DEBUG

}
