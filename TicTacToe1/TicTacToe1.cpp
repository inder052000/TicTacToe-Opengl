//Libraries
#define GLUT_DISABLE_ATEXIT_HACK
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>
#include<GL/glut.h>
#include<vector>
#include<string>

#include<stdio.h>
#include<stdlib.h>

using namespace std;

int matrix[3][3]; //this matrix stores the x and o and blank box of the game, a value of {0: blank, 1: x, 2: o}
int playerTurn; //playerTurn if it is 1 then 1st players turn else if it is 2 then its second players turn
int result; //result of the game if it is 0 then draw if it is 1 then player 1 wins if it is 2 then player 2 wins
bool game_over; //game_over if it is 0 then its not game over else if it is 1 then its game over

string player1Name = "Player 1";
string player2Name = "Computer";
string computer = "0";

void singleOrMulti();
//Take corresponding input for player name(s)
void takeInput() {
	if (computer == "1") {
		cout << "Enter your name: ";
		getline(cin, player1Name);
	}
	else if(computer == "2"){
		cout << "Enter first player name: ";
		getline(cin, player1Name);
		cout << "Enter second player name: ";
		getline(cin, player2Name);
	}
	else if (computer == "3") {
		exit(0);
	}
	else {
		cout << "Wrong Input entered! Please check and enter again..";
		singleOrMulti();
	}
}

//Function Menu to specify if game is single player or multiplayer
void singleOrMulti() {
	cout << "Choose the Play Mode: ";
	cout << "\n\t\t1. Single Player";
	cout << "\n\t\t2. Multi-Player";
	cout << "\n\t\t3. Exit";
	cout << "\n\tEnter your option: ";
	getline(cin, computer);
	takeInput();
}

//initialize the game
void InitGame(){
	playerTurn = 1; //x starts first
	
	//clear the matrix
	for (int i = 0; i <= 2; i++){
		for (int j = 0; j <= 2; j++){
			matrix[i][j] = 0;
		}
	}
	singleOrMulti();
}


//this function is called when keyboard button is pressed 
void KeyPress(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		if (game_over == true){
			game_over = false;
			InitGame();
		}
		break;
	case 'n':
		if (game_over == true)
			exit(0);
		break;
	case 27:	//If press 'esc', then game ends
		exit(0);
	}
}

//Computer Move Function 
pair<int, int> ComputerMove() {
	vector<pair<int, int>> moves;
	for (int i = 0;i < 3;i++) {
		for (int j = 0;j < 3;j++) {
			if (matrix[i][j] == 0) {
				moves.push_back(make_pair(i, j));
			}
		}
	}
	pair<int, int> finalMove = moves[rand()%moves.size()];
	moves.clear();
	return finalMove;
}

//This function is called when the mouse button is pressed and this function puts the x or o on the blank box 
void onClick(int button, int state, int x, int y){
	if (game_over == false && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){	//When left mouse button is pressed
		if (playerTurn == 1){
			if (matrix[(y - 50) / 100][x / 100] == 0){
				matrix[(y - 50) / 100][x / 100] = 1;
				playerTurn = 2;
			}
		}
		else{
			if (computer == "1") {
				pair<int, int> move = ComputerMove();
				matrix[move.first][move.second] = 2;
				playerTurn = 1;
			}
			else {
				if (matrix[(y - 50) / 100][x / 100] == 0) {
					matrix[(y - 50) / 100][x / 100] = 2;
					playerTurn = 1;
				}
			}
		}
	}
}

//Used to write text in the opengl window
void drawString(void* font, const char* s, float x, float y) {
	unsigned int i;
	glRasterPos2f(x, y);
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]);
}

//This function is used to draw the 4 lines 2 vertical and 2 horizontal
void drawLines(){			//3X3 Cross playing board
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);		//Denotes black color
	//2 vertical lines
	glVertex2f(100, 50);
	glVertex2f(100, 340);
	glVertex2f(200, 340);
	glVertex2f(200, 50);
	//2 horizontal lines
	glVertex2f(0, 150);
	glVertex2f(300, 150);
	glVertex2f(0, 250);
	glVertex2f(300, 250);
	glEnd();
}

