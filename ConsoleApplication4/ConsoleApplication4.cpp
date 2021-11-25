// Based on code by N. Vitanovic


#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace std;

bool		GameLooper = true;
bool		GameOver;
const int	PlayFieldWidth = 20;
const int	PlayFieldHeight = 20;
const int	MaxScore = 3990;
int			SnakeX, SnakeY, Score;
int			TailX[400], TailY[400], FruitX[100], FruitY[100];
int			NumTailSegments;
int			NumFruits;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;

// If Debug is TRUE then debug menu can be accessed by pressing M during gameplay. Should be FALSE in prod.
bool		Debug = TRUE;

// Debug settings configured within the Debug menu
bool		VerboseEnabled = FALSE;
bool		WallCollisionsEnabled = TRUE;
bool		TailCollisionsEnabled = TRUE;


void HideCursor() {
	//Function from cplusplus.com - Hides cursor to reduce flickering
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
} // HideCursor

void FruitPositioner() {
	// Positions the correct number of fruits randomly. Replaces fruits spawned at the origin (11,11)
	srand(time(NULL));
	for (int i = 0; i < NumFruits; i++) {
		FruitX[i] = rand() % PlayFieldWidth;
		FruitY[i] = rand() % PlayFieldHeight;
		if (FruitX[i] == 10 && FruitY[i] == 10) {
			FruitX[i] = rand() % PlayFieldWidth;
			FruitY[i] = rand() % PlayFieldHeight;
		}
	}
} // FruitPositioner

void GameSetup() {
	// Runs before every game instance. Resets tail, score, fruits, and snake position / direction
	GameOver = false;
	dir = STOP;
	SnakeX = PlayFieldWidth / 2;
	SnakeY = PlayFieldHeight / 2;
	FruitPositioner();
	Score = 0;
	NumTailSegments = 0;
} // GameSetup

void FruitPositionDebug() {
	// Displays fruit position information if verbose Mode is enabled via the debug menu
	for (int i = 0; i < NumFruits; i++) {
		cout << "Fruit " << i + 1 << " Position = " << FruitX[i] + 1 << "," << FruitY[i] + 1 << endl;
	}
} // FruitPositionDebug

void SnakePositionDebug() {
	// Displays snake position information if verbose Mode is enabled via the debug menu
	cout << "Snake Head Position:  " << SnakeX + 1 << "," << SnakeY + 1;
} // SnakePositionDebug


void DrawToporBottomWall() {
	for (int i = 0; i < PlayFieldWidth + 2; i++)
		cout << "#";
	cout << endl;
} // DrawToporBottomWall

void DrawGameBoard() {
	system("cls");
	DrawToporBottomWall();
	for (int i = 0; i < PlayFieldHeight; i++) {
		for (int j = 0; j < PlayFieldWidth; j++) {
			// Draws left wall
			if (j == 0)
				cout << "#";
			// Draws snake
			if (i == SnakeY && j == SnakeX)
				cout << "O";
			else {
				bool PrintBlank = false;
				bool PrintedTail = false;
				// Tail drawing
				for (int k = 0; k < NumTailSegments; k++) {
					if (TailX[k] == j && TailY[k] == i) {
						cout << "o";
						PrintBlank = true;
						PrintedTail = true;
						break;
					}
				}
				// Fruit drawing
				for (int l = 0; l < NumFruits; l++) {
					if (FruitX[l] == j && FruitY[l] == i && PrintedTail == false) {
						cout << "F";
						PrintBlank = true;
						break;
					}
				}
				// Draws blank spaces where snake, tail, fruit, and walls are NOT.
				if (!PrintBlank)
					cout << " ";
			}
			// Draws right wall
			if (j == PlayFieldWidth - 1)
				cout << "#";
		}
		cout << endl;
	}
	DrawToporBottomWall();
	cout << "Score:" << Score << endl;
	// If verbose mode is enabled will print that information below the score
	if (VerboseEnabled) {
		FruitPositionDebug();
		SnakePositionDebug();
	}
} // DrawGameBoard

