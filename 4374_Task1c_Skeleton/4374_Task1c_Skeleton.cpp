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
#include "TimeUtils.h"						//for GetTime, GetDate

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
//defining the command letters to move spot on the maze
const int  UP(72);							//up arrow
const int  DOWN(80);						//down arrow
const int  RIGHT(77);						//right arrow
const int  LEFT(75);						//left arrow
//defining the other command letters
const char PLAY('P');						//play the game
const char QUIT('Q');						//end the game
const char INFO('I');						//show information
const char BACK('\r');						//go back

//data structure to store data for a grid item
struct Item
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
	int  getKeyPress();
	bool wantToQuit(int k);
	void playGame();
	void displayMenu();
	void displayInfo();
	void displayExit();
	//local variable declarations 
	bool running = true;									//to exit the game
	bool back = false;										//to get back to the menu
	int key(' ');											//create key to store keyboard events

	//menu
	do {
		displayMenu();										//display the main menu
		key = getKeyPress();								//read in next keyboard event
		//game
		if (toupper(key) == PLAY)
			playGame();										//run the game
		//quit
		if (wantToQuit(key))
		{
			displayExit();									//display the exit message
			system("pause");
			running = false;								//quit the game
		}
		//info
		if (toupper(key) == INFO)
		{
			do {
				displayInfo();								//display the info screen
				key = getKeyPress();						//read in next keyboard event
				if (key == BACK)
					back = true;
			} while (!back);
		}
	} while (running);

	return 0;
} //end main

void playGame()
{
	//function declarations (prototypes)
	int  getKeyPress();
	bool wantToQuit(int k);
	bool isArrowKey(int k);
	bool outOfLives(int lives);
	void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes);
	void updateGame(char g[][SIZEX], Item& sp, vector<Item> holes, int k, int& lives, string& mess);
	void renderGame(const char g[][SIZEX], string mess, int lives);
	void endProgram(int lives);

	//local variable declarations 
	int lives(3);											//The number of lives spot has
	int key(' ');											//create key to store keyboard events
	char grid[SIZEY][SIZEX];								//grid for display
	Item spot = { SPOT };									//Spot's symbol and position (0, 0) 
	Item hole = { HOLE };									//Hole's symbol and position (0, 0)
	vector <Item> holes(12, hole);							//Creates a vector of holes, with each element being initialised as hole 
	string message("         LET'S START...          ");	//current message to player

	bool playing = true;
	initialiseGame(grid, spot, holes);						//initialise grid (incl. walls and spot)
	while (playing)
	{
		renderGame(grid, message, lives);					//render game state on screen
		message = "                                 ";		//reset message
		key = getKeyPress();								//read in next keyboard event
		if (isArrowKey(key))
			updateGame(grid, spot, holes, key, lives, message);
		else
			message = "          INVALID KEY!           ";	//set 'Invalid key' message
		if (wantToQuit(key))								//if player wants to quit
			playing = false;
		if (outOfLives(lives))								//if player is out of lives
			playing = false;
	}
	endProgram(lives);										//display final message
}

void updateGame(char grid[][SIZEX], Item& spot, vector<Item> holes, int key, int& lives, string& message)
{ //updateGame state
	void updateSpotCoordinates(const char g[][SIZEX], Item& spot, int key, int& lives, string& mess);
	void updateGrid(char g[][SIZEX], Item spot, vector<Item> holes);

	updateSpotCoordinates(grid, spot, key, lives, message);	//update according to key
	updateGrid(grid, spot, holes);							//update grid information
}

