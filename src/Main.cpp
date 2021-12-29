// This code is a modified version of the original by
// Author: Patricia Terol
// Course: CSE 2050
// Project: assign10

// Modified By:
// Modifier: Sameer Sikka
// Project: VaxMan

#include <stdlib.h>
#include <vector>
#include <deque>
#include <windows.h>
#include <glut.h>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

static const int ORIGINAL_INFECTION = 4; //Original number of infections
static const int MAX_INFECTION = 32 * ORIGINAL_INFECTION; //Max number of infections on the screen at one point of time
static const int MULTIPLY_TIME = 30; //Time it takes for one infection to multiply
static const int MAX_POINTS = 106; //Max points attainable
static const float SQUARE_SIZE = 50.0; //Size of one square on the game

static bool replay = false; //check if starts a new game
static bool over = true; //check for the game to be over

class VaxMan {

	float positionX, positionY; // Co-ordinates of VaxMan
	float xIncrement, yIncrement; // Movement of VaxMan
	int rotation; // Orientation of VaxMan

public:

	//Resets vaxman to original configuration
	void New() {
		positionX = 1.5;
		positionY = 1.5;
		xIncrement = 0;
		yIncrement = 0;
		rotation = 0;
	}

	//Method to draw the vaxman character through consecutive circle algorithm
	void DrawVaxMan() {

		int x, y;
		positionX += xIncrement;
		positionY += yIncrement;
		glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 0.0);

		for (int k = 0; k < 32; k++) {
			x = (float)k / 2.0 * cos((30 + 90 * rotation) * M_PI / 180.0) + (positionX * SQUARE_SIZE);
			y = (float)k / 2.0 * sin((30 + 90 * rotation) * M_PI / 180.0) + (positionY * SQUARE_SIZE);

			for (int i = 30; i < 330; i++) {
				glVertex2f(x, y);
				x = (float)k / 2.0 * cos((i + 90 * rotation) * M_PI / 180.0) + (positionX * SQUARE_SIZE);
				y = (float)k / 2.0 * sin((i + 90 * rotation) * M_PI / 180.0) + (positionY * SQUARE_SIZE);
				glVertex2f(x, y);
			}
		}
		glEnd();
	}

	//Returns positionX
	const float getX() { return positionX; }

	//Returns positionY
	const float getY() { return positionY; }

	VaxMan() { New(); }
}vaxman;

class Infection {

	static int numberOfInfections = 0;

	int secondsAlive = 0;
	float location[3];
	float rgb[3];

public:

	//Creates an infection at specified co-ordinates and direction
	Infection(const float& locationX, const float& locationY, const float& direction)
		: location[0](locationX), location[1](locationY), location[2](direction) {
		for (int i = 0; i < 3; i++)
			rgb[i] = (rand() % 100)/100.0;
		numberOfInfections++;
	}

	//Checks if infection is a Pandemic, i.e. if the game is over
	static bool IsPandemic() const { return (numberOfInfections == MAX_INFECTION); }

	//Checks if infection can multiply at current time
	bool IsMultiply() {
		if (secondsAlive == MULTIPLY_TIME) {
			secondsAlive = 0;
			return true;
		}
		return false;
	}

