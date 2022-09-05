#include "Prompts.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include "macros.h"

using namespace std;

static State& state = State::getInstance();

static int charToInt(char a){
	return (int)a - 48;
}

static string floatToString(float x, int digits){
	string temp = to_string(x);
	string res = "";
	for (int i = 0; i < temp.size(); i++){
		res.push_back(temp.at(i));
		if (temp.at(i) == '.'){
			for (int j = 1; j <= digits; j++){
				if (temp.size() > i + j) {
					res.push_back(temp.at(i + j));
				}
			}
			return res;
		}
	}
	return res;
}

static bool searchTermInString(string term, string target, int termdex = 0, int tardex = 0){
	if (termdex == 0 && tardex == 0){
		int TEMP = target.size() - term.size();
		for (int i = 0; i < TEMP; i++){
			if (target.at(i) == term.at(0)){
				if (term.size() == 1){
					return true;
				}
				// cout << "term 0 letter matches " << i;
				// cout << " letter in targ " << target << endl;
				if (searchTermInString(term, target, termdex + 1, i + 1)){
					return true;
				}
			}
		}
		return false;
	} else {
		if (term.at(termdex) == target.at(tardex)){
			// cout << "term " << termdex << " letter matches " << tardex;
			// cout << " letter in targ " << target << endl;
			if (termdex == term.size() - 1){
				// cout << "match!\n";
				return true;
			} else {
				return searchTermInString(term, target, termdex + 1, tardex + 1);
			}
		} else {
			// cout << "term " << termdex << " letter DOESN'T match " << tardex;
			// cout << " letter in targ " << target << endl;
			return false;
		}
	}

}

static vector<Recipe*>* searchPartialRecips(string term){
	vector<Recipe*>* vec = new vector<Recipe*>;
	for (auto recip : state.recips){
		if (searchTermInString(term, recip->name)){
			vec->push_back(recip);
		}
	}
	return vec;
}

static Recipe* promptRecipeSearch(Screen &s){
	string text;
	char i;
	s.cleartag();
	s.p("Enter recipe name or term");
	s.genout();
	bool getlinedPrev = false;
	do {
		cout << s.out;
		if (!getlinedPrev)
			cin.ignore();
		getline(cin, text);
		getlinedPrev = true;
		cout << text << "\n";
		if (text.size() == 1){
			i = text.at(0);
			if (i == 'b')
				break;
		} 
		auto res = searchPartialRecips(text);
		vector<Recipe*>& results = *res;
		if (results.size() == 0){
			s.cleartag();
			s.p("No recipies found with keyword " + text + " in title!");
			s.p("Enter another search term to search again,");
			s.p("Enter 'b' to go back");
			s.genout();
		} else {
			s.cleartag();
			string resultStr = "Found " + to_string(results.size()) + " match";
			if (results.size() > 1)
				resultStr += "es";
			resultStr += " to \"" + text += "\":";
			s.p(resultStr);
			int k = 0;
			for (auto r : results){
				s.p(to_string(k) + ") " + r->name);
				k++;
			}
			if (results.size() == 1){
				return results.at(0);
			} else {
				s.nl();
				s.p("Enter 0 through " + to_string(results.size() - 1) + " to select a result,");
				s.p("Enter 's' to search for something else,");
				s.p("Enter 'b' to go back");
				s.genout();
				do {
					cout << s.out;
					cin >> i;
					getlinedPrev = false;
					if (i == 's') {

					} else if ((int)i - 48 < 0 || (int)i - 48 > results.size()){
						// i = 'b';
						s.cleartag();
						s.pc("input out of bounds...");
					} else {
						return results.at((int)i - 48);
					}
				} while (i != 's' && i != 'b');
				s.cleartag();
				s.p("Enter a search term search for a recipe,");
				s.p("Enter 'b' to go back to the main menu");
				s.genout();
			}
		}
		delete res;
	} while (i != 'b');
	return nullptr;
}

Prompt::Prompt(Screen &screen, char& i) : s(screen), input(i) {}

