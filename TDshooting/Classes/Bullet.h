#ifndef __BULLET_H__
#define __BULLET_H__

#include "stdafx.h"

class Bullet : public Node {
private:
	PhysicsBody* body;
	Sprite* spr;
public:
	static Bullet* create(int bitmask, int tag);
	virtual bool init(int bitmask, int tag);	// 두가지 파라미터를 받은 함수가 없기때문에 override를 붙이지 않는다. 단, 파라미터가 없으면 붙여야 됨.

	PhysicsBody* getBody();
	Sprite* getSprite();

};




#endif