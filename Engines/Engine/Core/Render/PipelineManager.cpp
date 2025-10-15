#include "PipelineManager.h"
#include <cassert>
#include "Convert.h"

using namespace MLEngine::Core::Render::Pipeline;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

void Manager::Initialize(ID3D12Device* device) {

	assert(device);

	device_ = device;

}

void Manager::CreatePipeLine(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, const std::string& pipelineName) {

	//既にある場合新しく作らない
	if (pipelineStates_.find(pipelineName) != pipelineStates_.end()) {

		return;

	}

	HRESULT hr;
	//パイプライン生成
	hr = device_->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineStates_[pipelineName]));

	assert(SUCCEEDED(hr));

}

ID3D12PipelineState* Manager::GetPipeline(const std::string& pipelineName) {
	//既に生成したパイプラインがあるならそれを返す
	if (pipelineStates_.find(pipelineName) != pipelineStates_.end()) {

		return pipelineStates_[pipelineName].Get();

	}

	return nullptr;

}
