#include "Screen.h"
#include "Item.h"
#include "Recipe.h"
#include "State.h"
#include "AIList.h"
#include <iostream>
#include <vector>

class Prompt {
protected:
	Screen &s;
	char &input;
public:
	Prompt(Screen&, char&);
	virtual void prompt() = 0;
};

class TutorialPrompt : public Prompt {
private:

public:
	TutorialPrompt(Screen&, char&);
	void prompt() override;
};

class ListPrompt : public Prompt {
private:
public:
	string ls;
	ListPrompt(Screen&, char&);
	void prompt() override;
};

class AIPrompt : public Prompt {
private:
	vector<Recipe*>* recipies;
	vector<Item*> items;
public:
	AIPrompt(Screen&, char&);
	void prompt() override;
	void cprompt();
	void popRecip(vector<Recipe*>*, int);
	std::string genList();
};

class RecipePrompt : public Prompt {
private:
public:
	string ls;
	RecipePrompt(Screen&, char&);
	void prompt() override;
};