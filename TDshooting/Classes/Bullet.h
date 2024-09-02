#ifndef __BULLET_H__
#define __BULLET_H__

#include "stdafx.h"

class Bullet : public Node {
private:
	PhysicsBody* body;
	Sprite* spr;
public:
	static Bullet* create(int bitmask, int tag);
	virtual bool init(int bitmask, int tag);	// �ΰ��� �Ķ���͸� ���� �Լ��� ���⶧���� override�� ������ �ʴ´�. ��, �Ķ���Ͱ� ������ �ٿ��� ��.

	PhysicsBody* getBody();
	Sprite* getSprite();

};




#endif