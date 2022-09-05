#include "RecipeReader.h"

RecipeReader::RecipeReader(State* s){
	state = s;
}

bool RecipeReader::focus(std::string f){
	filepath = f;
	fin.open(filepath);
	return !fin.fail();
}

bool RecipeReader::categoryExists(string cat){
	for (auto c : this->state->categories){
		if (*c == cat){
			return true;
		}
	}
	return false;
}

bool RecipeReader::read(){
	bool SUCCESS = true;
	bool more = true;
	while (more){
		string temp;
		fin >> temp;
		// cout << "outer loop, temp is " + temp + "\n";
		if (temp == "OUT"){more = false;}
		if (temp == "recipe"){
			//file is declaring recipe, read details:
			Recipe* newRecip = new Recipe;
			fin >> temp;
			// cout << "inner loop, temp is " + temp + "\n";
			while (temp != "named"){
				if (temp == "contains"){
					string itemp;
					do {
						Ingredient* newIng = new Ingredient;
						fin >> itemp;
						if (itemp == "opt"){
							newIng->optional = true;
							fin >> itemp;
						}
						newIng->count = stof(itemp);
						fin >> itemp;
						if (itemp == "oz"){
							newIng->weight = newIng->count;
							newIng->count = -1;
						}
						if (itemp == "lbs"){
							newIng->weight = newIng->count * 16;
							newIng->count = -1;
						}
						getline(fin, itemp);
						itemp.erase(0, 1);
						//ADD REFERENCE TO ITEM TO ING
						auto itm = state->getItem(itemp);
						if (itm){
							newIng->item = itm;
						} else {
							cout << "ERROR: " + itemp + " item not found!\n";
							SUCCESS = false;
						}
						
						//ALTERNATE
						fin >> itemp;
						if (itemp == "alt"){
							newIng->alt = new Ingredient;
							fin >> itemp;
							newIng->alt->count = stof(itemp);
							fin >> itemp;
							if (itemp == "oz"){
								newIng->alt->weight = newIng->alt->count;
								newIng->alt->count = -1;
							}
							if (itemp == "lbs"){
								newIng->alt->weight = newIng->alt->count * 16;
								newIng->alt->count = -1;
							}
							getline(fin, itemp);
							itemp.erase(0, 1);
							//ADD REFERENCE TO ITEM TO ING
							auto itm = state->getItem(itemp);
							if (itm){
								newIng->alt->item = itm;
							} else {
								cout << "ERROR: " + itemp + " alt item not found!\n";
								SUCCESS = false;
							}
							fin >> itemp;

						}

						//ADD ING TO RECIP
						newRecip->ingredients[newIng->item->name] = newIng;
					} while (itemp == "and");
					temp = itemp;
				}

				if (temp == "note"){
					getline(fin, newRecip->note, '#');
				}
				if (temp == "type"){
					fin >> newRecip->category;
					//check if cat exists,
					//if not, push dynamic string ref to list
					if (!this->categoryExists(newRecip->category)){
						string* newCat = new string;
						*(newCat) = newRecip->category;
						state->categories.push_back(newCat);
					}

				}
				if (temp == "feeds"){
					fin >> newRecip->feeds;
				}
				if (temp == "takes"){
					fin >> newRecip->time;
				}
				if (temp == "mess"){
					int mess;
					fin >> mess;
					newRecip->mess = (MESS_LEVEL)mess;

				}
				//other keywords...
				fin >> temp;
			}
			// cout << "exited inner loop\n";
			//name...
			getline(fin, newRecip->name);
			newRecip->name.erase(0, 1);

			state->recips.push_back(newRecip);
		}
	}
	for (int k = 0; k < state->categories.size(); k++){
		if (*state->categories.at(k) == "entree"){
			string temp = *state->categories.at(0);
			*state->categories.at(0) = "entree";
			*state->categories.at(k) = temp;
		}
	}
	return SUCCESS;
}

void RecipeReader::close(){
	fin.close();
}