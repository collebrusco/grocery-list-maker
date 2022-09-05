#include "AIList.h"
#include <cstdlib>
#include <time.h>
#include <iostream>
#include <map>
using namespace std;
// class AIList {
// private:
// 	State& state;
// public:
// 	AIList(State&);
// 	vector<Recipe*>& genRList();
// 	int bigCooks;
// 	std::vector<int> catPVec;	

// };

static State& state = State::getInstance();

AIList::AIList(State& s) : state(s) {
	bigCooks = -1;
}

template <typename T>
static void debugat(vector<T> v, int idx, string name){
	cout << "at called at " << idx << " on vec " << name << " w/ size " << v.size() << "\n";
}

// struct TypeRecipies {
// 	struct typeRecipe {
// 		Recipe* recipOfThisType;
// 		bool used;
// 		typeRecipe(Recipe* r){
// 			used = false;
// 			recipOfThisType = r;
// 		}
// 	};
// 	vector<typeRecipe> recipsOfThisType;
// 	TypeRecipies(string cat){
// 		for (auto r : state.recips){
// 			if (r->category == cat){
// 				recipsOfThisType.push_back(typeRecipe(r));
// 			}
// 		}
// 	}
// };

static void popValue(vector<int> v, int n){
	for (int i = 0; i < v.size(); i++){
		if (v.at(i) == n){
			v.at(i) = v.at(v.size() - 1);
			v.pop_back();
			return;
		}
	}
}

void AIList::removeInt(vector<int> &v, int tar){
	for (int i = 0; i < v.size(); i++){
		if (v.at(i) == tar){
			for (int k = i; k < v.size() - 1; k++){
				v.at(k) = v.at(k + 1);
			}
			v.pop_back();
			return;
		}
	}
}

