# grocery-list-maker
Command line app that keeps a recipe book and uses it to help create grocery lists.
Created out of personal necessity: I always forget what recipes and snacks I know of when I'm to go to the store, so I wanted a little tool to remember them for me and pick some out for a grocery list based on how many meals I need, snacks I want, and how much cooking I want to do. Created by Frank Collebrusco mostly during the summer of 2022.

Mostly complete instructions can be found in program by entering '?' on main menu.

# Building
This was written on macOS using the unix terminal to develop, build and use the program. It should build by navigating the terminal to a directory containing the entire repo and issuing "make". Note that the default makefile uses the clang compiler, if you need to use gcc, go into the makefile and replace clang++ with g++. The items.txt file should be in the directory with all the source & the directory the terminal is running from in order to be seen.
For windows: good luck! There's no OS specific code here, but I haven't tried building on windows yet.

# How to Use
To store information about purchasable food items and recipies, the program will read from an external file called "items.txt" located in the project directory. I've yet to wrap this app into a nice container and support things like a custom filepath or name for the items file, for now, it's items.txt located in the project directory.

# Items
The syntax for declaring an item is as follows:
First, write "item" to indicate that details about a new item will follow.
Write "costs" followed by the price of the item to declare a price.
Write "weight" followed by the weight of the item in ounces if applicable. A weight doesn't need to be specified.
Write "count" followed by how many of the item is included to specify a count, default is 1.
Write 'm', 'f', 'sf', or 'nv' to specify that the item contains non-fish meat, fish, shellfish, or is otherwise non vegan respectively.
Finally, write "named" followed by the name of the item. The name can be multiple words but must end with a new line to indicate the end of the name. Many examples can be found in the default items.txt.

# Recipes
Recipies are essentially lists of different quantities of items, packaged with some additional data such as category, how many it feeds, how long it takes, how much of a mess it makes, and instructions.
To declare a recipe, start with the word "recipe" to indicate the beginning of a recipe. Follow it with "contains" to indicate the start of the ingredient list.
Each ingredient starts with a number and a unit, specifying how much of the ingredient is needed. Units are "ct" for count, "oz" and "lbs" for weight. Next, write the name of the ingredient as specified in the item list, followed by a new line. Preface each ingredient after the first with "and" to tell the program there is more.
Ex:
0.5 ct onion
and 8 oz refried beans

Ingredients can be delared optional by adding "opt" after "and" and before the number and unit.
If an ingredient can be substituted for something else, on the line directly after said ingredient, start with "alt" instead of "and" and declare the alt ingredient.
Ex)
opt 1 oz soy sauce
1 lbs chicken breast
alt 1 lbs tofu

After all the ingredients are listed, there are a couple more things to specify about the recipe.
Write "note" followed by the instructions of the recipe. IMPORTANT! Place a pound sign ('#') at the end of your instructions to tell the program that's the end. This is not ideal but is done so the instructions can contain as many new lines as needed.
Write "type" followed by the category of the recipe. You are allowed to enumerate as many and whatever categories you'd like, the program will read them and ask how many of each you want when preparing your list. It is recommended you use the "entree" category to denote anything you cook at home, as the program will look for the entree category and use it to help you make your list, but this is the only recipe category that the program is aware of. Other than that, you create and control the categories.
Write "feeds" followed by an integer to enumerate how many people the recipe can feed.
Write "takes" followed by an integer number of minutes the recipe roughly takes to prepare.
Write "mess" followed by an integer 0-4 to represent how messy the recipe is.
Here is the enum:
enum MESS_LEVEL {
	NONE = 0,
	LOW,
	MEDIUM,
	HIGH,
	EXTREME,
};
Finally, at the end of the recipe, name it by typing "named" followed by the multi word name of the recipe, ended with a new line. 
The default items.txt has many many examples.

# End the items.txt file with "OUT" in all caps to tell the program that there is no more to read!

# Troubleshooting item file
There may arise a situation in which the program hits an infinite loop during the file read phase. When the program is run, the first thing it does is look for the item file, and read all the items and recipies into memory. This involves a number of while loops that look for some terminating condition, such as looking for "named" at the end of each item and recipe to indicate the end of that item/recipe, read the name, and move on. If you hit this infinite loop, make sure there aren't any half completed recipies or spelling errors on keywords. If the item file isn't found, there should be an error for that printed to console. During the recipe read phase, the program should also print an error if it cant find the name of an ingredient requested in a recipe in the item list. Make sure the items file ends with "OUT" in all caps.
For any other errors or help if anyone actually uses this, email me! I'd love to help sort it out or fix any bugs.
collebrusco.frank@gmail.com

# Creating Grocery Lists
I'll be brief here because instructions are in the program and this part is more user - friendly. Follow the prompts to create a list in the program. There are two types of list: AI lists and Custom lists. AI lists will ask you about what you need for the trip, pick recipes at random, let you re-roll the dice until you like it, and then add/remove recipes manually as desired before finalizing the list.
Custom lists simply start you with a blank list and let you add recipes you want by search.

When you're happy with the selected recipes, the program goes through them and asks how many people they each need to feed, asks which optional ingredients you want, and asks if you want to use any alternate ingredients. Finally, it adds all necessary items to a list, and writes it to a file. It will optionally write any recipe instructions to the file as well, as well as rough information about the price of the list. Then your list is done! Cross things off you already have, and go shoppin'!

# Things the program still needs
- The ability to create lists w/ dietary restrictions. The program already knows what items are vegan or not, vegitarian or not, contain shellfish, etc., as long as the item file was set up accurately. So all I need here is the extra option in the UI of the list making process.

- The ability to add/remove single ingredients at the end of list creation. I may never get around to this as it may just be more cumbersome to use than simply deleting lines from the text file if needed.

- Hooking up to a grocery store database. This I probably won't get to but I recognize that that would be the way to really do this program, that would give it live price data, all nutrition data, etc, and make it more user friendly as users wouldn't have to add every item manually.