AIPrompt::AIPrompt(Screen &screen, char& i) : Prompt(screen, i) {}
void AIPrompt::prompt(){
	string text;
	int intinput;
	AIList ailist(state);
	//maybe define an obj here that holds user data about list
	//and then has a gen() func to get a list, this way a user can
	//re-roll w/ current settings, or save settings!
	for (int j = 0; j < state.categories.size(); j++){
		string &cat = *state.categories.at(j);
		s.cleartag();
		s.p("How many " + cat + "s do you need?");
		cout << s.genout();
		cin >> intinput;
		ailist.catPVec.push_back(intinput);
		if (cat == "entree" && intinput > 0){
			s.cleartag();
			//   ----------------------------------------------------------------
			s.p("Of those " + to_string(intinput) + " entrees, how many would you like to cook?");
			s.p("Meaning, how many meals with significant prep time (> 15 min)");
			s.p("or with significant mess do you want to cook?");
			cout << s.genout();
			cin >> ailist.bigCooks;
		}

	}
	//AILIST HAS NUMBERS, CALC LOOP
	do {
		auto recips = ailist.genRList();
		s.cleartag();
		int k = 1;
		for (auto r : *recips){
			s.p(to_string(k) + ") " + r->name);
			k++;
		}
		s.nl();
		//   ----------------------------------------------------------------
		s.p("Chosen recipies listed above. Enter 'r' to reroll,");
		s.p("Or enter 'n' to edit and finalize this list.");
		s.p("Use 'c' to cancel & exit.");
		cout << s.genout();
		cin >> input;
		if (input == 'n'){
			recipies = recips;
		} else {
			delete recips;
		}
	} while (input != 'n' && input != 'c');
	//LIST IS SET, MANUAL VETO/ADDING NEXT
	//if user entered c to cancel, call in listprompt prompt()
	//will exit to main menu without continuing list
	if (input == 'c')
		return;

	//EDIT LOOP
	do {
		s.cleartag();
		int k = 1;
		for (auto r : *recipies){
			s.p(to_string(k) + ") " + r->name);
			k++;
		}
		s.nl();
		//   ----------------------------------------------------------------
		s.p("Enter 1 through " + to_string(k-1) + " to remove or replace an entry,");
		s.p("Enter '+' to add another recipe manually,");
		s.p("Enter 'n' to finalize this list.");
		cout << s.genout();
		cin >> input;
		if (input <= to_string(k-1).at(0) && input >= '1'){
			//selected an entry
			string in;
			do {
				s.cleartag();
				s.p("Selected " + recipies->at(charToInt(input) - 1)->name);
				s.p("Enter 'rem' to remove entry,");
				s.p("Enter 'rep' to replace entry,");
				s.p("Enter 'b' to go back");
				cout << s.genout();
				cin >> in;
				if (in == "rem"){
					//remove entry..
					this->popRecip(recipies, charToInt(input) - 1);
					in = "b";
				} else if (in == "rep"){
					//remove and search...
					auto replacement = promptRecipeSearch(s);
					if (replacement){
						recipies->at(charToInt(input) - 1) = replacement;
					}
					in = "b";
				} 
			} while (in != "b");
		} else if (input == '+') {
			//add from search...
			auto replacement = promptRecipeSearch(s);
			if (replacement){
				recipies->push_back(replacement);
			}
		} else if (input != 'n'){
			//unknown...
		}
	} while (input != 'n');
}

void AIPrompt::cprompt(){
	string text;
	int intinput;
	recipies = new vector<Recipe*>;
	//EDIT LOOP
	do {
		s.cleartag();
		int k = 1;
		for (auto r : *recipies){
			s.p(to_string(k) + ") " + r->name);
			k++;
		}
		s.nl();
		//   ----------------------------------------------------------------
		if (k > 1)
			s.p("Enter 1 through " + to_string(k-1) + " to remove or replace an entry,");
		s.p("Enter '+' to add another recipe manually,");
		s.p("Enter 'n' to finalize this list.");
		cout << s.genout();
		cin >> input;
		if (input <= to_string(k-1).at(0) && input >= '1'){
			//selected an entry
			string in;
			do {
				s.cleartag();
				s.p("Selected " + recipies->at(charToInt(input) - 1)->name);
				s.p("Enter 'rem' to remove entry,");
				s.p("Enter 'rep' to replace entry,");
				s.p("Enter 'b' to go back");
				cout << s.genout();
				cin >> in;
				if (in == "rem"){
					//remove entry..
					this->popRecip(recipies, charToInt(input) - 1);
					in = "b";
				} else if (in == "rep"){
					//remove and search...
					auto replacement = promptRecipeSearch(s);
					if (replacement){
						recipies->at(charToInt(input) - 1) = replacement;
					}
					in = "b";
				} 
			} while (in != "b");
		} else if (input == '+') {
			//add from search...
			auto replacement = promptRecipeSearch(s);
			if (replacement){
				recipies->push_back(replacement);
			}
		} else if (input != 'n'){
			//unknown...
		}
	} while (input != 'n');
}