vector<Recipe*>* AIList::genRList(){
	vector<Recipe*>* rl = new vector<Recipe*>;
	vector<Recipe*>& rlist = *rl;
	//FIRST ENTREES
	{
		//LIST KNOWN RECIPIES W/ big cooks if they exist

		if (bigCooks > -1){
			vector<Recipe*> recipsOfThisType;
			vector<int> availableIdxs;
			for (int j = 0; j < state.recips.size(); j++){
				if (state.recips.at(j)->category == *state.categories.at(0)
					&& (state.recips.at(j)->time > 15 || state.recips.at(j)->mess > 1)) {
					recipsOfThisType.push_back(state.recips.at(j));
				}
			}
			for (int j = 0; j < recipsOfThisType.size(); j++){
				availableIdxs.push_back(j);
			}
			for (int j = 0; j < bigCooks; j++){
				int randx;
				if (availableIdxs.size() > 1){
					//pick from avail left...
					// cout << availableIdxs.size() << " avalIdxs\n";
					// cout << "case 1: ";
					randx = availableIdxs.at(rand() % availableIdxs.size());
					removeInt(availableIdxs, randx);
				} else if (availableIdxs.size() == 1){
					//pick single remaining recip...
					// cout << "case 2: ";
					randx = availableIdxs.at(0);
					removeInt(availableIdxs, randx);
				} else {
					//pick random from all
					// cout << "case 3: ";
					randx = rand() % recipsOfThisType.size();
				}
				// cout << randx << "\n\n";
				rlist.push_back(recipsOfThisType.at(randx));
			}
		}

		vector<Recipe*> recipsOfThisType;
		vector<int> availableIdxs;
		for (int j = 0; j < state.recips.size(); j++){
			if (state.recips.at(j)->category == *state.categories.at(0)
				&& (state.recips.at(j)->time <= 15 && state.recips.at(j)->mess <= 1)) {
				recipsOfThisType.push_back(state.recips.at(j));
			}
		}
		for (int j = 0; j < recipsOfThisType.size(); j++){
			availableIdxs.push_back(j);
		}





		for (int j = 0; j < catPVec.at(0) - (bigCooks * (bigCooks != -1)); j++){
			for (auto i : availableIdxs){
				cout << i << " ";
			}
			cout << "    <- avail idxs\n";
			int randx;
			if (availableIdxs.size() > 1){
				// pick from avail left...
				cout << availableIdxs.size() << " avalIdxs\n";
				cout << "case 1: ";
				randx = availableIdxs.at(rand() % availableIdxs.size());
				removeInt(availableIdxs, randx);
			} else if (availableIdxs.size() == 1){
				// pick single remaining recip...
				cout << "case 2: ";
				randx = availableIdxs.at(0);
				removeInt(availableIdxs, 0);
			} else {
				// pick random from all
				cout << "case 3: ";
				randx = rand() % recipsOfThisType.size();
			}
			cout << randx << "\n\n";
			rlist.push_back(recipsOfThisType.at(randx));
		}		
	}

	//FOR EACH KNOWN CATEGORY...
	for (int k = 1; k < state.categories.size(); k++){
		srand(time(NULL));
		//LIST KNOWN RECIPIES OF THIS CATEGORY...
		vector<Recipe*> recipsOfThisType;
		vector<int> availableIdxs;
		for (int j = 0; j < state.recips.size(); j++){
			if (state.recips.at(j)->category == *state.categories.at(k)){
				recipsOfThisType.push_back(state.recips.at(j));
			}
		}
		for (int j = 0; j < recipsOfThisType.size(); j++){
			availableIdxs.push_back(j);
		}
		//available indexes has idxs of recip OTTs to be popped as theyre
		//used, then the running size is how many left, and it can be picked
		//from & that fed to OTT array to get random index remaining,
		//which must then be popped. func for this?

		
		cout << endl;
		srand(time(NULL));
		//RUNS NUM OF TIMES AS CATEGORY IS REQUESTED
		for (int j = 0; j < catPVec.at(k); j++){
			// for (auto i : availableIdxs){
			// 	cout << i << " ";
			// }
			// cout << "    <- avail idxs\n";
			int randx;
			if (availableIdxs.size() > 1){
				//pick from avail left...
				// cout << availableIdxs.size() << " avalIdxs\n";
				// cout << "case 1: ";
				randx = availableIdxs.at(rand() % availableIdxs.size());
				removeInt(availableIdxs, randx);
			} else if (availableIdxs.size() == 1){
				//pick single remaining recip...
				// cout << "case 2: ";
				randx = availableIdxs.at(0);
				removeInt(availableIdxs, 0);
			} else {
				//pick random from all
				// cout << "case 3: ";
				randx = rand() % recipsOfThisType.size();
			}
			// cout << randx << "\n\n";
			rlist.push_back(recipsOfThisType.at(randx));
		}		





		// //PARALLEL ARRAY OF BOOLS FOR HAVE THEY BEEN USED
		// const int catRecipSize = recipsOfThisType.size();
		// bool usedRecips[catRecipSize];
		// for (int i = 0; i < catRecipSize; i++){usedRecips[i] = false;}
		// int numAvailRecipsOfThisType = recipsOfThisType.size();
		// //EMPLACE REQUESTED NUMBER OF RECIPIES
		// for (int j = 0; j < catPVec.at(k); j++){



		// 	// random index
		// 	int randx = (rand() % (recipsOfThisType.size() - 1));
		// 	if (recipsOfThisType.size() == 1){
		// 		randx = 0;
		// 	} else if (recipsOfThisType.size() != 0) {
		// 		if (numAvailRecipsOfThisType > 0) {

		// 		}
		// 	}

		// 	// debugat(recipsOfThisType, randx, "recipsOfThisType");
		// 	if (usedRecips[randx] == false){
		// 		if (recipsOfThisType.size() != 0){
		// 			rlist.push_back(recipsOfThisType.at(randx));	
		// 			usedRecips[randx] = true;		
		// 		}
		// 	}
		// }
	}
	return rl;
}














