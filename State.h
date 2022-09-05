#pragma once
#include <vector>
#include <string>
#include "Recipe.h"
using namespace std;

struct State {
	vector<Item*> items;
	vector<Recipe*> recips;
	vector<string*> categories;

	static State& getInstance();

	Item* getItem(string name);
	Recipe* getRecipe(string name);

	~State();

	State(State const&) = delete;
	void operator=(State const&) = delete;
private:
	State();
};