#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
// #include "Item.h"
#include "Recipe.h"
#include "State.h"
using namespace std;

class RecipeReader {
private:
	State* state;
	std::string filepath;
	ifstream fin;
	bool categoryExists(string);
public:
	RecipeReader(State* s);
	bool focus(std::string);
	bool read();
	void close();
};
