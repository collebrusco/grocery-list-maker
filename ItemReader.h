#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "Item.h"
using namespace std;

class ItemReader {
private:
	vector<Item*> *items;
	std::string filepath;
	ifstream fin;
public:
	ItemReader(vector<Item*>*);
	bool focus(std::string);
	bool read();
	void close();
};