void AIPrompt::popRecip(vector<Recipe*>* r, int idx){
	r->at(idx) = r->at(r->size() - 1);
	r->pop_back();
}
static string soutRecip(bool d, Recipe* r, int NUM){
	stringstream out;
	string rName = "";
	for (int i = 0; i < r->name.size(); i++){
		if (r->name.at(i) == ' ')
			rName += " ";
		else
			rName += (char)((int)r->name.at(i) - 32);
	}
	out << NUM << ") " << rName << " contains ";
	out << r->ingredients.size() << " ingredients:\n";
	out << "---------------------\n";
	for (auto i : r->ingredients){
		if (i.second->optional){
			out << "OPTIONALLY, ";
		}
		if (i.second->weight > 0){
			out << i.second->weight << " oz ";
		} else {
			out << i.second->count << " ";
		}
		out << i.second->item->name << " at $" << setprecision(3) << i.second->price() << "\n";
		if (i.second->alt){
			out << "ALTERNATIVELY ";
			if (i.second->alt->weight > 0){
				out << i.second->alt->weight << " oz ";
			} else {
				out << i.second->alt->count << " ";
			}
			out << i.second->alt->item->name << " can be used for $";
			out << setprecision(3) << i.second->alt->price() << "\n";
		}
	}
	out << "---------------------\n";
	out << setprecision(3) << "feeds " << r->feeds << " for $" << r->price(false);
	if (r->hasOptionals()){
		out << " or $" << r->price(true) << " with options.\n";
	} else {
		out << ".\n";
	}
	if (r->feeds > 1) {
		out << setprecision(3) << "that's $" << r->pricePer(false) << " per person";
		if (r->hasOptionals()){
			out << " or $" << r->pricePer(true) << " per person with options.\n";
		} else {
			out << ".\n";
		}
	}

	if (r->hasAlts()) {
		out << setprecision(3) << "\nAlternate version costs $" << r->price(false, true);
		if (r->hasOptionals()){
			out << " and $" << r->price(true, true) << " optioned.\n";
		} else {
			out << ".\n";
		}
		if (r->feeds > 1) {
			out << setprecision(3) << "that's $" << r->pricePer(false, true) << " per person";
			if (r->hasOptionals()){
				out << " or $" << r->pricePer(true, true) << " per person optioned.\n";
			} else {
				out << ".\n";
			}
		}
	}

	out << "takes about " << r->time << " minutes to make ";
	out << "and makes ";
	switch (r->mess){
		case 0:
		out << "no ";
		break;
		case 1:
		out << "a light ";
		break;
		case 2:
		out << "a medium ";
		break;
		case 3:
		out << "a decent ";
		break;
		case 4:
		out << "an extreme ";
		break;
		default:
		out << "ERROR ";
		break;
	}
	out << "mess.\n";

	if (d && r->note != "" && r->note != " ") {out << "\ndirections:" << r->note;}

	out << "\n\n";
	return out.str();
}
string AIPrompt::genList(){
	string rlist = "";
	string list = "";
	struct CTWT {
		float ct;
		float wt;
		CTWT(){ct = 0;wt = 0;}
		CTWT(float c, float w){ct = c;wt = w;}
	};
	map<Item*, CTWT> itemMap;
	int k = 1;
	int num;
	char temp;
	Screen s;
	s.cleartag();
	s.pc("Do you want recipe details included with list?");
	s.pc("Enter y / n");
	cout << s.genout();
	cin >> temp;
	bool rdetails = false;
	if (temp != 'n'){
		rdetails = true;
	}
	for (auto r : *recipies){
		if (rdetails){
			rlist += soutRecip(true, r, k);
		} else {
			rlist += "Recipe " + to_string(k) + ") " + r->name + "\n";
		}
		float ingratio = 1;
		if (r->feeds > 1){
			s.cleartag();
			s.pc("Recipe " + r->name + " feeds " + to_string(r->feeds) + ".");
			s.pc("How many does it need to feed?");
			cout << s.genout();
			cin >> num;
			ingratio = (float)num / (float)r->feeds;
		} else if (r->feeds == 1){
			s.cleartag();
			s.pc("Recipe " + r->name + " only feeds 1 person.");
			s.pc("How many do you need to get?");
			cout << s.genout();
			cin >> num;
			ingratio = (float)num / (float)r->feeds;
		}
		for (auto i : r->ingredients){
			auto& ing = *i.second;
			char in = 'y';
			if (ing.optional){
				s.cleartag();
				s.p("Include optional ingredient " + ing.item->name);
				s.p("in recipe " + r->name + "?");
				s.p("Enter y / n");
				cout << s.genout();
				cin >> in;
			}
			if (in != 'n'){
				Ingredient& ingtoadd = ing;
				if (ing.alt){
					s.cleartag();
					s.p("In recipe " + r->name + ",");
					s.p("Ingredient " + ing.item->name + " can be substituted with");
					s.p("Alternate ingredient " + ing.alt->item->name);
					s.p("Use alternate ingredient? (enter y/n)");
					cout << s.genout();
					cin >> in;
					if (in == 'y')
						ingtoadd = *ing.alt;
				}
				//add to list...
				if (!itemMap.contains(ingtoadd.item)){
					itemMap[ingtoadd.item] = CTWT();
				}
				if (ingtoadd.count > 0){
					itemMap[ingtoadd.item].ct = itemMap[ingtoadd.item].ct + (ingtoadd.count * ingratio);
				}
				if (ingtoadd.weight > 0){
					itemMap[ingtoadd.item].wt = itemMap[ingtoadd.item].wt + (ingtoadd.weight * ingratio);
				}		

				
			}
		}


		k++;
	}
	int cnt = 0;
	float listCost = 0;
	float fullListCost = 0;
	for (auto i : itemMap){
		cnt++;
		Item& itm = *i.first;
		CTWT& ctwt = i.second;
		float totalCt = ctwt.ct, totalWt = ctwt.wt;
		float priceMult = 1;
		if (ctwt.ct > 0 && ctwt.wt > 0){ //possible bug if item ct/wt = -1
			totalCt += (ctwt.wt / (itm.weight / itm.count));
			totalWt += (ctwt.ct * (itm.weight / itm.count));
			priceMult = totalCt / itm.count;
			list += floatToString(totalCt, 2);
			list += " (";
			list += floatToString(totalWt, 2);
			list += " oz)";
		} else if (ctwt.ct > 0){
			list += floatToString(ctwt.ct, 2);
			priceMult = ctwt.ct / itm.count;
		} else if (ctwt.wt > 0){
			priceMult = ctwt.wt / itm.weight;
			list += floatToString(ctwt.wt, 2);
			list += " oz";
		}
		list += " ";
		list += itm.name;
		listCost += itm.minPrice * priceMult;
		fullListCost += itm.minPrice * ((int)priceMult + (priceMult != (float)(int)priceMult));
		if (itm.name.size() < 8)
			list += "\t";
		list += "\t\tfor ~$" + floatToString(itm.minPrice * priceMult, 2);
		list += "\tor ~$" + floatToString(itm.minPrice * ((int)priceMult + (priceMult != (float)(int)priceMult)), 2);
		list += " rounded up.";
		list += "\n";
	}
	list += "\n";
	list += to_string(cnt) + " items in list.\n";
	list += "Total cost of needed items: $";
	list += floatToString(listCost, 2) + "\n";
	list += "Note that this price is calculated using the cost of ";
	list += "the exact amount of each item needed.\n";
	list += "The cost for a full package of everything on the item list";
	list += " is $" + floatToString(fullListCost, 2);
	list += "\n";
	//
	string result = "";
	result += "================ITEM LIST================\n";
	result += list;
	result += "\n===============RECIPE LIST===============\n";
	result += rlist;
	result += "\n";
	result += "List Created w/ assistance from the Grocery List Maker 3000!\n";
	result += "Created by Frank Collebrusco\n";
	result += EMAIL + "\n";
	result += GITHUB_REPO + "\n";
	return result;
}