void PauseGame() {
	cout << "Game Paused.\nPress any key to resume.";
	_getch();
} // PauseGame

void VerboseModeSelector() {
	// Enables/Disables verbose mode (Debug Option)
	if (!VerboseEnabled) {
		cout << "Verbose Mode Enabled";
		VerboseEnabled = TRUE;
	}
	else {
		cout << "Verbose Mode Disabled";
		VerboseEnabled = FALSE;
	}
} // VerboseModeSelector

void WallCollisionSelector() {
	// Enables/Disables wall collisions (Debug Option)
	if (WallCollisionsEnabled) {
		cout << "Wall Collisions Disabled";
		WallCollisionsEnabled = FALSE;
	}
	else {
		cout << "Wall Collisions Enabled";
		WallCollisionsEnabled = TRUE;
	}
} // WallCollisionSelector

void TailCollisionSelector() {
	// Enables/Disables tail collisions (Debug Option)
	if (TailCollisionsEnabled) {
		cout << "Tail Collisions Disabled";
		TailCollisionsEnabled = FALSE;
	}
	else {
		cout << "Tail Collisions Enabled";
		TailCollisionsEnabled = TRUE;
	}
} // TailCollisionSelector

void ShowDebugMenu() {
	system("cls");
	cout << "Debug Menu. Press the corresponding key for the desired menu option. \nPress any other key to return to the game." << endl;
	cout << "1. Enable/Disable Verbose Mode\n2. Disable/Enable Wall Collisions\n3. Disable/Enable Tail Collisions\n4. Add 10 Tail Segments (Requires Tail Collisions to be Disabled)\n5. Add 100 Tail Segments (Requires Tail Collisions to be Disabled)\n";
	switch (_getch()) {
	case '1':
		VerboseModeSelector();
		Sleep(1000);
		break;
	case '2':
		WallCollisionSelector();
		Sleep(1000);
		break;
	case '3':
		TailCollisionSelector();
		Sleep(1000);
		break;
	case '4':
		NumTailSegments += 10;
		Score += 100;
		cout << "10 Tail Segments Added";
		Sleep(1000);
		break;
	case '5':
		NumTailSegments += 100;
		Score += 1000;
		cout << "100 Tail Segments Added";
		Sleep(1000);
		break;
	}
} // ShowDebugMenu

void GetInput() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
		case 'A':
			if ((dir != RIGHT) || NumTailSegments < 1)
				dir = LEFT;
			break;
		case 'd':
		case 'D':
			if ((dir != LEFT) || NumTailSegments < 1)
				dir = RIGHT;
			break;
		case 'w':
		case 'W':
			if ((dir != DOWN) || NumTailSegments < 1)
				dir = UP;
			break;
		case 's':
		case 'S':
			if ((dir != UP) || NumTailSegments < 1)
				dir = DOWN;
			break;
		case 'x':
		case 'X':
			GameOver = TRUE;
			break;
		case 'p':
		case 'P':
			PauseGame();
			break;
		case 'm':
		case 'M':
			if (Debug)
				ShowDebugMenu();
			break;
		}
	}
} // GetInput

void SnakeMovement() {
	switch (dir) {
	case LEFT:
		SnakeX--;
		break;
	case RIGHT:
		SnakeX++;
		break;
	case UP:
		SnakeY--;
		break;
	case DOWN:
		SnakeY++;
		break;
	default:
		break;
	}
} // SnakeMovement

void TailMovement() {
	// Moves through each tail segment, and sets it to the current position of the segment in front of it.
	for (int i = (NumTailSegments - 1); i > 0; i--) {
		TailX[i] = TailX[i - 1];
		TailY[i] = TailY[i - 1];
	}
	// Moves the tail segments closest to the Snake to the Snake's current position.
	TailX[0] = SnakeX;
	TailY[0] = SnakeY;
} // TailMovement

