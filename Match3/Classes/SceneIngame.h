#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"
#include "LayerIngameUI.h"

class SceneIngame : public Scene {
private:
	GameState state;
	// blockData 는 0값일 경우 비어있는 블록, 0이 아닌 양수값일 경우 블록
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];	// 블록 데이터

	// blockSprite는 nullptr 일 경우 비어있음
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];	// blockData 기반으로 화면에 뿌려지는 Sprite

	// 유니크 스택을 위한 자료구조
	Vec2 judgeStack[128];
	// 스택에 있는 자료의 수
	int judgeStackCount = 0;
	// 0 이라면 스택에 자료가 없음, 그게 아니라면 자료가 있음
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	// == 
	bool isDrop = false;

	void createBlock(int x, int y, int type);	// 블럭을 만들어주는 함수
	int getBlockData(int x, int y);	// 블럭을 가져오는 함수
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);	// 게임coord 값을 블럭으로 변환
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);	// 블럭을 게임coord 값으로 변환

	// 아래에서부터 찾아 올라가면서 비어있는 블록을 찾고 -1 이 리턴이 되면 비어있는 블록이 없다는 뜻
	int findEmptyBlockYIndex(int x, int y);
	// y위치부터 찾아 올라가면서 비어있지 않은 블록을 찾고 -1 이 리턴이 되면 비어있지 않은 블록이 없다는 뜻
	int findFilledBlockYIndex(int x, int y);
	// 블록을 떨어뜨리는 함수
	void dropBlocks(int x);

	void stackPush(Vec2 value);
	Vec2 stackPop();
	void stackEmpty();
	bool stackFind(Vec2 value);
	void judgeMatch(int x, int y);

	LayerIngameUI* ui;

public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override;	// 상위 노드에 붙어 실행이 될 때 자동으로 실행되는 callback 함수

	void initUI();	// UI 초기화
	void initGame();	// 게임 최초 초기화
	void destroyUI();	// UI 파괴
	void destroyGame();	// 게임이나 스프라이트 파괴

	void alignBlockSprite();

	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);

	void startGame();
	void pauseGame();
	void winGame();
	void loseGame();
};


#endif