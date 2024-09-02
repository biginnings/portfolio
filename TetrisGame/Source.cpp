#include "Header.h"

#define INTERVAL 1.0 / 60.0

int displayData[GRID_HEIGHT][GRID_WIDTH + 8] = { 0, };

int main() {
	srand(time(0));

	float prev = (float)clock() / CLOCKS_PER_SEC; // 시간 초 계산
	float prevGrow = (float)clock() / CLOCKS_PER_SEC; 
	Display* display = new Display();
	GameEngine* gameEngine = new GameEngine();
	showConsoleCursor(false);

	gameEngine->init();

	while (true) {
		float curr = (float)clock() / CLOCKS_PER_SEC;
		float curGrow = (float)clock() / CLOCKS_PER_SEC;
		float dt = curr - prev;
		float dtGrow = curGrow - prevGrow;

		if (dt < INTERVAL) continue;
		prev = curr;

		bool left = keyState('a');
		bool right = keyState('d');
		bool down = keyState('s');
		bool rotate = keyState('w');
		bool space = keyState(' ');

		if (left) {
			gameEngine->next(dt, 'a');
		}
		else if (right) {
			gameEngine->next(dt, 'd');
		}
		else if (down) {
			gameEngine->next(dt, 's');
		}
		else if (rotate) {
			gameEngine->next(dt, 'w');
		}
		else if (space) {
			gameEngine->next(dt, ' ');
		}
		else {
			gameEngine->next(dt, 0);
		}

		if (dtGrow > 4.0f) {
			prevGrow = curGrow;
			gameEngine->makeBlockLastLine();
		}

		gameEngine->makeDisplayData();
		display->draw();// 화면 출력
		// 게임 상태 판별

		if (gameEngine->gamestate == GameEngine::GameState::GAMEOVER) {
			break;
		}
	}

	return 0;
}

