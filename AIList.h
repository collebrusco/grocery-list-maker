#include <vector>
#include <string>
#include "State.h"

class AIList {
private:
	State& state;
	void removeInt(std::vector<int>&, int);
public:
	AIList(State&);
	vector<Recipe*>* genRList();
	int bigCooks;
	std::vector<int> catPVec;	

};
