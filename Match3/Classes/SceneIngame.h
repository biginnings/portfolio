#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"
#include "LayerIngameUI.h"

class SceneIngame : public Scene {
private:
	GameState state;
	// blockData �� 0���� ��� ����ִ� ���, 0�� �ƴ� ������� ��� ���
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];	// ��� ������

	// blockSprite�� nullptr �� ��� �������
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];	// blockData ������� ȭ�鿡 �ѷ����� Sprite

	// ����ũ ������ ���� �ڷᱸ��
	Vec2 judgeStack[128];
	// ���ÿ� �ִ� �ڷ��� ��
	int judgeStackCount = 0;
	// 0 �̶�� ���ÿ� �ڷᰡ ����, �װ� �ƴ϶�� �ڷᰡ ����
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];
	// == 
	bool isDrop = false;

	void createBlock(int x, int y, int type);	// ���� ������ִ� �Լ�
	int getBlockData(int x, int y);	// ���� �������� �Լ�
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlock(int x, int y);

	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);	// ����coord ���� ������ ��ȯ
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);	// ���� ����coord ������ ��ȯ

	// �Ʒ��������� ã�� �ö󰡸鼭 ����ִ� ����� ã�� -1 �� ������ �Ǹ� ����ִ� ����� ���ٴ� ��
	int findEmptyBlockYIndex(int x, int y);
	// y��ġ���� ã�� �ö󰡸鼭 ������� ���� ����� ã�� -1 �� ������ �Ǹ� ������� ���� ����� ���ٴ� ��
	int findFilledBlockYIndex(int x, int y);
	// ����� ����߸��� �Լ�
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
	virtual void onEnter() override;	// ���� ��忡 �پ� ������ �� �� �ڵ����� ����Ǵ� callback �Լ�

	void initUI();	// UI �ʱ�ȭ
	void initGame();	// ���� ���� �ʱ�ȭ
	void destroyUI();	// UI �ı�
	void destroyGame();	// �����̳� ��������Ʈ �ı�

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