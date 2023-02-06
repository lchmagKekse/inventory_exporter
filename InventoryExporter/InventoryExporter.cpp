#include "pch.h"
#include "InventoryExporter.h"

#include <fstream>
#include "json.hpp"
using nlohmann::json;

BAKKESMOD_PLUGIN(InventoryExporter, "InventoryExporter", plugin_version, PLUGINTYPE_FREEPLAY)
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::shared_ptr<GameWrapper> gw;

void InventoryExporter::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerNotifier("Inventory_Export_CSV", [this](std::vector<std::string> args) {
		InventoryExport("csv");
	}, "", PERMISSION_ALL);

	cvarManager->registerNotifier("Inventory_Export_JSON", [this](std::vector<std::string> args) {
		InventoryExport("json");
	}, "", PERMISSION_ALL);
}
void InventoryExporter::onUnload() {

	cvarManager->removeNotifier("Inventory_Export_CSV");
	cvarManager->removeNotifier("Inventory_Export_JSON");
}

void InventoryExporter::InventoryExport(std::string Format) {

	auto itemsWrapper = gw->GetItemsWrapper();
	if (itemsWrapper.IsNull()) return;

	auto inventory = itemsWrapper.GetOwnedProducts();

	std::vector<ProductStruct> products;

	for (auto item : inventory) {

		bool success = false;
		auto productStruct = GetProductStruct(item, success);

		if (!success) continue;

		products.push_back(productStruct);
	}

	RemoveDuplicates(products);

	if (Format == "json") ExportToJSON(products);
	if (Format == "csv") ExportToCSV(products);
}
ProductStruct InventoryExporter::GetProductStruct(OnlineProductWrapper& product, bool& success) {

	ProductStruct productStruct{};

	if (product.IsNull()) return productStruct;
	if (product.GetProductID() <= 0) return productStruct;
	if (product.GetQuality() == 0 || product.GetQuality() == 7 || product.GetQuality() == 9) return productStruct;
	if (productStruct.slot == "Player Title" || productStruct.slot == "Blueprints" || productStruct.slot == "Goal Stinger") return productStruct;


	//ProductID
	productStruct.product_id = product.GetProductID();


	//Label
	productStruct.name = product.GetLongLabel().ToString();


	//Slot name
	auto productWraper = product.GetProduct();

	if (!productWraper.IsNull()) {
		productStruct.slot = productWraper.GetSlot().GetOnlineLabel().ToString();
	}


	//Attributes
	auto attributes = product.GetAttributes();

	if (!attributes.IsNull()) {

		for (auto attribute : attributes) {

			//Paint
			if (attribute.GetAttributeType() == "ProductAttribute_Painted_TA") {

				auto PaintID = ProductAttribute_PaintedWrapper(attribute.memory_address).GetPaintID();
				productStruct.paint = PaintNames[PaintID];
			}


			//Special Edition
			if (attribute.GetAttributeType() == "ProductAttribute_SpecialEdition_TA") {

				auto specialEditionDB = gameWrapper->GetItemsWrapper().GetSpecialEditionDB();

				ProductAttribute_SpecialEditionWrapper SpecialEditionWrapper(attribute.memory_address);
				productStruct.product_id = SpecialEditionWrapper.GetOverrideProductID(productStruct.product_id);

				if (!specialEditionDB.IsNull()) {
					productStruct.special_edition = specialEditionDB.GetSpecialEditionName(SpecialEditionWrapper.GetEditionID());
				}				
			}
		}
	}
	

	//Quality
	productStruct.quality = QualityNames[product.GetQuality()];
	

	//Crate
	productStruct.crate = product.GetProductSeries();

	if (productStruct.crate == "Postgame") productStruct.crate = "none";


	//Tradable
	productStruct.tradeable = product.GetIsUntradable() ? "false" : "true";


	//Platform
	//Not implemented yet

	success = true;
	return productStruct;
}

void InventoryExporter::RemoveDuplicates(std::vector<ProductStruct>& Products) {

	for (int i = 0; i < Products.size(); i++) {

		for (int x = i + 1; x < Products.size(); x++) {

			if (IsSameItem(Products[i], Products[x])) {

				Products[i].amount++;
				Products.erase(Products.begin() + x);
				x--;
			}
		}
	}
}
bool InventoryExporter::IsSameItem(ProductStruct& product1, ProductStruct& product2) {

	if (product1.product_id != product2.product_id) return false;
	if (product1.paint != product2.paint) return false;
	if (product1.quality != product2.quality) return false;
	if (product1.crate != product2.crate) return false;
	if (product1.special_edition != product2.special_edition) return false;
	if (product1.tradeable != product2.tradeable) return false;

	return true;
}

void InventoryExporter::ExportToCSV(std::vector<ProductStruct>& Products) {

	std::ofstream invExport;
	invExport.open(gameWrapper->GetDataFolder() / "inventory.csv");
	invExport << "product id,name,slot,paint,quality,crate,amount,special_edition,tradeable,platform" << std::endl;

	for (auto& Product : Products) {

		invExport << Product.product_id << ",";
		invExport << Product.name << ",";
		invExport << Product.slot << ",";
		invExport << Product.paint << ",";
		invExport << Product.quality << ",";
		invExport << Product.crate << ",";
		invExport << Product.amount << ",";
		invExport << Product.special_edition << ",";
		invExport << Product.tradeable << ",";
		invExport << Product.platform << "\n";
	}

	invExport.close();
}
void InventoryExporter::ExportToJSON(std::vector<ProductStruct>& Products) {

	json jInventory;

	for (auto& Product : Products) {

		json jProduct;

		jProduct["product_id"] = Product.product_id;
		jProduct["name"] = Product.name;
		jProduct["slot"] = Product.slot;
		jProduct["paint"] = Product.paint;
		jProduct["quality"] = Product.quality;
		jProduct["crate"] = Product.crate;
		jProduct["amount"] = Product.amount;
		jProduct["special_edition"] = Product.special_edition;
		jProduct["tradeable"] = Product.tradeable;
		jProduct["platform"] = Product.platform;

		jInventory.push_back(jProduct);
	}

	std::ofstream(gw->GetDataFolder() / "inventory.json") << jInventory.dump(4);
}