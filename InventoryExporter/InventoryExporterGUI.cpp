#include "pch.h"
#include "InventoryExporter.h"

/* Plugin Settings Window code here
std::string InventoryExporter::GetPluginName() {
	return "InventoryExporter";
}

void InventoryExporter::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> InventoryExporter
void InventoryExporter::RenderSettings() {
	ImGui::TextUnformatted("InventoryExporter plugin settings");
}
*/

/*
// Do ImGui rendering here
void InventoryExporter::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string InventoryExporter::GetMenuName()
{
	return "inventoryexporter";
}

// Title to give the menu
std::string InventoryExporter::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void InventoryExporter::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool InventoryExporter::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool InventoryExporter::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void InventoryExporter::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void InventoryExporter::OnClose()
{
	isWindowOpen_ = false;
}
*/
