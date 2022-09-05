#pragma once
#include <string>


struct Item {
	Item();
	Item(std::string);
	Item(std::string, float);
	Item(std::string, float, float);
	~Item() = default;
	Item(Item&) = default;

	float pricePer();

	float minPrice, maxPrice;
	std::string name;
	float count; // or weight if applicable (eg lbs of ground beef)
	float weight; //stored in oz

	bool meat = false;
	bool fish = false;
	bool shellfish = false;
	bool nv = false;


	//TODO: nutritional info?? category?? add weight??
};
