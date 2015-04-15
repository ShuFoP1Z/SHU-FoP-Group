//SKELETON PROGRAM
//---------------------------------
//include libraries
//include standard libraries
#include <iostream >						//for output and input: cin >> and cout <<
#include <iomanip>							//for formatted output in 'cout'
#include <conio.h>							//for getch()
#include <string>							//for string
#include <vector>							//for vectors
using namespace std;

//include our own libraries
#include "RandomUtils.h"					//for Seed, Random
#include "ConsoleUtils.h"					//for Clrscr, Gotoxy, etc.

//---------------------------------
//define constants
//---------------------------------
//define global constants
//defining the size of the grid
const int SIZEY(12);						//vertical dimension
const int SIZEX(20);						//horizontal dimension
//defining symbols used for display of the grid and content
const char SPOT('@');						//spot
const char TUNNEL(' ');						//open space
const char WALL('#');						//border
const char HOLE('O');						//hole
const char PILL('*');						//pill
const char ZOMBIE('Z');						//Zombie

//defining the command letters to move spot on the maze
const int  UP(72);							//up arrow
const int  DOWN(80);						//down arrow
const int  RIGHT(77);						//right arrow
const int  LEFT(75);						//left arrow
//defining the other command letters
const char QUIT('Q');						//end the game

//data structure to store data for a grid item
struct Item
{
	char symbol;						//symbol on grid
	int x, y;								//coordinates
};

struct ChangingItem
{
	const char symbol;						//symbol on grid
	int x, y;								//coordinates
	bool isBeingRendered;					//boolean to determine whether its being rendered
};

//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)

	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<ChangingItem>& pills, vector<ChangingItem>& zombies);
	bool wantToQuit(int k);
	bool isArrowKey(int k);
	bool outOfLives(int lives);
	bool outOfPills(int pillsRemaining);
	int  getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, vector<Item> holes, int k, int& lives, string& mess, vector<ChangingItem>& pills, int& pillsRemaining, vector<ChangingItem>& zombies);
	void renderGame(const char g[][SIZEX], string mess, int lives);
	void endProgram(int lives, int key, int pillsRemaining);

	//local variable declarations 
	char grid[SIZEY][SIZEX];								//grid for display
	int lives(3);											//The number of lives spot has
	int pillsRemaining(5);									//The number of pills still being shown
	Item spot = { SPOT };									//Spot's symbol and position (0, 0) 
	Item hole = { HOLE };									//Hole's symbol and position (0, 0)

	ChangingItem pill = { PILL };									//Pill's symbol and position (0, 0)
	vector <Item> holes(12, hole);							//Creates a vector of holes, with each element being initialised as hole 
	vector <ChangingItem> pills(5, pill);							//Creates a vector of pills, with each element being initialised as pills
	ChangingItem zombie = { ZOMBIE };								//Zombies symbol and position (0, 0)
	vector<ChangingItem> zombies(4, zombie);				     	//Initialise a vector of zombies, each element will be initialised as zombie

	string message("LET'S START...      ");					//current message to player

	bool running = true;

	//action...

	initialiseGame(grid, spot, holes, pills, zombies);						//initialise grid (incl. walls and spot)

	int key(' ');											//create key to store keyboard events
	do {
		renderGame(grid, message, lives);					//render game state on screen
		message = "                    ";					//reset message
		key = getKeyPress();								//read in next keyboard event
		if (isArrowKey(key))
			updateGame(grid, spot, holes, key, lives, message, pills, pillsRemaining, zombies);
		else
			message = "INVALID KEY!        ";				//set 'Invalid key' message

		if (wantToQuit(key))								//if player wants to quit
			running = false;
		if (outOfLives(lives))								//if player is out of lives
			running = false;
		if (outOfPills(pillsRemaining))
			running = false;
	} while (running);

	endProgram(lives, key, pillsRemaining);										//display final message
	return 0;
} //end main


