#include "State.h"

State::State(){}

Item* State::getItem(string name){
	for (auto i : items){
		if (i->name == name){
			return i;
		}
	}
	return nullptr;
}

Recipe* State::getRecipe(string name){
	for (auto r : recips){
		if (r->name == name){
			return r;
		}
	}
	return nullptr;
}

State& State::getInstance(){
		static State instance;
		return instance;
	}

State::~State(){
	for (auto i : items){
		delete i;
	}
	for (auto r : recips){
		delete r;
	}
	for (auto c : categories){
		delete c;
	}
}