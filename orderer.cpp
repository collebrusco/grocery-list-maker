#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

#include "ItemReader.h"
#include "State.h"

using namespace std;

State state;
string file = "items.txt";

void out(string msg){
	cout << msg << endl;
}

template<class T>
void swap(vector<T> &vec, int a, int b){
	T temp = vec.at(a);
	vec.at(a) = vec.at(b);
	vec.at(b) = temp;
}

int main(){
	cout << "Reading....\n";
	ItemReader ireader(&state.items);
	if (!ireader.focus(file)){
		cout << "no file found!!!\n";
	}
	ireader.read();
	ireader.close();
	cout << "Read " << state.items.size() << " items.\n";

	cout << "Advancing past items...\n";
	ifstream fin;
	fin.open("items.txt");
	string CONTENTS = "";
	bool moreItems = true;
	while (moreItems){
		string temp;
		string t;
		fin >> temp;
		out("enter outer loop...");
		if (temp == "item"){
			//file is declaring item, read details:
			fin >> temp;

			while (temp != "named"){
				out("in inner loop..");
				//costs keyword
				if (temp == "costs" || temp == "cost"){
					fin >> t;
				}
				if (temp == "to"){
					fin >> t;
				}
				if (temp == "count"){
					fin >> t;
				}
				if (temp == "weight"){
					fin >> t;
				}
				if (temp == "lbs"){
					
				}
				//other keywords...
				fin >> temp;
			}
			out("exited inner loop..=======");
			//name...
			getline(fin, t);
		} else {moreItems = false;}
	}
	cout << "Copying up file contents...\n";
	getline(fin, CONTENTS, '$');
	fin.close();
	// out(CONTENTS);





	ofstream fout;
	fout.open(file);
	if (fout.fail()){
		cout << "failed to open fout!!!\n";
	}

	cout << "Sorting...\n";
	//SORT
	for (int i = 1; i < state.items.size(); i++){
		for (int j = i; j > 0; j--){
			if (state.items.at(j)->name < state.items.at(j-1)->name){
				swap<Item*>(state.items, j, j - 1);
			}
		}
	}
	cout << "Writing...\n";
	//OUTPUT
	for (auto i : state.items){
		out("entering items outer loop...\n");
		fout << "item costs ";
		fout << setw(5) << left;
		fout << i->minPrice << " ";
		int tabs = 3;
		if (i->minPrice != i->maxPrice){
			fout << "to " << i->maxPrice << " ";
			// tabs--;
		}
		if (i->count > 1){
			fout << "count " << i->count << " ";
			tabs -= (1 * (i->count >= 10));
		}
		if (i->weight > 0){
			fout << "weight " << i->weight << " ";
			tabs--;
		}
		if (i->meat){fout << "m ";}
		if (i->fish){fout << "f ";}
		if (i->shellfish){fout << "sf ";}
		if (i->nv){fout << "nv ";}

		if ((i->count > 1) && (i->weight > 0)){tabs--;}
		fout << setw(0);
		for (int j = 0; j < tabs && !(tabs < 1); j++) {
			out("in tab output loop...\n");
			fout << "	";
		}
		fout << "named " + i->name + "\n";
	}
	fout << "\n\n\n" + CONTENTS;


	cout << "Done.\n";
	return 0;
}