ListPrompt::ListPrompt(Screen &screen, char& i) : Prompt(screen, i) {ls = "";}
void ListPrompt::prompt(){
	string text;
	string sinput;
	s.clear();
	s.tag();
	s.p("Would you like me to make you a list, or to make your own?");
	s.p("(Enter 'ai' or 'custom', use 'x' to exit.)");
	s.genout();
	do {
		cout << s.out;
		cin >> sinput;
		if (sinput == "ai"){
			AIPrompt ai(s, input);
			ai.prompt();
			if (input == 'c'){
				sinput = "x";
				break;
			}
			ls = ai.genList();
			cout << ls;
			
		} else if (sinput == "custom"){
			AIPrompt ai(s, input);
			ai.cprompt();
			if (input == 'c'){
				sinput = "x";
				break;
			}
			ls = ai.genList();
			cout << ls;
		} else if (sinput == "x") {

		} else {
			s.clear();
			s.tag();
			s.pc("ERROR: Unknown option! Try again..");
			s.nl();
			s.p("Would you like me to make you a list, or make your own?");
			s.p("(Enter 'ai' or 'custom')");
			s.genout();
		}
		//HAVE LIST, FILE IO HERE
		if (ls != ""){
			string filename;
			s.cleartag();
			s.pc("Enter a file name for your list!");
			s.pc("(no spaces)");
			cout << s.genout();
			cin >> filename;
			bool validFile = true;
			do {
				ifstream fin;
				fin.open(filename + ".txt");
				if (!fin.fail()){
					validFile = false;
					s.cleartag();
					s.pc("This name is taken!");
					s.pc("Enter another:");
					cout << s.genout();
					cin >> filename;
				} else {
					validFile = true;
				}
				fin.close();
			} while (!validFile);
			ofstream fout;
			fout.open(filename + ".txt");
			fout << ls;
			fout.close();
			sinput = "x";
		}
	} while (sinput != "x");
	s.clear();
	s.tag();
	s.pops();
	s.genout();
}

