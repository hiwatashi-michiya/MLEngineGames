#pragma once
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include <map>
#include <string>
#include <vector>

namespace MLEngine::Resource {

	//頂点データ
	struct VertexData {
		MLEngine::Math::Vector4 position;
		MLEngine::Math::Vector2 texcoord;
		MLEngine::Math::Vector3 normal;
		float padding;
	};
	//マテリアルデータ
	struct MaterialData
	{
		MLEngine::Math::Vector4 color;

		int32_t enableLighting;
		float shininess; //光沢度

		float padding[2];

		MLEngine::Math::Matrix4x4 uvTransform;

	};
	//平行光源
	struct DirectionalLight {
		MLEngine::Math::Vector4 color;
		MLEngine::Math::Vector3 direction;
		float intensity;
	};
	//点光源
	struct PointLight {
		MLEngine::Math::Vector4 color; //ライトの色
		MLEngine::Math::Vector3 position; //ライトの位置
		float intensity; //輝度
		float radius; //ライトの届く最大距離
		float decay; //減衰率
		float padding[2];
	};
	//画像データ
	struct TextureData {
		std::string textureFilePath;
	};
	//メッシュデータ
	struct MeshData {
		std::vector<VertexData> vertices;
	};

	//GPUで利用するカメラ座標
	struct CameraForGPU
	{
		MLEngine::Math::Vector3 worldPosition;
	};

	//マップエディター用のデータ構造体
	struct ObjectData
	{
		//オブジェクトの形状を決める名前
		std::string objName;
		//位置
		MLEngine::Math::Vector3 position;
		//回転
		MLEngine::Math::Vector3 rotation;
		//スケール
		MLEngine::Math::Vector3 scale;

	};
	//オイラー角トランスフォーム
	struct EulerTransform {
		MLEngine::Math::Vector3 scale;
		MLEngine::Math::Vector3 rotate;
		MLEngine::Math::Vector3 translate;
	};
	//クォータニオントランスフォーム
	struct QuaternionTransform {
		MLEngine::Math::Vector3 scale;
		MLEngine::Math::Quaternion rotate;
		MLEngine::Math::Vector3 translate;
	};
	//ノード
	struct Node {
		QuaternionTransform transform;
		MLEngine::Math::Matrix4x4 localMatrix; //ノードのトランスフォーム
		std::string name; //ノードの名前
		std::vector<Node> children; //子供のノード
	};
	//頂点の重みデータ
	struct VertexWeightData {

		float weight;
		uint32_t vertexIndex;

	};
	//ジョイントの重みデータ
	struct JointWeightData {

		MLEngine::Math::Matrix4x4 inverseBindPoseMatrix;
		std::vector<VertexWeightData> vertexWeights;

	};
	//スキニングモデルデータ
	struct ModelData {
		std::map <std::string, JointWeightData> skinClusterData;
		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;
		MaterialData material;
		Node rootNode;
	};

}

//レンダリングに必要な構造体などを纏めたヘッダー

//TextureData LoadMaterialTemplateFile(const std::string& filename);

//MeshData LoadModelFile(const std::string& filename);