void updateGame(char grid[][SIZEX], Item& spot, vector<Item> holes, int key, int& lives, string& message, vector<ChangingItem>& pills, int& pillsRemaining, vector<ChangingItem>& zombies)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, int& lives, string& mess, vector<ChangingItem>& pills, int& pillsRemaining);
	void updateZombieCoordinates(const char g[][SIZEX], vector<ChangingItem>& zombies, Item spot, int& lives, string& message);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> holes, vector<ChangingItem> pills, vector<ChangingItem> zombies);

	updateSpotCoordinates(grid, spot, key, lives, message, pills, pillsRemaining);	//update according to key
	updateZombieCoordinates(grid, zombies, spot, lives, message);	//update the zombie position based on spots location
	updateGrid(grid, spot, holes, pills, zombies);							//update grid information
}
//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<ChangingItem>& pills, vector<ChangingItem>& zombies)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);
	void setHoleInitialCoordinates(vector<Item>& holes, Item& spot);
	void setPillsInitialCoordinates(vector<Item>& holes, Item& spot, vector<ChangingItem>& pills);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placePills(char gr[][SIZEX], vector<ChangingItem> pills);

	void setZombieInitialCoordinates(vector<ChangingItem>& zombies);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placeZombies(char gr[][SIZEX], vector<ChangingItem> zombies);

	Seed();													//seed random number generator

	//do while?
	setGrid(grid);											//reset empty grid
	setSpotInitialCoordinates(spot);						//initialise spot position
	placeSpot(grid, spot);									//set spot in grid
	setHoleInitialCoordinates(holes, spot);					//intiialise holes position
	placeHoles(grid, holes);								//set holes in grid
	setPillsInitialCoordinates(holes, spot, pills);			//initialise pills position
	placePills(grid, pills);								//set pills in grid

	setZombieInitialCoordinates(zombies);					//setup the positions of each zombie
	placeZombies(grid, zombies);							//place the zombies in the grid
} //end of initialiseGame

void setSpotInitialCoordinates(Item& spot)
{ //set spot coordinates inside the grid at random at beginning of game
	spot.y = Random(SIZEY - 2);								//vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = Random(SIZEX - 2);								//horizontal coordinate in range [1..(SIZEX - 2)]
} //end of setSpotInitialCoordinates

void setHoleInitialCoordinates(vector<Item>& holes, Item& spot)
{ //set hole coordinates inside the grid at random at beginning of game
	for (int i = 0; i < holes.size(); ++i)
	{
		holes[i].y = Random(SIZEY - 2);						//vertical coordinate in range [1..(SIZEY - 2)]
		holes[i].x = Random(SIZEX - 2);						//horizontal coordinate in range [1..(SIZEX - 2)]

		if (holes[i].y == spot.y && holes[i].x == spot.x)	//if a hole is in the same place as spot
			--i;											//then decrement i so the hole is moved somewhere else
	}
} //end of setHoleInitialCoordinates


void setPillsInitialCoordinates(vector<Item>& holes, Item& spot, vector<ChangingItem>& pills)
{ //set the pills coordinates inside the grid randomly at the beginning of a game, checking theyre not on a taken space)
	for (int i = 0; i < pills.size(); ++i)
	{
		pills[i].y = Random(SIZEY - 2);						//vertical coordinate in range [1..(SIZEY - 2)]
		pills[i].x = Random(SIZEX - 2);						//horizontal coordinate in range [1..(SIZEX - 2)]
		pills[i].isBeingRendered = true;					//set to draw the pill

		if (pills[i].y == spot.y && pills[i].x == spot.x)	//if a pill is in the same place as spot
			--i;											//then decrement i so the pill is moved somewhere else

		for (int c = 0; c < holes.size(); ++c)						//for every hole
			if (pills[i].y == holes[c].y && pills[i].x == holes[c].x)	//check if the new pill will be in the same space as a hole
			--i;											//if it is remove that pill to create a new one
	}
} //end of setPillsInitialCoordinates

void setZombieInitialCoordinates(vector<ChangingItem>& zombies)
{
	//Will set up all 4 zombies to spawn in a corner
	void resetZombiePosition(vector<ChangingItem>& zombies, int arrayIndex);

	for (int i = 0; i < zombies.size(); ++i)
	{
		resetZombiePosition(zombies, i);
	}
}//end of setZombieInitialCoordinates

void setGrid(char grid[][SIZEX])
{ //reset the empty grid configuration
	for (int row(0); row < SIZEY; ++row)				//for each column
	{
		for (int col(0); col < SIZEX; ++col)			//for each col
		{
			if ((row == 0) || (row == SIZEY - 1))		//top and bottom walls
				grid[row][col] = WALL;					//draw a wall symbol
			else
				if ((col == 0) || (col == SIZEX - 1))	//left and right walls
					grid[row][col] = WALL;				//draw a wall symbol
				else
					grid[row][col] = TUNNEL;			//draw a space
		} //end of row-loop
	} //end of col-loop
} //end of setGrid

void placeSpot(char gr[][SIZEX], Item spot)
{ //place spot at its new position in grid
	gr[spot.y][spot.x] = spot.symbol;
} //end of placeSpot

