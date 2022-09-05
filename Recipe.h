#pragma once
#include <vector>
#include <string>
#include <map>
#include "Item.h"

using namespace std;

enum MESS_LEVEL {
	NONE = 0,
	LOW,
	MEDIUM,
	HIGH,
	EXTREME,
};

struct Ingredient {
	Item* item;
	float count;
	float weight;
	Ingredient();
	// Ingredient(&Ingredient i) = default;
	string note;
	bool optional;
	float price();
	Ingredient* alt;
};

struct Recipe {
	string name;
	string category;
	std::map<string, Ingredient*> ingredients;
	string note;
	int feeds;
	int time; // minutes
	MESS_LEVEL mess;

	Recipe();

	const Ingredient& getIngredient(string name);
	Ingredient& modIngredient(string name);

	float price(bool);
	float pricePer(bool);
	float price(bool, bool);
	float pricePer(bool, bool);

	bool hasOptionals();
	bool hasAlts();
	bool isVegan();
	bool isVegitarian();
	bool hasMeat();
	bool hasShellfish();

};