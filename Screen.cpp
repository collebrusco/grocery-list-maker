#include <iostream>
#include "Screen.h"
using namespace std;

void Screen::enq(string l){
	if (lines->isFull()){
		Queue<string> *newq = new Queue<string>(qsize *= 2);
		while (!lines->isEmpty()){
			newq->enqueue(lines->dequeue());
		}
		delete lines;
		lines = newq;
	}
	assert(l.size() < 63);
	lines->enqueue(l);
}

Screen::Screen(){
	lines = new Queue<string>(1);
	out = "";
	qsize = 1;
}

Screen::~Screen(){
	delete lines;
}

void Screen::clear(){
	delete lines;
	qsize = 1;
	lines = new Queue<string>(qsize);
	out = "";
}

void Screen::tag(){
	this->pc("Grocery List Maker 3000");
	this->pc("-------------------------------------------");
}

void Screen::tag(string tag){
	this->pc(tag);
	this->pc("-------------------------------------------");
}

void Screen::cleartag(){
	this->clear();
	this->tag();
}

void Screen::p(string line){
	this->enq(line);
}

void Screen::pc(string line){
	assert(line.size() < 63);
	string centerLine = "";
	int spaces = (62 - line.size()) / 2;
	for (int i = 0; i < spaces; i++){
		centerLine += " ";
	}
	centerLine += line;
	for (int i = 0; i < spaces; i++){
		centerLine += " ";
	}
	this->enq(centerLine);
}

void Screen::multiLine(string copy){
	int lines = copy.size() / 62;
	if (copy.size() / 62.0f > (float)lines){
		lines++;
	}
	for (int i = 0; i < lines; i++){
		string line = "";
		for (int j = 0; j < 62; j++){
			if (copy.size() > ((62 * i) + j)){
				line += copy.at(((62 * i) + j));
			}
		}
		assert(line.size() < 63);
		this->p(line);
	}
}

void Screen::nl(){
	this->p("");
}

void Screen::pops(){
	this->p("Enter 'n' to create a new list,");
	this->p("Enter 'r' to search or list known recipies,");
	this->p("Enter 'i' to list known store items,");
	// s.p("enter 'b' to go back to main menu,");
	this->p("Enter '?' for instructions,");
	this->p("Enter 'x' to exit the program.");
}

string Screen::genout(){
	out += " /==============================================================\\\n";
	this->enq("");
	while (!lines->isEmpty()){
		out += "|";
		int spaces = 64;
		if (!lines->isEmpty()){
			string line = lines->dequeue();
			spaces -= line.size();
			out += line;
		}
		for (int i = 0; i < spaces; i++){
			out += " ";
		}
		out += "|\n";
	}
	out += " \\==============================================================/\n";
	return out;
}
