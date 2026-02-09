
#ifdef _DEBUG
#include "ImGuiManager.h"
#include "WindowManager.h"
#include "DirectXSetter.h"
#include "DXDevice.h"
#include "TextureManager.h"

using namespace MLEngine::Core;

ImGuiManager* ImGuiManager::GetInstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Initialize()
{

	uint32_t index = DirectXSetter::GetInstance()->GetSrvHeap()->GetUnUsedIndex();

	//ImGuiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// ImGuiIO
	auto& imguiIO = ImGui::GetIO();
	// Docking有効化
	imguiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImWchar const ranges[] = { 0x0020, 0xfffd, 0, };
	// 日本語フォント追加
	ImFont* font = imguiIO.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, NULL, ranges);

	if (font == nullptr) {
		// フォントの読み込みに失敗
		printf("Failed to load Japanese font!\n");
	}
	else {
		// フォント読み込み成功
		printf("Japanese font loaded successfully!\n");
		imguiIO.FontDefault = font; // デフォルトフォントに設定
	}

	imguiIO.Fonts->Build();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(Window::Manager::GetInstance()->GetHwnd());
	ImGui_ImplDX12_Init(DXDevice::GetInstance()->GetDevice(),
		2,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DirectXSetter::GetInstance()->GetSrvHeap()->Get(),
		DirectXSetter::GetInstance()->GetSrvHeap()->GetCPUDescriptorHandle(index),
		DirectXSetter::GetInstance()->GetSrvHeap()->GetGPUDescriptorHandle(index));



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

void ImGuiManager::SetDisplay()
{

	RECT rc;
	GetClientRect(Window::Manager::GetInstance()->GetHwnd(), &rc);

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(
		(float)(rc.right - rc.left),
		(float)(rc.bottom - rc.top));

	float dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(Window::Manager::GetInstance()->GetHwnd());
	io.FontGlobalScale = dpiScale;

}

#endif // _DEBUG