//Utility function for bresenhan's algo
void drawCircle(int xc, int yc, int x, int y)
{
	glBegin(GL_POINTS);
	glVertex2i(xc + x, yc + y);
	glVertex2i(xc - x, yc + y);
	glVertex2i(xc + x, yc - y);
	glVertex2i(xc - x, yc - y);
	glVertex2i(xc + y, yc + x);
	glVertex2i(xc - y, yc + x);
	glVertex2i(xc + y, yc - x);
	glVertex2i(xc - y, yc - x);
	glEnd();
}
//Function To draw circle using bresenhan's algo
void circleBres(int xc, int yc, int r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	drawCircle(xc, yc, x, y);
	while (y >= x)
	{
		// for each pixel we will draw all eight pixels
		x++;

		// check for decision parameter and correspondingly update d, x, y
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawCircle(xc, yc, x, y);
	}
}
//This Function Draw the x and o's
void drawXO(){
	for (int i = 0; i <= 2; i++){
		for (int j = 0; j <= 2; j++){
			if (matrix[i][j] == 1){ //if it is 1 then draw x 
				glBegin(GL_LINES);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(50 + j * 100 - 25, 100 + i * 100 - 25);
				glVertex2f(50 + j * 100 + 25, 100 + i * 100 + 25);
				glVertex2f(50 + j * 100 - 25, 100 + i * 100 + 25);
				glVertex2f(50 + j * 100 + 25, 100 + i * 100 - 25);
				glEnd();
			}
			else if (matrix[i][j] == 2){ //if it is 2 then draw o
				int xc = j * 100 + 50;	//Circle Center x-coordinate	
				int yc = i * 100 + 100;	//Circle Center y-coordinate
				circleBres(xc, yc, 25);
			}
		}
	}
}

//This function checks if there is any winner
bool checkIfWin(){
	int i, j;
	//check if there are horizontal win i.e if there is any row that has same value
	for (i = 0; i <= 2; i++){
		for (j = 1; j <= 2; j++){
			if (matrix[i][0] != 0 && matrix[i][0] == matrix[i][j]){
				if (j == 2)
					return true;
			}
			else
				break;
		}
	}

	//check if there are vertical win i.e if there is any column that has same value
	for (i = 0; i <= 2; i++){
		for (j = 1; j <= 2; j++){
			if (matrix[0][i] != 0 && matrix[0][i] == matrix[j][i]){
				if (j == 2)
					return true;
			}
			else
				break;
		}
	}

	//check if there is any diagonal win i.e. if there is any diagonals that has same value
	for (i = 1; i <= 2; i++){
		if (matrix[0][0] != 0 && matrix[0][0] == matrix[i][i]){
			if (i == 2)
				return true;
		}
		else
			break;
	}
	for (i = 1; i <= 2; i++){
		if (matrix[2][0] != 0 && matrix[2][0] == matrix[2 - i][i]){
			if (i == 2)
				return true;
		}
		else
			break;
	}
}

//This function checks if the match is a draw i.e it returns false if there is atleast one empty box else returns true 
bool checkIfDraw()
{
	int i, j;
	bool draw;

	for (i = 0; i <= 2; i++){
		for (j = 0; j <= 2; j++){
			if (matrix[i][j] == 0)
				return false;
		}
	}
	return true;
}

//This is the idle call back function
void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1, 1, 0, 1);
	glColor3f(0, 0, 0);
	string p1 = player1Name + "'s turn";
	string p2 = player2Name + "'s turn";
	const char* p1Turn_c = p1.data();
	const char* p2Turn_c = p2.data();
	if (playerTurn == 1)
		drawString(GLUT_BITMAP_HELVETICA_18, p1Turn_c, 100, 30);
	else
		drawString(GLUT_BITMAP_HELVETICA_18, p2Turn_c, 100, 30);

	drawLines();
	drawXO();

	if (checkIfWin() == true){
		//the player who made the previous move is the winner
		if (playerTurn == 1){
			game_over = true;
			result = 2; //player2 wins
		}
		else{
			game_over = true;
			result = 1; //player1 wins
		}
	}
	else if (checkIfDraw() == true){
		game_over = true;
		result = 0;
	}

	if (game_over == true){
		drawString(GLUT_BITMAP_HELVETICA_18, "Game Over", 100, 160);

		if (result == 0)
			drawString(GLUT_BITMAP_HELVETICA_18, "Its a draw", 110, 185);

		if (result == 1) {
			p1 = player1Name + "Wins!!";
			const char* winner1 = p1.data();
			drawString(GLUT_BITMAP_HELVETICA_18, winner1, 95, 185);
		}

		if (result == 2) {
			p2 = player2Name + "Wins!!";
			const char* winner2 = p2.data();
			drawString(GLUT_BITMAP_HELVETICA_18, winner2, 95, 185);
		}

		drawString(GLUT_BITMAP_HELVETICA_18, "Do you want to continue (Y/N)?", 40, 210);
	}

	glutSwapBuffers();
}

//This function is called on windows resize
void reshape(int x, int y){
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, x, y, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
}

//main function of the program

int main(int argc, char** argv){
	InitGame();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(300, 350);
	glutCreateWindow("Tic Tac Toe Game");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(KeyPress);
	glutMouseFunc(onClick);
	glutIdleFunc(display);
	glutMainLoop();
}