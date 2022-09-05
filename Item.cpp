#include "Item.h"

Item::Item(){
	minPrice = -1;
	maxPrice = -1;
	name = "";
	count = 1;
	weight = -1;
}
Item::Item(std::string n){
	name = n;
	count = 1;
	minPrice = -1;
	maxPrice = -1;
	weight = -1;
}
Item::Item(std::string n, float p){
	name = n;
	minPrice = p;
	maxPrice = p;
	count = 1;
	weight = -1;
}
Item::Item(std::string n, float mp, float Mp){
	name = n;
	minPrice = mp;
	maxPrice = Mp;
	count = 1;
	weight = -1;
}
float Item::pricePer(){
	return minPrice / count;
}