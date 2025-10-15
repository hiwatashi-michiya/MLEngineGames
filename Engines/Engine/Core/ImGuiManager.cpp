
#ifdef _DEBUG
#include "ImGuiManager.h"
#include "WindowManager.h"
#include "DirectXSetter.h"
#include "TextureManager.h"
#include "DescriptorHandle.h"

using namespace MLEngine::Core;

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{

	uint32_t index = Core::DirectXSetter::GetInstance()->GetSrvHeap()->GetUnUsedIndex();
	uint32_t descriptorSizeSRV_ = 
		Core::DirectXSetter::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Core::Window::Manager::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(Core::DirectXSetter::GetInstance()->GetDevice(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		Core::DirectXSetter::GetInstance()->GetSrvHeap()->Get(),
		MLEngine::Core::GetCPUDescriptorHandle(Core::DirectXSetter::GetInstance()->GetSrvHeap()->Get(), descriptorSizeSRV_, index),
		MLEngine::Core::GetGPUDescriptorHandle(Core::DirectXSetter::GetInstance()->GetSrvHeap()->Get(), descriptorSizeSRV_, index));

}

void ImGuiManager::Finalize()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiManager::BeginFrame()
{

	//フレーム開始を伝える
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

}

void ImGuiManager::Render()
{

	//ImGuiの内部コマンドを生成する
	ImGui::Render();

	//実際のcommandListのImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Core::DirectXSetter::GetInstance()->GetCommandList());

}



#endif // _DEBUG