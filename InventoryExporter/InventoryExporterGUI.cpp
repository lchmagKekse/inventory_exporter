#include "pch.h"
#include "InventoryExporter.h"

std::string InventoryExporter::GetPluginName() {
	return "InventoryExporter";
}

void InventoryExporter::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}


void InventoryExporter::RenderSettings() {
	
	if (ImGui::Button("Export to CSV")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("Inventory_Export_CSV");
		});
	}

	ImGui::SameLine();

	if (ImGui::Button("Export to JSON")) {
		gameWrapper->Execute([this](GameWrapper* gw) {
			cvarManager->executeCommand("Inventory_Export_JSON");
		});
	}
}