TutorialPrompt::TutorialPrompt(Screen &screen, char& i) : Prompt(screen, i) {}
void TutorialPrompt::prompt(){
	string text;
	s.clear();
	s.tag();
	text = "Welcome! This app is designed to help you generate a grocery  ";
	text += "list without much thought. It does this by keeping a list of  ";
	text += "your favorite recipies & items, asking you some questions     about what ";
	text += "you need from the trip, and generating a list for you by picking out ";
	text += "the appropriate ingredients based on your needs. ";
	s.multiLine(text);
	s.nl();
	s.p("Enter '0' to learn how to write recipies,");
	s.p("Enter '1' to learn how to add ingredients to the database,");
	s.p("Enter '2' to learn about generating a grocery list,");
	s.p("Enter any other character to return to main menu.");
	cout << s.genout();
	cin >> input;
	do {
		switch(input){
		case '0':
			s.clear();
			s.tag();
			//   ----------------------------------------------------------------
			text = "This program uses an external text file to store ingredients ";
			text += "and recipies. With this program there is a file called items.txt. ";
			text += "In this file, under the item list is the recipe list.";
			s.multiLine(text);
			s.nl();
			text = "Declare a recipe with ";
			text += "'recipe' followed by 'contains' to begin listing ingredients.";
			s.multiLine(text);
			text = "Ingredients are declared by first listing either the weight or ";
			text += "the number of the item needed, followed by its name as given in the item list.";
			text += " Use 'lbs' or 'oz' to specify by weight, or 'ct' to specify by count. ";
			s.multiLine(text);
			s.p("For example: '0.5 ct white onion' or '16 oz ground beef'");
			text = "If a recipe has more than one ingredient, each subsequent ingredient needs ";
			text += "to be preceeded by 'and' to tell the program that there is more. ";
			text += "An ingredient can be declared optional by adding 'opt' before the ingredient ";
			text += "is enumerated.";
			s.multiLine(text);
			text = "After an ingredient is listed, an alternate replacement ingredient can be listed ";
			text += "by using 'alt' instead of 'and'. For example:";
			s.multiLine(text);
			s.p("...");
			s.p("and opt 1 ct shallot");
			s.p("and 0.5 lbs catfish");
			s.p("alt 0.5 lbs hard tofu");
			s.nl();
			text = "After all ingredients are listed, a few more details can be added about the recipe. ";
			text += "Type 'note' to add instructions. They can be as long and as many lines as you want. End them with '#'";
			s.multiLine(text);
			s.p("E.G: note (instructions go here) #");
			text = "Write 'type' followed by the category of the recipe (entree, snack, desert, etc.)";
			s.multiLine(text);
			s.p("Type 'feeds' followed by how many the recipe feeds (default 1)");
			s.p("Type 'takes' followed by about how many mins it takes to make.");
			s.p("Type 'mess' followed by the mess level the preparation takes.");
			text = "Mess level ranges from 0 to 3, where 0 is no mess, 1 is a light mess, ";
			text += "2 is medium mess and 3 is heavy.";
			s.multiLine(text);
			text = "After all details are given, end with 'named' followed by the name of the recipe.";
			s.multiLine(text);
			s.nl();
			s.p("Examples can be found in the given item file");
			s.nl();
			s.p("Enter '0' to learn how to write recipies,");
			s.p("Enter '1' to learn how to add ingredients to the database,");
			s.p("Enter '2' to learn about generating a grocery list,");
			s.p("Enter any other character to return to main menu.");
			cout << s.genout();
			cin >> input;
			break;
		case '1':
			s.clear();
			s.tag();
			text = "This program uses an external text file to store ingredients ";
			text += "and recipies. With this program there is a file called items.txt. ";
			text += "At the top of the file is a list of all known items as they're found ";
			text += "in the store. All items at least have a name and cost, but can ";
			text += "also have a count, weight, and tags for containing fish, non-fish meat, ";
			text += "shellfish, or for otherwise being non-vegan. ";
			s.multiLine(text);
			s.nl();
			text = "To add items to the list, on a new line, write 'item' followed by a space. ";
			s.multiLine(text);
			s.nl();
			text = "Next, give the item a cost by writing 'costs ' followed by the cost. ";
			s.multiLine(text);
			s.nl();
			text = "To specify a count, write 'count' followed by the count. By default, ";
			text += "the count is assumed to be 1.";
			s.multiLine(text);
			s.nl();
			text = "To specify a weight, write 'weight' followed by the weight. By default, ";
			text += "the weight is assumed to be given in ounces. Write 'lbs' after the weight to specify ";
			text += "pounds.";
			s.multiLine(text);
			s.nl();
			text = "If item contains fish, meat other than fish, shellfish, ";
			text += "or an otherwise non-vegan ingredient (dairy or egg, etc.), ";
			text += "write f, m, sf, or nv respectively. Only one tag is needed for ";
			text += "each item, for example if an item has the meat tag, the non-vegan ";
			text += "tag is not also necessary, the program will know. ";
			s.multiLine(text);
			s.nl();
			text = "The item is named AFTER all the other details are given, by writing ";
			text += "'named' followed by the item name. The item name can be multiple words.";
			s.multiLine(text);
			s.nl();
			s.p("an example would be:");
			s.p("item costs 4.79 weight 1 lbs m named ground beef");
			text = "This tells the program that there is ground beef at the store for ";
			text += "4.79 per lb, and the 'm' indicates that it contains non-fish meat, ";
			text += "making it non vegan, non vegitarian, and non pescatarian.";
			s.multiLine(text);
			s.p("See the given items file for more examples.");
			s.nl();

			s.p("Enter '0' to learn how to write recipies,");
			s.p("Enter '1' to learn how to add ingredients to the database,");
			s.p("Enter '2' to learn about generating a grocery list,");
			s.p("Enter any other character to return to main menu.");
			cout << s.genout();
			cin >> input;
			break;
		case '2':

			break;
		default:
			s.clear();
			s.tag();
			s.pops();
			s.genout();
			break;
		}
	} while (input == '1' || input == '2' || input == '0');
}