	//Method to update the position of the infection randomly
	void UpdateInfection() {

		//find the current position of the infection
		int x1Quadrant = (int)((location[0] - (2 / SQUARE_SIZE)) - (16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		int x2Quadrant = (int)((location[0] + (2 / SQUARE_SIZE)) + (16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		int y1Quadrant = (int)((location[1] - (2 / SQUARE_SIZE)) - (16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		int y2Quadrant = (int)((location[1] + (2 / SQUARE_SIZE)) + (16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);

		//move infection acording to its direction until it hits an obstacle
		switch ((int)location[2]) {
		case 1:
			if (!bitmap.at(x1Quadrant).at((int)location[1])) {
				location[0] -= 2 / SQUARE_SIZE;
			}
			else {
				int current = location[2];
				do {
					location[2] = (rand() % 4) + 1;
				} while (current == (int)location[2]);
			}
			break;

		case 2:
			if (!bitmap.at(x2Quadrant).at((int)location[1])) {
				location[0] += 2 / SQUARE_SIZE;
			}
			else {
				int current = location[2];
				do {
					location[2] = (rand() % 4) + 1;
				} while (current == (int)location[2]);
			}
			break;

		case 3:
			if (!bitmap.at((int)location[0]).at(y1Quadrant)) {
				location[1] -= 2 / SQUARE_SIZE;
			}
			else {
				int current = location[2];
				do {
					location[2] = (rand() % 4) + 1;
				} while (current == (int)location[2]);
			}
			break;

		case 4:
			if (!bitmap.at((int)location[0]).at(y2Quadrant)) {
				location[1] += 2 / SQUARE_SIZE;
			}
			else {
				int current = location[2];
				do {
					location[2] = (rand() % 4) + 1;
				} while (current == (int)location[2]);
			}
			break;

		default:
			break;
		}
	}

	//Method to draw an infection character through consecutive circles algorithm
	void DrawInfection() {

		int x, y;
		glBegin(GL_LINES);
		glColor3f(rgb[0], rgb[1], rgb[2]);

		//draw the head
		for (int k = 0; k < 32; k++) {

			x = (float)k / 2.0 * cos(360 * M_PI / 180.0) + (location[0] * SQUARE_SIZE);
			y = (float)k / 2.0 * sin(360 * M_PI / 180.0) + (location[1] * SQUARE_SIZE);

			for (int i = 180; i <= 360; i++) {
				glVertex2f(x, y);
				x = (float)k / 2.0 * cos(i * M_PI / 180.0) + (location[0] * SQUARE_SIZE);
				y = (float)k / 2.0 * sin(i * M_PI / 180.0) + (location[1] * SQUARE_SIZE);
				glVertex2f(x, y);
			}
		}
		glEnd();

		//draw body
		glRectf((location[0] * SQUARE_SIZE) - 17, location[1] * SQUARE_SIZE, 
			(location[0] * SQUARE_SIZE) + 15, (location[1] * SQUARE_SIZE) + 15);
		glBegin(GL_POINTS);
		glColor3f(0, 0.2, 0.4);

		//draw eyes and legs
		glVertex2f((location[0] * SQUARE_SIZE) - 11, (location[1] * SQUARE_SIZE) + 14); //legs
		glVertex2f((location[0] * SQUARE_SIZE) - 1, (location[1] * SQUARE_SIZE) + 14); //legs
		glVertex2f((location[0] * SQUARE_SIZE) + 8, (location[1] * SQUARE_SIZE) + 14); //legs
		glVertex2f((location[0] * SQUARE_SIZE) + 4, (location[1] * SQUARE_SIZE) - 3); //eyes
		glVertex2f((location[0] * SQUARE_SIZE) - 7, (location[1] * SQUARE_SIZE) - 3); //eyes 
		glEnd();
	}
};

static auto infectionArray = new vector<Infection, MAX_INFECTION>(); //array for storing all infections present on screen

static vector<int> border = { 0, 0, 15, 1, 15, 15, 14, 1, 0, 14, 15, 15, 1, 14, 0, 0 }; //coordinates of the border walls

//coordinates of the obstacles (divided into 3 for clarity)
static vector<int> obstaclesTop = { 2, 2, 3, 6, 3, 6, 4, 5, 4, 2, 5, 4, 5, 3, 6, 5, 6, 1, 9, 2, 7, 2, 8, 5, 9, 5, 10, 3, 10, 4, 11, 2, 11, 5, 12, 6, 12, 6, 13, 2 };
static vector<int> obstaclesMiddle = { 2, 9, 3, 7, 3, 7, 4, 8, 4, 9, 5, 11, 5, 6, 6, 10, 6, 10, 7, 8, 7, 8, 8, 9, 6, 7, 7, 6, 8, 6, 9, 7, 10, 6, 9, 10, 9, 10, 8, 8, 11, 9, 10, 11, 11, 8, 12, 7, 12, 7, 13, 9 };
static vector<int> obstaclesBottom = { 2, 10, 3, 13, 3, 13, 4, 12, 5, 12, 6, 13, 6, 13, 7, 11, 8, 11, 9, 13, 9, 13, 10, 12, 11, 12, 12, 13, 12, 13, 13, 10 };
static deque<float> food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };

static vector<vector<bool>> bitmap; // 2d image of which squares are blocked and which are clear for vaxman to move in 
bool* keyStates = new bool[256]; // record of all keys pressed 
int points = 0; // total points collected

//Initializes the game with the appropiate information 
void Init(void) {

	//clear screen
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	//reset all keys
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
	}

	//start infection
	StartInfection();

	//fill the bitmap with the obstacles
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, true, true, true, true, false, false, false, true, false, false, false, false, false, true });
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
}

//Method to draw the obstacle course and the walls
void DrawLabyrinth() {

	glColor3f(1.0, 1.0, 1.0);

	//Border
	for (int i = 0; i < border.size(); i = i + 4) {
		glRectf(border.at(i) * SQUARE_SIZE, border.at(i + 1) * SQUARE_SIZE, 
			border.at(i + 2) * SQUARE_SIZE, border.at(i + 3) * SQUARE_SIZE);
	}

	//Obstacles
	for (int j = 0; j < obstaclesBottom.size(); j = j + 4) {
		glRectf(obstaclesBottom.at(j) * SQUARE_SIZE, obstaclesBottom.at(j + 1) * SQUARE_SIZE, obstaclesBottom.at(j + 2) * SQUARE_SIZE, obstaclesBottom.at(j + 3) * SQUARE_SIZE);
	}
	for (int k = 0; k < obstaclesMiddle.size(); k = k + 4) {
		glRectf(obstaclesMiddle.at(k) * SQUARE_SIZE, obstaclesMiddle.at(k + 1) * SQUARE_SIZE, obstaclesMiddle.at(k + 2) * SQUARE_SIZE, obstaclesMiddle.at(k + 3) * SQUARE_SIZE);
	}
	for (int p = 0; p < obstaclesTop.size(); p = p + 4) {
		glRectf(obstaclesTop.at(p) * SQUARE_SIZE, obstaclesTop.at(p + 1) * SQUARE_SIZE, obstaclesTop.at(p + 2) * SQUARE_SIZE, obstaclesTop.at(p + 3) * SQUARE_SIZE);
	}
}

//Method to check if the food has been eaten
bool IsFoodEaten(const int& x, const int& y) {

	if (x >= vaxman.getX() - 16.0 * cos(359 * M_PI / 180.0) && x <= vaxman.getX() + 16.0 * cos(359 * M_PI / 180.0)) {
		if (y >= vaxman.getY() - 16.0 * cos(359 * M_PI / 180.0) && y <= vaxman.getY() + 16.0 * cos(359 * M_PI / 180.0)) {
			return true;
		}
	}
	return false;
}

//Method to draw all the food left and delete the ate one
void DrawFood() {

	deque<float> temp;

	//check if the food has not been eaten
	for (int i = 0; i < food.size(); i = i + 2) {
		if (!IsFoodEaten(food.at(i) * SQUARE_SIZE, food.at(i + 1) * SQUARE_SIZE, vaxman.getX(), vaxman.getY())) {
			temp.push_back(food.at(i));
			temp.push_back(food.at(i + 1));
		}
		else {
			points++;
		}
	}

	food.swap(temp);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 1.0);

	//draw all the food avilable
	for (int j = 0; j < food.size(); j = j + 2) {
		glVertex2f(food.at(j) * SQUARE_SIZE, food.at(j + 1) * SQUARE_SIZE);
	}

	glEnd();
}

//Destroys infection in contact with VaxMan
void DestroyInfection(const int& deleteIndex) {

	infectionArray->at(deleteIndex) = infectionArray->back();
	infectionArray->pop_back();
}

//Method to set the pressed key
void KeyPressed(const unsigned char& key, int x, int y) { keyStates[key] = true; }

//Method to unset the released key
void KeyUp(const unsigned char& key, int x, int y) { keyStates[key] = false; }

//Method to start the infection
void StartInfection() {

	//Empties infectionArray
	infectionArray->clear();

	// Creates ORIGINAL_INFECTION infections
	infectionArray->push_back({ 10.5, 8.5, 1.0 }); //coordinates and direction of first infection
	infectionArray->push_back({ 13.5, 1.5, 2.0 }); //coordinates and direction of second infection
	infectionArray->push_back({ 4.5, 6.5, 3.0 }); //coordinates and direction of third infection
	infectionArray->push_back({ 2.5, 13.5, 4.0 }); //coordinates and direction of fourth infection
}

//Method to reset all the variable necessaries to start the game again
void ResetGame() {

	over = false;
	xIncrement = 0;
	yIncrement = 0;
	rotation = 0;
	StartInfection();

	points = 0;
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
	}

	food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 5.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
}

//Method to update the movement of the vaxman according to the movement keys pressed
void KeyOperations() {

	//get current position
	float  x = (1.5 + xIncrement) * SQUARE_SIZE;
	float y = (1.5 + yIncrement) * SQUARE_SIZE;

	//update according to keys pressed
	if (keyStates['a']) {
		x -= 2;
		int x1Quadrant = (int)((x - 16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		if (!bitmap.at(x1Quadrant).at((int)y / SQUARE_SIZE)) {
			xIncrement -= 2 / SQUARE_SIZE;
			rotation = 2;
		}
	}
	if (keyStates['d']) {
		x += 2;
		int x2Quadrant = (int)((x + 16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		if (!bitmap.at(x2Quadrant).at((int)y / SQUARE_SIZE)) {
			xIncrement += 2 / SQUARE_SIZE;
			rotation = 0;
		}
	}
	if (keyStates['w']) {
		y -= 2;
		int y1Quadrant = (int)((y - 16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		if (!bitmap.at((int)x / SQUARE_SIZE).at(y1Quadrant)) {
			yIncrement -= 2 / SQUARE_SIZE;
			rotation = 3;
		}
	}
	if (keyStates['s']) {
		y += 2;
		int y2Quadrant = (int)((y + 16.0 * cos(360 * M_PI / 180.0)) / SQUARE_SIZE);
		if (!bitmap.at((int)x / SQUARE_SIZE).at(y2Quadrant)) {
			yIncrement += 2 / SQUARE_SIZE;
			rotation = 1;
		}
	}
	if (keyStates[' ']) {
		if (!replay && over) {
			resetGame();
			replay = true;
		}
		else if (replay && over) {
			replay = false;
		}
	}
}

//Method to check if the game is over
void IsGameOver() {

	int vaxman.getX() = (int)(1.5 + xIncrement);
	int vaxman.getY() = (int)(1.5 + yIncrement);
	int monster1X = (int)(monster1[0]);
	int monster1Y = (int)(monster1[1]);
	int monster2X = (int)(monster2[0]);
	int monster2Y = (int)(monster2[1]);
	int monster3X = (int)(monster3[0]);
	int monster3Y = (int)(monster3[1]);
	int monster4X = (int)(monster4[0]);
	int monster4Y = (int)(monster4[1]);

	if (vaxman.getX() == monster1X && vaxman.getY() == monster1Y) {
		over = true;
	}
	if (vaxman.getX() == monster2X && vaxman.getY() == monster2Y) {
		over = true;
	}
	if (vaxman.getX() == monster3X && vaxman.getY() == monster3Y) {
		over = true;
	}
	if (vaxman.getX() == monster4X && vaxman.getY() == monster4Y) {
		over = true;
	}
	if (points == MAX_POINTS) {
		over = true;
	}
}

//Method to display win
void WinScreen() {

	const char* message = "*************************************";
	glRasterPos2f(170, 250);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "CONGRATULATIONS, YOU WON! ";

	glColor3f(1, 1, 1);
	glRasterPos2f(200, 300);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "*************************************";

	glRasterPos2f(170, 350);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "To start or restart the game, press the space key.";

	glRasterPos2f(170, 550);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
}

//Method to display loss
void LossScreen() {

	const char* message = "*************************";
	glRasterPos2f(210, 250);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "SORRY, YOU LOST ... ";

	glColor3f(1, 1, 1);
	glRasterPos2f(250, 300);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "*************************";

	glRasterPos2f(210, 350);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "You got: ";

	glRasterPos2f(260, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	string result = to_string(points);
	message = (char*)result.c_str();

	glRasterPos2f(350, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = " points!";

	glRasterPos2f(385, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "To start or restart the game, press the space key.";

	glRasterPos2f(170, 550);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
}

//Method to display the results of the game at the ends
void ResultsDisplay() {

	if (points == MAX_POINTS)
		win();
	else
		loss();
}

//Method to display the starting instructions
void WelcomeScreen() {

	glClearColor(0, 0.2, 0.4, 1.0);
	const char* message = "*************************************";
	glRasterPos2f(150, 200);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "VAXMAN - by Sameer Sikka";

	glColor3f(1, 1, 1);
	glRasterPos2f(225, 250);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "*************************************";

	glRasterPos2f(150, 300);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "To control Pacman use A to go right, D to go left, W to go up and S to go down.";

	glRasterPos2f(50, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	message = "To start or restart the game, press the space key.";

	glRasterPos2f(170, 450);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
}

//Method to display the screen and its elements
void Display() {

	if (points == 1) {
		over = false;
	}
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);
	IsGameOver();

	if (replay) {
		if (!over) {
			DrawLabyrinth();
			DrawFood((1.5 + xIncrement) * SQUARE_SIZE, (1.5 + yIncrement) * SQUARE_SIZE);
			vaxman.DrawVaxMan();

			for (int i = 0; i < infectionArray.size(); i++) {
				infectionArray->at(i).UpdateInfection();
				infectionArray->at(i).DrawInfection();
			}
		}
		else {
			ResultsDisplay();
		}
	}
	else {
		WelcomeScreen();
	}
	glutSwapBuffers();
}

//Methdo to reshape the game is the screen size changes
void Reshape(int w, int h) {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, 750, 750, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Main functions that controls the running of the game
int main(int argc, char** argv) {

	//initialize and create the screen
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 50);
	glutCreateWindow("VAXMAN - by Sameer Sikka");

	//define all the control functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);
	glutKeyboardFunc(KeyPressed);
	glutKeyboardUpFunc(KeyUp);

	//run the game
	Init();
	glutMainLoop();
	return 0;
}