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
	const char symbol;						//symbol on grid
	int x, y;								//coordinates
};
struct Zombie
{
	const char symbol;						//symbol on grid
	int x, y;								//coordinates
};
//---------------------------------------------------------------------------
//----- run game
//---------------------------------------------------------------------------

int main()
{
	//function declarations (prototypes)
	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<Zombie>& zombies);
	bool wantToQuit(int k);
	bool isArrowKey(int k);
	bool outOfLives(int lives);
	int  getKeyPress();
	void updateGame(char g[][SIZEX], Item& sp, vector<Item> holes,vector<Zombie> zombies, int k, int& lives, string& mess);
	void renderGame(const char g[][SIZEX], string mess, int lives);
	void endProgram(int lives);

	//local variable declarations 
	char grid[SIZEY][SIZEX];								//grid for display
	int lives(3);											//The number of lives spot has
	Item spot = { SPOT };									//Spot's symbol and position (0, 0) 
	Item hole = { HOLE };									//Hole's symbol and position (0, 0)
	Zombie zombie = { ZOMBIE };								//Zombies symbol and position (0, 0)
	vector<Item> holes(12, hole);							//Creates a vector of holes, with each element being initialised as hole 
	vector<Zombie> zombies(4, zombie);				     	//Initialise a vector of zombies, each element will be initialised as zombie
	string message("LET'S START...      ");					//current message to player

	bool running = true;

	//action...
	initialiseGame(grid, spot, holes, zombies);						//initialise grid (incl. walls and spot and zombies)
	int key(' ');											//create key to store keyboard events
	do {
		renderGame(grid, message, lives);					//render game state on screen
		message = "                    ";					//reset message
		key = getKeyPress();								//read in next keyboard event
		if (isArrowKey(key))
			updateGame(grid, spot, holes, zombies, key, lives, message);
		else
			message = "INVALID KEY!        ";				//set 'Invalid key' message

		if (wantToQuit(key))								//if player wants to quit
			running = false;
		if (outOfLives(lives))								//if player is out of lives
			running = false;
	} while (running);

	endProgram(lives);										//display final message
	return 0;
} //end main

void updateGame(char grid[][SIZEX], Item& spot, vector<Item> holes,vector<Zombie> zombies,int key, int& lives, string& message)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, int& lives, string& mess);
	void updateZombieCoordinates(const char g[][SIZEX], vector<Zombie>& zombies, Item spot, int& lives, string& message);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> holes, vector<Zombie> zombies);

	updateSpotCoordinates(grid, spot, key, lives, message);	//update according to key
	updateZombieCoordinates(grid, zombies,spot, lives, message);
	updateGrid(grid, spot, holes, zombies);							//update grid information
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes, vector<Zombie>& zombies)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);
	void setHoleInitialCoordinates(vector<Item>& holes, Item& spot);
	void setZombieInitialCoordinates(vector<Zombie>& zombies);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);
	void placeZombies(char gr[][SIZEX], vector<Zombie> zombies);

	Seed();													//seed random number generator

	//do while?
	setGrid(grid);											//reset empty grid
	setSpotInitialCoordinates(spot);						//initialise spot position
	placeSpot(grid, spot);									//set spot in grid
	setHoleInitialCoordinates(holes, spot);					//intiialise holes position
	placeHoles(grid, holes);								//set holes in grid
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
void setZombieInitialCoordinates(vector<Zombie>& zombies)
{
	//Will set up all 4 zombies to spawn in a corner

	for (int i = 0; i < zombies.size(); ++i)
	{
		switch (i)
		{
		case 0:
			zombies.at(i).x = 1;
			zombies.at(i).y = 1;
			break; 
		case 1: 
			zombies.at(i).x = (SIZEX - 2);
			zombies.at(i).y = 1;
			break;
		case 2: 
			zombies.at(i).x = 1; 
			zombies.at(i).y = (SIZEY - 2);
			break; 
		case 3: 
			zombies.at(i).x = (SIZEX - 2);
			zombies.at(i).y = (SIZEY - 2);
			break;
		}
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
void placeZombies(char gr[][SIZEX], vector<Zombie> zombies)
{
	for (int i = 0; i < zombies.size(); ++i)
	{
		gr[zombies[i].y][zombies[i].x] = zombies[i].symbol; //Places a zombie symbol at the x & y of each zombie index in the vector
	}
}//end of placeZombies
//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], Item spot, vector<Item> holes, vector<Zombie> zombies)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char g[][SIZEX], vector<Item> holes);
	void placeZombies(char g[][SIZEX], vector<Zombie> zombies);

	setGrid(grid);							//reset empty grid
	placeHoles(grid, holes);				//set holes in grid
	placeSpot(grid, spot);					//set spot in grid
	placeZombies(grid, zombies);			//set zombies in the grid
} //end of updateGrid

//---------------------------------------------------------------------------
//----- move the spot
//---------------------------------------------------------------------------
void updateSpotCoordinates(const char g[][SIZEX], Item& sp, int key, int& lives, string& mess)
{ //move spot in required direction
	void setKeyDirection(int k, int& dx, int& dy);

	//calculate direction of movement required by key - if any
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);			//find direction indicated by key
	//check new target position in grid 
	//and update spot coordinates if move is possible
	const int targetY(sp.y + dy);
	const int targetX(sp.x + dx);
	switch (g[targetY][targetX])
	{//...depending on what's on the target position in grid...
	case HOLE:								//can move
		--lives;
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
void updateZombieCoordinates(const char g[][SIZEX], vector<Zombie>& zombies, Item spot, int& lives, string& message)
{
	int isLocationFree(int x, int y);
	int dx(0), dy(0); //The maximum amount the player can move by
	int displaceX(0), displaceY(0); //Distance in a vector form from the zombie
	
 	for (int i = 0; i < zombies.size(); ++i)
	{
		displaceX = spot.x - zombies.at(i).x; 
		displaceY = spot.y - zombies.at(i).y;
		if (displaceX < 0)
			dx = -1;
		else
			dx = 1;
		if (displaceY < 0)
			dy = -1;
		else
			dy = + 1;
		
	}
}//end of updateZombieCoordinates
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
	return (key == QUIT);
} //end of wantToQuit

bool outOfLives(int lives)
{ //check if spot has lives
	if (lives > 0)
		return false;
	else
		return true;
} //end of outOfLives

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
	cout << "Ashley Swanson, Masimba Walker, Kris Taylor - 1Z : March 15";
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

void endProgram(int lives)
{ //end program with appropriate message
	SelectBackColour(clBlack);
	SelectTextColour(clYellow);
	Gotoxy(40, 8);
	if (outOfLives(lives))
		cout << "YOU LOST!              ";
	else
		cout << "PLAYER QUITS!          ";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 9);
	system("pause");
} //end of endProgram