void DetectWallCollisions() {
	// If wall collisiosn are enabled, ends the same when wall collisions occur
	if ((SnakeX >= PlayFieldWidth || SnakeX < 0 || SnakeY >= PlayFieldHeight ||
		SnakeY < 0) && WallCollisionsEnabled == TRUE)
		GameOver = true;
	// otherwise, if wall collisions are disabled, will handle wrap around functionality
	if (SnakeX >= PlayFieldWidth)
		SnakeX = 0;
	else if (SnakeX < 0)
		SnakeX = PlayFieldWidth - 1;
	if (SnakeY >= PlayFieldHeight)
		SnakeY = 0;
	else if (SnakeY < 0)
		SnakeY = PlayFieldHeight - 1;
} // DetectWallCollisions

void DetectSnakeCollisions() {
	for (int i = 0; i < NumTailSegments; i++)
		if (TailX[i] == SnakeX && TailY[i] == SnakeY && TailCollisionsEnabled == TRUE)
			GameOver = true;
} // DetectSnakeCollisions

void DetectFruitCollisions() {
	for (int i = 0; i < NumFruits; i++) {
		if (SnakeX == FruitX[i] && SnakeY == FruitY[i]) {
			Score += 10;
			FruitX[i] = rand() % PlayFieldWidth;
			FruitY[i] = rand() % PlayFieldHeight;
			NumTailSegments++;
		}
	}
} // DetectFruitCollisions

void LogicManager() {
	// Calls functions handling all gameplay logic.
	DetectFruitCollisions();
	TailMovement();
	SnakeMovement();
	DetectWallCollisions();
	DetectSnakeCollisions();
} // LogicManager

void FruitSelector() {
	// Allows user to select number of fruits present. Then validates the entry.
	system("cls");
	cout << "Enter Number of Fruits: (Between 1-20):  ";
	for (;;)
		if (cin >> NumFruits && NumFruits <= 20) {
			break;
		}
		else {
			cout << "Invalid Entry! Please enter a number between 1 and 20:  ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
} // FruitSelector

void GameModeHandler() {
	// Allows user to select X to quit the game, or any other key to play the game. This appears at the instruction screen, and after every game over.
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	switch (_getch()) {
	case 'x':
	case 'X':
		GameLooper = false;
		break;
	default:
		FruitSelector();
		GameSetup();
		GameOver = false;
		break;
	}
} // GameModeHandler

void GameInit() {
	// Only runs before the first game. Hides the cursor, displays the instructions, then calls GameModeHandler() to setup new game.
	HideCursor();
	cout << "Welcome to Snake! \nControl the snake to collect fruit.\nTouching your tail, or the edges of the playing field will end the game! \n____________________________________________________\n\nUse WASD to control the snake. \nPress P to pause the game. \nPress X at any time to quit." << endl;
	cout << "Press any key to begin: ";
	GameModeHandler();
} // GameInit

void GameEnd() {
	cout << "Game Over! Your score is " << Score << "!" << endl;
	Sleep(1000); //Pause to ensure user has a chance to see score, and doesn't accidentally press a key too early
	cout << "Press any key to play again or press X to quit";
	GameModeHandler();
} // GameEnd

void GameWin() {
	// Ends the game when the max score is reached is reached
	cout << "Congratulations! You Win!";
	Sleep(2000);
	exit(0);
} // GameWin

int main() {
	GameInit();
	while (GameLooper == true) {
		while (!GameOver) {
			DrawGameBoard();
			if (Score >= MaxScore) {
				GameWin();
			}
			// Reduces speed variability between the axes.
			if (dir == LEFT || dir == RIGHT) {
				Sleep(5);
			}
			if (dir == UP || dir == DOWN) {
				Sleep(20);
			}
			GetInput();
			LogicManager();
		}
		GameEnd();
	}
	return 0;
} // main