void placeHoles(char gr[][SIZEX], vector<Item> holes)
{ //place holes at their new positions in grid
	for (int i = 0; i < holes.size(); ++i)
		gr[holes[i].y][holes[i].x] = holes[i].symbol;
} //end of placeHoles


void placePills(char gr[][SIZEX], vector<ChangingItem> pills)
{ //place pills at their new positions in grid
	for (int i = 0; i < pills.size(); ++i)					//for every pill
		if (pills[i].isBeingRendered == true)				//check to see if that pill has already been eaten
			gr[pills[i].y][pills[i].x] = pills[i].symbol;	//if it hasn't, place that pill in the grid
} //end of placePills


void placeZombies(char gr[][SIZEX], vector<ChangingItem> zombies)
{
	for (int i = 0; i < zombies.size(); ++i)
	{
		gr[zombies[i].y][zombies[i].x] = zombies[i].symbol; //Places a zombie symbol at the x & y of each zombie index in the vector
	}
}//end of placeZombies

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], Item spot, vector<Item> holes, vector<ChangingItem> pills, vector<ChangingItem> zombies)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char g[][SIZEX], vector<Item> holes);
	void placePills(char g[][SIZEX], vector<ChangingItem> pills);
	void placeZombies(char g[][SIZEX], vector<ChangingItem> zombies);

	setGrid(grid);							//reset empty grid
	placeHoles(grid, holes);				//set holes in grid
	placePills(grid, pills);				//set pills in grid
	placeSpot(grid, spot);					//set spot in grid
	placeZombies(grid, zombies);			//set zombies in the grid
} //end of updateGrid

//---------------------------------------------------------------------------
//----- move the spot
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, int& lives, string& mess, vector<ChangingItem>& pills, int& pillsRemaining)
{ //move spot in required direction
	void setKeyDirection(int k, int& dx, int& dy);
	void removePill(vector<ChangingItem>& pills, Item sp, string& message, int& pillsRemaining);

	//calculate direction of movement required by key - if any
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);			//find direction indicated by key
	//check new target position in grid 
	//and update spot coordinates if move is possible
	const int targetY(sp.y + dy);
	const int targetX(sp.x + dx);
	switch (g[targetY][targetX])
	{//...depending on what's on the target position in grid...
	case PILL:
		sp.y += dy;							//go in that Y direction
		sp.x += dx;							//go in that X direction
		++lives;							//add a life
		removePill(pills, sp, mess, pillsRemaining);		//remove the pill
		break;
	case HOLE:								//can move
		--lives;
		break;
	case TUNNEL:
		sp.y += dy;							//go in that Y direction
		sp.x += dx;							//go in that X direction
		break;
	case WALL:								//hit a wall and stay there
		cout << '\a';						//beep the alarm
		mess = "CANNOT GO THERE!    ";
		break;
	}
} //end of updateSpotCoordinates
void updateZombieCoordinates(const char g[][SIZEX], vector<ChangingItem>& zombies, Item spot, int& lives, string& message)
{
	void resetZombiePosition(vector<ChangingItem>& zombies, int arrayIndex); //Will reset the zombies position 
	int dx(0), dy(0); //The maximum amount the player can move by
	int displaceX(0), displaceY(0); //Distance in a vector form from the zombie

	for (int i = 0; i < zombies.size(); ++i)
	{
		displaceX = (spot.x - zombies[i].x); //Determine whether a positive dx is needed or a negative
		displaceY = (spot.y - zombies[i].y); //Determine whether a positive dy is needed or a negative
		
		if (displaceX != 0  && displaceY != 0)
		{
			//
			dx = displaceX / abs(displaceX); //Get the positive or negative direction in the horizontal axis
			dy = displaceY / abs(displaceY); //Get the positive or negative direction in the vertical axis
		}
		
		const int targetX(zombies[i].x + dx);
		const int targetY(zombies[i].y + dy);

		switch (g[targetY][targetX])
		{
		case PILL:
		case TUNNEL:
			zombies[i].x += dx; 
			zombies[i].y += dy;
			break; 
		case HOLE: 
			resetZombiePosition(zombies, i);
			break;
		case ZOMBIE:
			resetZombiePosition(zombies, i); //Prevent zombie stacking
			break;
		case SPOT:
			--lives; 
			resetZombiePosition(zombies, i);
			break;
		}
	}

	
}//end of updateZombieCoordinates
void resetZombiePosition(vector<ChangingItem>& zombies, int arrayIndex) 
{
	switch (arrayIndex)
	{
	case 0:
		zombies[arrayIndex].x = 1;
		zombies[arrayIndex].y = 1;
		break;
	case 1:
		zombies[arrayIndex].x = (SIZEX - 2);
		zombies[arrayIndex].y = 1;
		break;
	case 2:
		zombies[arrayIndex].x = 1;
		zombies[arrayIndex].y = (SIZEY - 2);
		break;
	case 3:
		zombies[arrayIndex].x = (SIZEX - 2);
		zombies[arrayIndex].y = (SIZEY - 2);
		break;
	}
}//end of resetZombiePosition
//---------------------------------------------------------------------------
//----- process key
//---------------------------------------------------------------------------
void setKeyDirection(int key, int& dx, int& dy)
{ //
	switch (key)    //...depending on the selected key...
	{
	case LEFT:			//when LEFT arrow pressed...
		dx = -1;		//decrease the X coordinate
		dy = 0;
		break;
	case RIGHT:			//when RIGHT arrow pressed...
		dx = +1;		//increase the X coordinate
		dy = 0;
		break;
	case UP:			//when UP arrow pressed...
		dx = 0;
		dy = -1;		//decrease the Y coordinate
		break;
	case DOWN:			//when DOWN arrow pressed...
		dx = 0;
		dy = +1;		//increase the Y coordinate
		break;
	}
} //end of setKeyDirection

