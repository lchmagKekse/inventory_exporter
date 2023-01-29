#include "pch.h"
#include "InventoryExporter.h"
#include <fstream>

BAKKESMOD_PLUGIN(InventoryExporter, "InventoryExporter", plugin_version, PLUGINTYPE_FREEPLAY)
std::shared_ptr<CVarManagerWrapper> _globalCvarManager;
std::shared_ptr<GameWrapper> gw;

void InventoryExporter::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerNotifier("InventoryExport", [this](std::vector<std::string> args) {
		InventoryExport();
		}, "", PERMISSION_ALL);

}

void InventoryExporter::onUnload() { }

void InventoryExporter::InventoryExport() {

	auto itemsWrapper = gw->GetItemsWrapper();
	if (itemsWrapper.IsNull()) { LOG("ItemsWrapper NULL"); return; }

	auto inventory = itemsWrapper.GetOwnedProducts();

	std::vector<ProductStruct> products;

	for (auto item : inventory) {

		if (item.IsNull()) continue;
		if (item.GetProductID() <= 0) continue;
		
		products.push_back(GetProductStruct(item));
	}

	ExportToCSV(products);
}

ProductStruct InventoryExporter::GetProductStruct(OnlineProductWrapper& product) {

	ProductStruct productStruct{};

	if (product.IsNull()) return productStruct;

	//ProductID
	productStruct.product_id = product.GetProductID();

	//Label
	productStruct.name = product.GetLongLabel().ToString();

	//Slot name
	auto productWraper = product.GetProduct();
	if (!productWraper.IsNull()) {
		productStruct.slot = productWraper.GetSlot().GetOnlineLabel().ToString();
	}

	//Paint
	auto attributes = product.GetAttributes();
	if (!attributes.IsNull()) {

		for (auto attribute : attributes) {

			if (attribute.GetAttributeType() == "ProductAttribute_Painted_TA") {

				auto PaintID = ProductAttribute_PaintedWrapper(attribute.memory_address).GetPaintID();

				if (PaintID <= 0 && PaintID < PaintNames.size()) {
					productStruct.paint = QualityNames[PaintID];
				}
			}
		}
	}
	
	//Certified
	if (!attributes.IsNull()) {

		for (auto attribute : attributes) {

			if (attribute.GetAttributeType() == "ProductAttribute_Certified_TA") {

				ProductAttribute_CertifiedWrapper certifiedWrapper(attribute.memory_address);

				if (!certifiedWrapper.IsNull()) {

					auto statDB = gw->GetItemsWrapper().GetCertifiedStatDB();

					productStruct.certification = statDB.GetStatName(certifiedWrapper.GetStatId());
				}
			}
		}
	}

	//Quality
	auto quality = product.GetQuality();
	if (quality <= 0 && quality < QualityNames.size()) {
		productStruct.quality = QualityNames[quality];
	}

	//Crate
	productStruct.crate = product.GetProductSeries();

	//Tradable
	productStruct.tradeable = product.GetIsUntradable() ? "false" : "true";

	return productStruct;
}

void InventoryExporter::ExportToCSV(std::vector<ProductStruct>& Products) {

	std::ofstream invExport;
	invExport.open(gameWrapper->GetDataFolder() / "Inventory.csv");
	invExport << "product id,name,slot,paint,certification,quality,crate,tradeable,amount" << std::endl;

	for (auto& Product : Products) {

		invExport << Product.product_id << ",";
		invExport << Product.name << ",";
		invExport << Product.slot << ",";
		invExport << Product.paint << ",";
		invExport << Product.certification << ",";
		invExport << Product.quality << ",";
		invExport << Product.crate << ",";
		invExport << Product.tradeable << ",";
		invExport << Product.amount << "\n";
	}

	invExport.close();
}