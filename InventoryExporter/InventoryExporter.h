#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


struct ProductStruct {

	int product_id = 0;
	std::string name = "none";
	std::string slot = "none";
	std::string paint = "none";
	std::string quality = "unknown";
	std::string crate = "none";
	std::string special_edition = "none";
	std::string tradeable = "unknown";
	std::string platform = "unknown";
	int amount = 1;
};

const std::vector<std::string> QualityNames = {

	"Common",
	"Uncommon",
	"Rare",
	"VeryRare",
	"Import",
	"Exotic",
	"BlackMarket",
	"Premium",
	"Limited",
	"Legacy",
	"MAX"
};

const std::vector<std::string> PaintNames = {

	"none",
	"Crimson",
	"Lime",
	"Black",
	"Sky Blue",
	"Cobalt",
	"Burnt Sienna",
	"Forest Green",
	"Purple",
	"Pink",
	"Orange",
	"Grey",
	"Titanium White",
	"Saffron",
	"Gold",
	"Rose Gold",
	"White Gold",
	"Onyx",
	"Platinum"
};

class InventoryExporter: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow {

	virtual void onLoad();
	virtual void onUnload();

	void RenderSettings() override;
	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;

	void InventoryExport(std::string Format);
	ProductStruct GetProductStruct(OnlineProductWrapper& product, bool& success);

	bool IsSameItem(ProductStruct& product1, ProductStruct& product2);
	void RemoveDuplicates(std::vector<ProductStruct>& Products);

	void ExportToCSV(std::vector<ProductStruct>& Products);
	void ExportToJSON(std::vector<ProductStruct>& Products);
};