//---------------------------------------------------------------------------
//----- initialise game state
//---------------------------------------------------------------------------
void initialiseGame(char grid[][SIZEX], Item& spot, vector<Item>& holes)
{ //initialise grid and place spot in middle
	void setGrid(char[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot);
	void setHoleInitialCoordinates(vector<Item>& holes, Item& spot);
	void placeSpot(char gr[][SIZEX], Item spot);
	void placeHoles(char gr[][SIZEX], vector<Item> holes);

	Seed();													//seed random number generator

	//do while?
	setGrid(grid);											//reset empty grid
	setSpotInitialCoordinates(spot);						//initialise spot position
	placeSpot(grid, spot);									//set spot in grid
	setHoleInitialCoordinates(holes, spot);					//intiialise holes position
	placeHoles(grid, holes);								//set holes in grid

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

//---------------------------------------------------------------------------
//----- update grid state
//---------------------------------------------------------------------------
void updateGrid(char grid[][SIZEX], Item spot, vector<Item> holes)
{ //update grid configuration after each move
	void setGrid(char[][SIZEX]);
	void placeSpot(char g[][SIZEX], Item spot);
	void placeHoles(char g[][SIZEX], vector<Item> holes);

	setGrid(grid);							//reset empty grid
	placeHoles(grid, holes);				//set holes in grid
	placeSpot(grid, spot);					//set spot in grid
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
		mess = "       CANNOT GO THERE...        ";
		break;
	}
} //end of updateSpotCoordinates

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

//---------------------------------------------------------------------------
//----- display info on screen
//---------------------------------------------------------------------------
void displayMenu()
{
	void showMenuTitle();
	void showDateAndTime();
	void showCredits();
	void showInfo();

	SelectBackColour(clBlack);
	Clrscr();

	showMenuTitle();
	showDateAndTime();
	showCredits();
	showInfo();
}

void displayInfo()
{
	void showMenuTitle();
	void showDateAndTime();
	void showHelp();
	void showOptions();

	showMenuTitle();
	showDateAndTime();
	showHelp();
	showOptions();
}

void renderGame(const char gd[][SIZEX], string mess, int lives)
{ //display game title, messages, maze, spot and apples on screen
	void paintGrid(const char g[][SIZEX]);
	void showGameTitle();
	void showGameOptions();
	void showDateAndTime();
	void showMessage(string, int lives);

	SelectBackColour(clBlack);
	Clrscr();
	Gotoxy(0, 0);
	//display grid contents
	paintGrid(gd);
	//display game title
	showGameTitle();
	showDateAndTime();
	//display game options available
	showGameOptions();
	//display message if any
	showMessage(mess, lives);
} //end of paintGame

void paintGrid(const char g[][SIZEX])
{ //display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(10, 4);
	for (int row(0); row < SIZEY; ++row)		//for each row (vertically)
	{
		for (int col(0); col < SIZEX; ++col)	//for each column (horizontally)
		{
			cout << g[row][col];				//output cell content
		} //end of col-loop
		cout << endl << "          ";
	} //end of row-loop
} //end of paintGrid

void showMessage(string m, int lives)
{ //print auxiliary messages if any
	SelectBackColour(clDarkCyan);
	SelectTextColour(clGreen);
	Gotoxy(40, 13);
	cout << "      You have " << lives << " lives left      ";
	Gotoxy(40, 14);
	cout << m;	//display current message
} //end of showMessage

void clearMessage()
{ //clear message area on screen
	SelectBackColour(clDarkCyan);
	SelectTextColour(clGreen);
	Gotoxy(40, 14);
	string str(33, ' ');
	cout << str;								//display blank message
} //end of setMessage

void showMenuTitle()
{
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);
	Gotoxy(10, 2);
	cout << "                    ";
	Gotoxy(10, 3);
	cout << "  ----------------  ";
	Gotoxy(10, 4);
	cout << "  LEFT FOR DEAD: 3  ";
	Gotoxy(10, 5);
	cout << "  ----------------  ";
	Gotoxy(10, 6);
	cout << "                    ";
}

void showGameTitle()
{ //display game title
	SelectBackColour(clWhite);
	SelectTextColour(clBlack);
	Gotoxy(9, 2);
	cout << "   LEFT FOR DEAD: 3   ";
} //end of showTitle

void showDateAndTime()
{
	SelectBackColour(clDarkYellow);
	SelectTextColour(clYellow);
	Gotoxy(40, 2);
	cout << "  ASHLEY SWANSON 1st April 2015  ";
	Gotoxy(40, 3);
	cout << "      " << GetDate() << "   " << GetTime() << "      ";
}

