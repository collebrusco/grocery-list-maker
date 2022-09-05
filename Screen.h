#pragma once
#include <iostream>
#include "flib/Queue.tpp"

class Screen {
private:
	Queue<std::string> *lines;
	int qsize;
	void enq(std::string l);
public:
	std::string out;
	Screen();
	~Screen();
	void clear();
	void tag();
	void cleartag();
	void tag(std::string);
	void p(std::string line);
	void pc(std::string line);
	void multiLine(std::string);
	void nl();
	void pops();
	std::string genout();
};