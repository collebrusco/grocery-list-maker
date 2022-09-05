#makefile

a.out: GroceryDriver.o Item.o ItemReader.o Recipe.o RecipeReader.o State.o Screen.o Prompts.o AIList.o
	g++ GroceryDriver.o Item.o ItemReader.o Recipe.o RecipeReader.o State.o Screen.o Prompts.o AIList.o -std=c++14

GroceryDriver.o: GroceryDriver.cpp ItemReader.h RecipeReader.h Recipe.h State.h flib/Queue.tpp
	g++ -c GroceryDriver.cpp -std=c++14

Item.o: Item.cpp Item.h
	g++ -c Item.cpp -std=c++14

ItemReader.o: ItemReader.cpp ItemReader.h Item.h
	g++ -c ItemReader.cpp -std=c++14

Recipe.o: Recipe.cpp Recipe.h Item.h
	g++ -c Recipe.cpp -std=c++14

RecipeReader.o: RecipeReader.cpp RecipeReader.h Recipe.h Item.h State.h
	g++ -c RecipeReader.cpp -std=c++14

State.o: State.cpp State.h Recipe.h Item.h
	g++ -c State.cpp -std=c++14

Screen.o: Screen.cpp Screen.h flib/Queue.tpp
	g++ -c Screen.cpp -std=c++14

Prompts.o: Prompts.cpp Prompts.h Screen.h flib/Queue.tpp
	g++ -c Prompts.cpp -std=c++20

AIList.o: AIList.cpp AIList.h State.h
	g++ -c AIList.cpp -std=c++14
