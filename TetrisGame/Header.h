#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstring>
#include "TetrisHelper.h"

#define GRID_WIDTH 14
#define GRID_HEIGHT 20
#define USERBLOCK_SIZE 4
#define SCOREBOARD 8

extern int displayData[GRID_HEIGHT][GRID_WIDTH + SCOREBOARD];

class Display {
public:
	
	void draw() {
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				// drawPosition(i, k, displayData[i][k] == 1);
				// 아래 코드와 같은 코드임
				if (displayData[i][k] == 0) {
					drawPosition(k, i, false);
				}
				else {
					drawPosition(k, i, true);
				}
			}
		}
		
		for (int i = 18; i < 22; i++) {
			for (int k = USERBLOCK_SIZE; k < SCOREBOARD; k++) {
				drawPosition(i, k, displayData[i - 14][k + 14] == 1);
			}
		}
	}
};

class GameEngine {
public:
	enum class GameState {
		PLAYING, GAMEOVER
	};
	GameState gamestate = GameState::PLAYING;

	int gameGridData[GRID_HEIGHT + 1][GRID_WIDTH] = { '\0'};

	int userBlock[USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	int tempUserBlock[USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	int savedUserBlock[USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	int savedTempUserBlock[USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
	};

	int userBlockVarious[5][USERBLOCK_SIZE][USERBLOCK_SIZE] = {
		{{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1}},

		{{0, 0, 1, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 0}},

		{{0, 0, 1, 0},
		{0, 1, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}},

		{{0, 0, 1, 1},
		{0, 0, 1, 1},
		{0, 0, 0, 0},
		{0, 0, 0, 0}},

		{{0, 0, 1, 1},
		{0, 0, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0}}
	};

	int blockX = 0;
	int blockY = 0;
	int count = 0;
	int score = 0;

	float elapsed = 0.0f;
	float controlCheck = 0.0f;
	float rotateCheck = 0.5f;
	float switchCheck = 1.0f;
	float maxLimit = 0.0f;

	void init() {
		// 최초에 게임 엔진을 초기화 하는 과정을 맡음.
		makeUserBlock();
		displayData[0][21] = 0;
	}

	// next 는 while 루프에서 매번 불려지는 함수가 됨.
	void next(float dt, char keyboardInput) {
		if (gamestate == GameState::GAMEOVER) return;
		elapsed = elapsed + dt;
		if (elapsed >= 0.5f) {
			if (canGoDown()) {
				blockY++;
			}
			else {
				// userblock 을 gameGridData 에 전사
				trans();
				if (gameOverDecision()) gamestate = GameState::GAMEOVER;
			}
			
			elapsed -= 0.5f;
		}

		controlCheck += dt;
		rotateCheck += dt;
		switchCheck += dt;

		if (keyboardInput == 'a' && canGoLeft() && controlCheck > 0.1) {
			blockX--;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 'd' && canGoRight() && controlCheck > 0.1) {
			blockX++;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 's' && canGoDown() && controlCheck > 0.1) {
			blockY++;
			controlCheck = 0.0f;
		}
		if (keyboardInput == 'w' && rotateCheck > 0.5) {
			if (canGoRotate()) {
				rotate();
				rotateCheck = 0.0f;
			}
		}
		if (keyboardInput == ' ' && switchCheck > 1.0) {
			saveUserBlock(count);
			count++;
			switchCheck = 0.0f;
		}
	}

	bool canGoDown() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && i + blockY + 1 >= GRID_HEIGHT)
					return false;
				if (userBlock[i][k] == 1 && gameGridData[i + blockY + 1][k + blockX] == 1) {
					return false;
				}
			}
		}
		return true;
	}

	bool canGoRight() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && k + blockX >= GRID_WIDTH - 1) {
					return false;
				}
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX + 1] == 1) {
					return false;
				}
			}
		}
		return true;
	}
	bool canGoLeft() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && k + blockX - 1 < 0)
					return false;
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX - 1] == 1)
					return false;
			}
		}
		return true;
	}

	bool canGoRotate() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (tempUserBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX] == 1) {
					return false;
				}
			}
		}
		return true;
	}

	bool isLineFilled(int y) {
		int count = 0;

		for (int i = 0; i < GRID_WIDTH; i++) {
			if (gameGridData[y][i] == 0) {
				return false;
			}
		}
		return true;
	}

	void drop(int y) {
		for (int i = y; i > 0; i--) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				gameGridData[i][k] = gameGridData[i - 1][k];
			}
		}
	}

	void growUp() {
		for (int i = 1; i < GRID_HEIGHT; i++) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				gameGridData[i][k] = gameGridData[i + 1][k];
			}
		}
	}

	void eraseLine(int y) {
		for (int i = 0; i < GRID_WIDTH; i++) {
			gameGridData[y][i] = 0;
		}
	}

	// userblock 을 gameGrid 에 전사하는 함수
	void trans() {

		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				int _y = i + blockY;
				int _x = k + blockX;

				// gameGridData[i + blockY][k + blockX] = userBlock[i][k] == 1 ? userBlock[i][k] : gameGridData[i + blockY][k + blockX];
				gameGridData[_y][_x] = userBlock[i][k] | gameGridData[_y][_x];
			}
		}

		// 한 줄이 가득 차 있는지 확인
		for (int i = 0; i < GRID_HEIGHT; i++) {
			if (isLineFilled(i)) {
				eraseLine(i);
				drop(i);
				getScore();
			}
		}

		// 새로운 블록 생성
		makeUserBlock();
	}

	bool gameOverDecision() {
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (userBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX] == 1) {
					return true;
				}
			}
		}
		return false;
	}

	void makeUserBlock() {
		blockX = 0;
		blockY = 0;

		int various = rand() % 5;
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				userBlock[i][k] = userBlockVarious[various][i][k];
			}
		}
		// ToDo 랜덤 벽돌을 생성한다.
	}

  	void saveUserBlock(int count) {
		if (count > 0) {
			for (int i = 0; i < USERBLOCK_SIZE; i++) {
				for (int k = 0; k < USERBLOCK_SIZE; k++) {
					savedTempUserBlock[i][k] = savedUserBlock[i][k];
					savedUserBlock[i][k] = userBlock[i][k];
					userBlock[i][k] = savedTempUserBlock[i][k];
				}
			}
		}
		else {
			for (int i = 0; i < USERBLOCK_SIZE; i++) {
				for (int k = 0; k < USERBLOCK_SIZE; k++) {
					savedUserBlock[i][k] = userBlock[i][k];
				}
			}
			makeUserBlock();
		}
	}

	void makeBlockLastLine() {
		int randBlockLine[1][GRID_WIDTH] = { '\0' };

		for (int i = 0; i < GRID_WIDTH; i++) {
			randBlockLine[0][i] = rand() % 2;
		}

		for (int i = 0; i < GRID_WIDTH; i++) {
			gameGridData[GRID_HEIGHT][i] = randBlockLine[0][i];
		}

		growUp();
	}

	void rotate() {
		// 회전을 한다.
		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				tempUserBlock[i][k] = userBlock[k][3 - i];
			}
		}

		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				if (tempUserBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX] == 1 && i + blockY > 0) {
					blockY--;
				}
				else if (tempUserBlock[i][k] == 1 && gameGridData[i + blockY][k + blockX] == 1 && i + blockY <= 0) {
					GameState::GAMEOVER;
				}
				if (tempUserBlock[i][k] == 1 && k + blockX < 0) {
					blockX++;
				}
				if (tempUserBlock[i][k] == 1 && k + blockX >= GRID_WIDTH) {
					blockX--;
				}
			}
		}

		for (int i = 0; i < USERBLOCK_SIZE; i++) {
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				userBlock[i][k] = tempUserBlock[i][k];
			}
		}
	}

	void getScore() {
		score++;
		displayData[0][21] = score;
		printf("\t%d", displayData[0][21]);
	}

	// 실제 게임 데이터를 화면에 출력할 수 있는 데이터로 바꿈
	void makeDisplayData() {
		for (int i = 0; i < GRID_HEIGHT; i++) {
			for (int k = 0; k < GRID_WIDTH; k++) {
				displayData[i][k] = gameGridData[i][k];
			}
		}

 		for (int i = GRID_WIDTH + 4; i < GRID_WIDTH + SCOREBOARD; i++) {
			int j = i - 18;
			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				displayData[i - 14][k + 18] = savedUserBlock[j][k];
			}
		}

		for (int i = 0; i < USERBLOCK_SIZE; i++) {

			for (int k = 0; k < USERBLOCK_SIZE; k++) {
				int _y = i + blockY;
				int _x = k + blockX;

				if (_y < 0 || _y > GRID_HEIGHT) {
					// Do Nothing
				}
				else if (_x < 0 || _x > GRID_WIDTH) {
					// Do Nothing
				}
				else {// TODO:
					// displayData[i + blockY][k + blockX] = userBlock[i][k] == 1 ? userBlock[i][k] : displayData[i + blockY][k + blockX];
					// 위에 보다 아래가 연산 속도가 빠름 Bitwise or 연산자 | 활용
					// 또한 계산이 중복되는 연산은 변수 지정을 해주는게 속도가 빠름
					displayData[_y][_x] = userBlock[i][k] | displayData[_y][_x];
				}
			}
		}
	}
};