static void outRecips(bool d){
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

static void outRecip(bool d, Recipe* r){
	string rName = "";
	for (int i = 0; i < r->name.size(); i++){
		if (r->name.at(i) == ' ')
			rName += " ";
		else
			rName += (char)((int)r->name.at(i) - 32);
	}
	cout << rName << " contains ";
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

	if (d && r->note != "" && r->note != " ") {cout << "\ndirections:" << r->note;}

	cout << "\n\n";
}

static vector<Recipe*>& searchExactRecips(string term){
	vector<Recipe*>* vec = new vector<Recipe*>;
	for (auto recip : state.recips){
		if (term == recip->name){
			vec->push_back(recip);
		}
	}
	return *vec;
}

static void searchPromptCaseS(Screen &s, char& i){
	string text;
	s.cleartag();
	s.p("Enter recipe name or term");
	s.genout();
	// string temp;
	do {
		cout << s.out;
		cin.ignore();
		getline(cin, text);
		// cin >> temp;
		if (text.size() == 1){
			i = text.at(0);
		} else {
			i = 'b';
		}
		auto res = searchPartialRecips(text);
		vector<Recipe*>& results = *res;
		if (results.size() == 0){
			s.cleartag();
			s.p("No recipies found with keyword " + text + " in title!");
			s.p("Enter 's' to search for another recipe,");
			s.p("Enter 'a' to show them all,");
			s.p("Enter 'b' to go back to the main menu");
			s.genout();
		} else {
			s.cleartag();
			string resultStr = "Found " + to_string(results.size()) + " match";
			if (results.size() > 1)
				resultStr += "es";
			resultStr += " to \"" + text += "\":";
			s.p(resultStr);
			int k = 0;
			for (auto r : results){
				s.p(to_string(k) + ") " + r->name);
				k++;
			}
			if (results.size() == 1){
				s.p("Scroll up to see " + results.at(0)->name + " details!");
				outRecip(true, results.at(0));
				s.cleartag();
				s.pc("1 match found! Scroll up to see details.");
				s.p("Enter 's' to search for another recipe,");
				s.p("Enter 'a' to show them all,");
				s.p("Enter 'b' to go back to the main menu");
				s.genout();
			} else {
				s.nl();
				s.p("Enter 0 through " + to_string(results.size() - 1) + " to see details about a result,");
				s.p("Enter 'a' to show details about all search results,");
				s.p("Enter 'b' to go back to the search menu");
				s.genout();
				do {
					cout << s.out;
					cin >> i;
					if (i == 'a') {
						cout << endl;
						for (int j = 0; j < results.size(); j++){
							outRecip(true, results.at(j));
						}
					} else if (i == 's') {

					} else if ((int)i - 48 < 0 || (int)i - 48 > results.size()){
						// i = 'b';
						s.cleartag();
						s.pc("input out of bounds...");
					} else {
						s.p("Scroll up to see " + results.at((int)i - 48)->name + " details!");
						cout << endl;
						outRecip(true, results.at((int)i - 48));
						// i = 'b';
					}
				} while (i != 's' && i != 'b');
				s.cleartag();
				s.p("Enter 's' to search for a specific recipe,");
				s.p("Enter 'a' to show them all,");
				s.p("Enter 'b' to go back to the main menu");
				s.genout();
			}
			// s.cleartag();
			// s.p("Enter recipe name or term");
			// s.genout();
		}
		delete res;
	} while (i != 'b');
}

RecipePrompt::RecipePrompt(Screen &screen, char& i) : Prompt(screen, i) {}
void RecipePrompt::prompt(){
	string text;
	s.cleartag();
	s.p("Enter 's' to search for a specific recipe,");
	s.p("Enter 'a' to show them all,");
	s.p("Enter 'b' to go back to the main menu");
	s.genout();
	do {
		cout << s.out;
		cin >> input;
		switch (input){
		case 'a':
			s.clear();
			s.tag();
			s.p("Would you like to print recipe instructions? (y/n)");
			cout << s.genout();
			cin >> input;
			cout << endl;
			outRecips(input == 'y' || input == 'Y');
			s.clear();
			s.tag();
			s.pc("== Scroll up to see recipe list! ==");
			s.nl();
			s.pops();
			s.genout();
			input = 'b';
			break;
		case 's':
			searchPromptCaseS(s, input);
			if (input == 'b'){
				input = 0;
			}
			break;
		case 'b':
			break;
		default:
			s.cleartag();
			s.pc("Unknown option!!");
			s.p("Enter 's' to search for another recipe,");
			s.p("Enter 'a' to show them all,");
			s.p("Enter 'b' to go back to the main menu");
			s.genout();
			break;
		}
	} while (input != 'b');
}










