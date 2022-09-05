#include <iostream>
#include <vector>
// #include "Item.h"
#include "ItemReader.h"
#include "RecipeReader.h"
#include "Recipe.h"
#include "State.h"

using namespace std;

State state;

void outItems(){
	cout << "Item List:\n";
	for (auto item : state.items){
		if (item->weight > 0){
			cout << item->weight << " oz";
		} else {
			cout << item->count;
		}
		cout << " " + item->name + ": $" << item->minPrice << endl;
	}
	cout << endl;
}

void outRecips(bool d){
	cout << "Recipe List (" << state.recips.size() << " recipe";
	if (state.recips.size() != 1) {
		cout << 's';
	}
	cout << "):\n\n";
	int i = 1;
	for (auto r : state.recips){
		cout << "RECIPE " << i << ": " + r->name << "\ncontains ";
		cout << r->ingredients.size() << " ingredients:\n";
		cout << "---------------------\n";
		for (auto i : r->ingredients){
			if (i.second->optional){
				cout << "OPTIONALLY, ";
			}
			if (i.second->weight > 0){
				cout << i.second->weight << " oz ";
			} else {
				cout << i.second->count << " ";
			}
			cout << i.second->item->name << " at $" << setprecision(3) << i.second->price() << "\n";
			if (i.second->alt){
				cout << "ALTERNATIVELY ";
				if (i.second->alt->weight > 0){
					cout << i.second->alt->weight << " oz ";
				} else {
					cout << i.second->alt->count << " ";
				}
				cout << i.second->alt->item->name << " can be used for $";
				cout << setprecision(3) << i.second->alt->price() << "\n";
			}
		}
		cout << "---------------------\n";
		cout << setprecision(3) << "feeds " << r->feeds << " for $" << r->price(false);
		if (r->hasOptionals()){
			cout << " or $" << r->price(true) << " with options.\n";
		} else {
			cout << ".\n";
		}
		if (r->feeds > 1) {
			cout << setprecision(3) << "that's $" << r->pricePer(false) << " per person";
			if (r->hasOptionals()){
				cout << " or $" << r->pricePer(true) << " per person with options.\n";
			} else {
				cout << ".\n";
			}
		}

		if (r->hasAlts()) {
			cout << setprecision(3) << "\nAlternate version costs $" << r->price(false, true);
			if (r->hasOptionals()){
				cout << " and $" << r->price(true, true) << " optioned.\n";
			} else {
				cout << ".\n";
			}
			if (r->feeds > 1) {
				cout << setprecision(3) << "that's $" << r->pricePer(false, true) << " per person";
				if (r->hasOptionals()){
					cout << " or $" << r->pricePer(true, true) << " per person optioned.\n";
				} else {
					cout << ".\n";
				}
			}
		}



		cout << "takes about " << r->time << " minutes to make ";
		cout << "and makes ";
		switch (r->mess){
			case 0:
			cout << "no ";
			break;
			case 1:
			cout << "a light ";
			break;
			case 2:
			cout << "a medium ";
			break;
			case 3:
			cout << "a decent ";
			break;
			case 4:
			cout << "an extreme ";
			break;
			default:
			cout << "ERROR ";
			break;
		}
		cout << "mess.\n";

		if (d) {cout << "directions:" << r->note;}
		i++;
		cout << "========================================================\n\n";
	}
	cout << "\n";
}

int main(){
	ItemReader ireader(&state.items);
	if (!ireader.focus("items.txt")){
		cout << "no item file found!!!\n";
	}
	ireader.read();
	ireader.close();

	RecipeReader rreader(&state);
	if (!rreader.focus("items.txt")){
		cout << "no item file found for recipes!!!\n";
	}
	rreader.read();
	rreader.close();

	cout << endl;
	outRecips(false);
	

	return 0;
}