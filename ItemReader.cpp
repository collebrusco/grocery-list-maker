#include "ItemReader.h"

// class ItemReader {
// private:
// 	vector<Item> *Items;

// public:
// 	ItemReader(vector<Item>*);
// 	void focus(string);
// 	bool read();
// };

ItemReader::ItemReader(vector<Item*>* i){
	items = i;
}
bool ItemReader::focus(std::string f){
	filepath = f;
	fin.open(filepath);
	return !fin.fail();
}

bool ItemReader::read() {
	bool SUCCESS = true;
	bool moreItems = true;
	while (moreItems){
		string temp;
		fin >> temp;
		// cout << "outer loop, temp is " + temp + "\n";
		if (temp == "OUT"){moreItems = false;}
		if (temp == "item"){
			//file is declaring item, read details:
			Item * newItem = new Item;
			fin >> temp;
			// cout << "inner loop, temp is " + temp + "\n";

			while (temp != "named"){
				//costs keyword
				if (temp == "costs" || temp == "cost"){
					fin >> newItem->minPrice;
					newItem->maxPrice = newItem->minPrice;
				}
				if (temp == "to"){
					fin >> newItem->maxPrice;
				}
				if (temp == "count"){
					fin >> newItem->count;
				}
				if (temp == "weight"){
					fin >> newItem->weight;
				}
				if (temp == "lbs"){
					newItem->weight *= 16;
				}
				if (temp == "m"){
					newItem->meat = true;
				}
				if (temp == "f"){
					newItem->fish = true;
				}
				if (temp == "sf"){
					newItem->shellfish = true;
				}
				if (temp == "nv"){
					newItem->nv = true;
				}
				//other keywords...
				fin >> temp;
			}
			// cout << "exited inner loop\n";
			//name...
			getline(fin, newItem->name);
			newItem->name.erase(0, 1);
			items->push_back(newItem);
		}
	}
	return true;
}

void ItemReader::close(){
	fin.close();
}