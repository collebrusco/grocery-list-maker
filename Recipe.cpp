#include "Recipe.h"
#include <iostream>

Ingredient::Ingredient(){
	weight = -1;
	count = -1;
	optional = false;
	alt = nullptr;
}

float Ingredient::price(){
	if (weight < 0){
		return item->minPrice * (count / item->count);
	} else {
		return item->minPrice * (weight / item->weight);
	}
}

Recipe::Recipe(){
	mess = NONE;
}

const Ingredient& Recipe::getIngredient(string name){
	return *ingredients[name];
}

Ingredient& Recipe::modIngredient(string name){
	return *ingredients[name];
}

float Recipe::price(bool opt){
	float total = 0.0;
	for (auto i : ingredients){
		if (!i.second->optional || opt){
			total += (i.second->price());
		}
	}
	return total;
}

float Recipe::price(bool opt, bool alt){
	float total = 0.0;
	for (auto i : ingredients){
		if (!i.second->optional || opt){
			total += (i.second->price());
		}
		if (i.second->alt && alt){
			total += (i.second->alt->price());
		}
	}
	return total;
}

float Recipe::pricePer(bool opt){
	return this->price(opt) / feeds;
}

float Recipe::pricePer(bool opt, bool alt){
	return this->price(opt, alt) / feeds;
}

bool Recipe::hasOptionals(){
	for (auto i : ingredients){
		if (i.second->optional){
			return true;
		}
	}
	return false;
}

bool Recipe::hasAlts(){
	for (auto i : ingredients){
		if (i.second->alt){
			return true;
		}
	}
	return false;
}

bool Recipe::isVegan(){
	for (auto i : ingredients){
		if (i.second->item->nv ||
			i.second->item->meat ||
			i.second->item->fish ||
			i.second->item->shellfish){
			return false;
		}
	}
	return true;
}

bool Recipe::isVegitarian(){
	for (auto i : ingredients){
		if (i.second->item->meat ||
			i.second->item->fish ||
			i.second->item->shellfish){
			return false;
		}
	}
	return true;
}

bool Recipe::hasMeat(){
	for (auto i : ingredients){
		if (i.second->item->meat){
			return true;
		}
	}
	return false;
}

bool Recipe::hasShellfish(){
	for (auto i : ingredients){
		if (i.second->item->shellfish){
			return true;
		}
	}
	return false;
}