int getKeyPress()
{ //get key or command selected by user
	int keyPressed;
	keyPressed = getch();			//read in the selected arrow key or command letter
	while (keyPressed == 224)		//ignore symbol following cursor key
		keyPressed = getch();
	return(keyPressed);
} //end of getKeyPress

bool isArrowKey(int key)
{ //check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return ((key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN));
} //end of isArrowKey

bool wantToQuit(int key)
{ //check if the key pressed is 'Q'
	return (toupper(key) == QUIT);
} //end of wantToQuit

bool outOfLives(int lives)
{ //check if spot has lives
	if (lives > 0)
		return false;
	else
		return true;
} //end of outOfLives

bool outOfPills(int pillsRemaining)
{
	if (pillsRemaining == 0)
		return true;
	else
		return false;
}

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------
void clearMessage()
{ //clear message area on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(40, 9);
	string str(20, ' ');
	cout << str;								//display blank message

} //end of setMessage

void renderGame(const char gd[][SIZEX], string mess, int lives)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX]);
	void showTitle();
	void showOptions();
	void showMessage(string, int lives);

	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd);
	//display game title
	showTitle();
	//display menu options available
	showOptions();
	//display message if any
	showMessage(mess, lives);
} //end of paintGame

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)		//for each row (vertically)
	{
		for (int col(0); col < SIZEX; ++col)	//for each column (horizontally)
		{
			cout << g[row][col];				//output cell content
		} //end of col-loop
		cout << endl;
	} //end of row-loop
} //end of paintGrid

void showTitle()
{ //display game title
	SelectTextColour(clYellow);
	Gotoxy(0, 0);
	cout << "___ZOMBIES GAME SKELETON___\n" << endl;
	SelectBackColour(clWhite);
	SelectTextColour(clRed);
	Gotoxy(40, 0);
	cout << "Ashley Swanson, Masimba Walker, Kris Taylor - 1Z : March 25th";
} //end of showTitle


void showOptions()
{ //show game options

	SelectBackColour(clRed);
	SelectTextColour(clYellow);
	Gotoxy(40, 5);
	cout << "TO MOVE USE KEYBOARD ARROWS  ";
	Gotoxy(40, 6);
	cout << "TO QUIT ENTER 'Q'   ";

} //end of showOptions

void showMessage(string m, int lives)
{ //print auxiliary messages if any
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(40, 9);
	cout << m;	//display current message
	Gotoxy(40, 11);
	cout << "You have " << lives << " lives left";
} //end of showMessage

void endProgram(int lives, int key, int pillsRemaining)
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 8);
	if (outOfLives(lives))
		cout << "YOU LOST!              ";
	if (wantToQuit(key))
		cout << "PLAYER QUITS!          ";
	if (outOfPills(pillsRemaining))
		cout << "YOU WIN WITH " << lives << " LIVES REMAINING";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 9);
	system("pause");
} //end of endProgram

void removePill(vector<ChangingItem>& pills, Item sp, string& message, int& pillsRemaining)
{
	for (int i = 0; i < pills.size(); i++)					//for every pill
	if (pills[i].x == sp.x && pills[i].y == sp.y)			//check if the pills coordinates equal spots coordinates
	{
		pills[i].isBeingRendered = false;					//if they do, no longer draw that pill
		--pillsRemaining;
	}
}