void showCredits()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(10, 8);
	cout << "  GROUP 1Z:         ";
	Gotoxy(10, 9);
	cout << "  Masimba Walker    ";
	Gotoxy(10, 10);
	cout << "  Kris Taylor       ";
	Gotoxy(10, 11);
	cout << "  Ashley Swanson    ";
	Gotoxy(10, 13);
	cout << " (We are in no way  ";
	Gotoxy(10, 14);
	cout << "  affiliated with   ";
	Gotoxy(10, 15);
	cout << " Valve Corporation) ";
}

void showInfo()
{
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << " Left for Dead: 3 is a top-down  ";
	Gotoxy(40, 6);
	cout << " strategy game, where you find   ";
	Gotoxy(40, 7);
	cout << " yourself avoiding the horde AND ";
	Gotoxy(40, 8);
	cout << " feeding your addiction to pixels";
	Gotoxy(40, 9);
	cout << "                                 ";
	Gotoxy(40, 10);
	cout << " PRESS P TO START PLAYING        ";
	Gotoxy(40, 11);
	cout << " PRESS I FOR MORE INFORMATION    ";
}

void showOptions()
{ //show options
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << " PRESS ENTER TO GO BACK          ";
	Gotoxy(40, 6);
	cout << " PRESS P TO PLAY, Q TO QUIT      ";
	Gotoxy(40, 7);
	cout << "                                 ";
	Gotoxy(40, 8);
	cout << " MOVE USING THE ARROW KEYS       ";
	Gotoxy(40, 9);
	cout << " PRESS F TO FREEZE THE ZOMBIES   ";
	Gotoxy(40, 10);
	cout << " PRESS X TO KILL ALL ZOMBIES     ";
	Gotoxy(40, 11);
	cout << " PRESS E TO EAT ALL THE PILLS    ";
} //end of showOptions

void showGameOptions()
{ //show game options
	SelectBackColour(clDarkMagenta);
	SelectTextColour(clGrey);
	Gotoxy(40, 5);
	cout << "  Collect Pills - Avoid Zombies  ";
	Gotoxy(40, 6);
	cout << "                                 ";
	Gotoxy(40, 7);
	cout << " MOVE USING THE ARROW KEYS       ";
	Gotoxy(40, 8);
	cout << " PRESS F TO FREEZE THE ZOMBIES   ";
	Gotoxy(40, 9);
	cout << " PRESS X TO KILL ALL ZOMBIES     ";
	Gotoxy(40, 10);
	cout << " PRESS E TO EAT ALL THE PILLS    ";
	Gotoxy(40, 11);
	cout << " PRESS Q TO QUIT THE GAME        ";
} //end of showGameOptions

void showHelp()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(10, 8);
	cout << "  AIM OF THE GAME:  ";
	Gotoxy(10, 9);
	cout << " - Collect Pills    ";
	Gotoxy(10, 10);
	cout << " - Avoid Zombies    ";
	Gotoxy(10, 11);
	cout << " - Avoid Holes      ";
	Gotoxy(10, 13);
	cout << " YOUR SCORE IS MADE ";
	Gotoxy(10, 14);
	cout << " BY CALCULATING DIS ";
	Gotoxy(10, 15);
	cout << " STUFF I DON'T KNOW ";
}

void endProgram(int lives)
{ //end program with appropriate message
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(40, 13);
	if (outOfLives(lives))
		cout << "            YOU LOST!            ";
	else
		cout << "          PLAYER QUITS!          ";
	//hold output screen until a keyboard key is hit
	Gotoxy(40, 14);
	cout << " ";
	system("pause");
} //end of endProgram

void displayExit()
{
	SelectBackColour(clBlue);
	SelectTextColour(clWhite);
	Gotoxy(40, 13);
	cout << "       THANKS FOR PLAYING!       ";
	Gotoxy(40, 14);
	cout << " ";
}