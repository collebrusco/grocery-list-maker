#include <iostream>
#include <vector>
#include <assert.h>
// #include "Item.h"
#include "ItemReader.h"
#include "RecipeReader.h"
#include "Recipe.h"
#include "State.h"
#include "flib/Queue.tpp"
#include "Screen.h"
#include "Prompts.h"
#include "macros.h"

using namespace std;

// State state;

State &state = State::getInstance();


void outItems(){
	// cout << "Item List:\n";
	// for (auto item : state.items){
	// 	if (item->weight > 0){
	// 		cout << item->weight << " oz";
	// 	} else {
	// 		cout << item->count;
	// 	}
	// 	cout << " " + item->name + ": $" << item->minPrice << endl;
	// }
	// cout << endl;
	cout << "Item List:\n";
	for (int i = 0; i < state.items.size(); i++){
		if (state.items.at(i)->weight > 0){
			cout << state.items.at(i)->weight << " oz";
		} else {
			cout << state.items.at(i)->count;
		}
		cout << " " + state.items.at(i)->name + ": $" << state.items.at(i)->minPrice << endl;
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
		cout << "\n========================================================\n\n";
	}
	cout << "\n";
}

void read(){
	string path = "items.txt";
	ItemReader ireader(&state.items);
	if (!ireader.focus(path)){
		cout << "no item file found!!!\n";
		return;
	}
	ireader.read();
	ireader.close();

	RecipeReader rreader(&state);
	if (!rreader.focus(path)){
		cout << "no item file found for recipes!!!\n";
		return;
	}
	rreader.read();
	rreader.close();
}

Screen welcomeScreen(){
	Screen s;
	s.pc("Welcome to the Grocery List Maker 3000!");
	s.pc("~an FC experience~");
	s.pc("just kidding");
	s.pc("");
	s.pops();
	s.genout();
	return s;
}

template <typename T>
void swap(vector<T> &vec, int a, int b){
	T temp = vec.at(a);
	vec.at(a) = vec.at(b);
	vec.at(b) = temp;
}

bool compAlpha(Item* a, Item* b){return 	a->name < b->name;}
bool compPLH(Item* a, Item* b){return 		a->minPrice < b->minPrice;}
bool compPHL(Item* a, Item* b){return 		a->minPrice > b->minPrice;}

void sortItems(vector<Item*> &items, bool(*comp)(Item*, Item*)){
	for (int i = items.size(); i >= 0; i--){
		for (int j = 1; j < i; j++){
			if (comp(items.at(j), items.at(j - 1))){
				swap<Item*>(items, j, j - 1);
			}
		}
	}
}

int main(){
	read();
	//DEBUG:
	// cout << "Categories read:\n";
	// for (auto c : state.categories){
	// 	cout << *c << "\n";
	// }
	
	Screen welc = welcomeScreen();
	Screen *window = &welc;

	Screen s;
	char input;
	//prompts..
	TutorialPrompt tutPrompt(s, input);
	ListPrompt lsPrompt(s, input);
	RecipePrompt rprompt(s, input);

	do {
		cout << window->out;
		if (window != &s){window = &s;}
		cin >> input;
		switch (input){
		case 'n':
			lsPrompt.prompt();
			break;
		case 'r':
			rprompt.prompt();
			if (input == 'b'){
				s.cleartag();
				s.pops();
				s.genout();
			}
			window = &s;
			break;
		case 'i':
			//TODO: sort by given
			s.clear();
			s.tag();
			s.p("Enter 'a' to display in alphabetical order,");
			s.p("Enter 'c' to display by price low to high,");
			s.p("Enter 'C' to display by price high to low.");
			cout << s.genout();
			cin >> input;
			switch(input){
			case 'a':
				sortItems(state.items, compAlpha);
				break;
			case 'c':
				// cout << "calling sort w PLH spec...\n";
				sortItems(state.items, compPLH);
				break;
			case 'C':
				sortItems(state.items, compPHL);
				break;
			default:
				break;
			}
			outItems();
			s.clear();
			s.tag();
			s.pc("== Scroll up to see item list! ==");
			s.nl();
			s.pops();
			s.genout();
			break;
		case '?':
			tutPrompt.prompt();
			s.clear();
			s.tag();
			s.pops();
			s.genout();
			break;
		
		case 'x':
		s.clear();
		s.tag();
		s.pc("Thanks for using the Grocery List Maker 3000!");
		s.pc("Created by Frank Collebrusco");
		s.pc("Contact " + EMAIL + " for bugs or comments");
		s.pc(GITHUB_REPO);
		cout << s.genout();
			break;

		default:
			s.clear();
			s.tag();
			s.pc("ERROR: Unknown menu option! Try again..");
			s.nl();
			s.pops();
			s.genout();
			window = &s;
			

			break;
		}



	} while (input != 'x');






	return 0;
}












