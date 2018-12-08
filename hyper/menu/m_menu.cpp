#include "../common_includes.hpp"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"

m_menu* cs_menu = new m_menu();

void m_menu::initialize()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;
	D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
	cs_renderer->device->GetCreationParameters(&d3dcp);
	//io.Fonts->AddFontDefault();
	ImGui_ImplWin32_Init(cs_overlay->handle);
	ImGui_ImplDX9_Init(cs_renderer->device);
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
}

void m_menu::draw()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Text("Hello World!");

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}