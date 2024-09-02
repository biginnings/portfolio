#include "stdafx.h"
#include "Bullet.h"

Bullet* Bullet::create(int bitmask, int tag)
{
	auto ret = new Bullet();
	if (ret && ret->init(bitmask, tag)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Bullet::init(int bitmask, int tag)
{
	if (!Node::init()) return false;
	
	body = PhysicsBody::createCircle(5.0);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);
	body->setTag(tag);
	addComponent(body);

	if (tag == TAG_PLAYER_BULLET) {
		addChild(spr = Sprite::create("res/Lasers/laserBlue16.png"));
	}
	
	if (tag == TAG_ENEMY_BULLET) {
		addChild(spr = Sprite::create("res/Lasers/laserRed08.png"));
		spr->runAction(RepeatForever::create(RotateBy::create(1.0f, 80)));
	}
	return true;
}

PhysicsBody* Bullet::getBody()
{

	return this->body;
}
