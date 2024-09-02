#include "stdafx.h"
#include "Unit.h"

Unit* Unit::create(const Size& size, int bitmask, int tag)
{
	auto ret = new Unit();
	if (ret && ret->init(size, bitmask, tag)) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Unit::init(const Size& size, int bitmask, int tag)
{
	if (!Node::init()) return false;

	body = PhysicsBody::createBox(size);
	body->setTag(tag);
	body->setCollisionBitmask(0);
	body->setContactTestBitmask(bitmask);	// 두개의 body가 충돌했을 때 
	addComponent(body);
	body->setRotationEnable(false);

	return true;
}

PhysicsBody* Unit::getBody()
{
	return body;
}

void Unit::setHP(float value)
{
	this->hp = value;
}

float Unit::getHP()
{
	return this->hp;
}

void Unit::addHP(float value)
{
	setHP(getHP() + value);
}

bool Unit::isHPZero()
{
	return getHP() <= 0.0f;
}

Player* Player::create()
{
	auto ret = new Player();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Player::init()
{
	if (!Unit::init(Size(75, 75), PLAYER_MASK, TAG_PLAYER)) return false;

	addChild(spr = Sprite::create("res/playerShip1_blue.png"));

	Sprite* tail = Sprite::create("res/Effects/fire11.png");
	Animation* animation = Animation::create();
	animation->addSpriteFrameWithFile("res/Effects/fire11.png");
	animation->addSpriteFrameWithFile("res/Effects/fire12.png");
	animation->addSpriteFrameWithFile("res/Effects/fire13.png");
	animation->addSpriteFrameWithFile("res/Effects/fire14.png");
	animation->addSpriteFrameWithFile("res/Effects/fire15.png");
	animation->addSpriteFrameWithFile("res/Effects/fire16.png");
	animation->addSpriteFrameWithFile("res/Effects/fire17.png");
	animation->setDelayPerUnit(0.125f);
	Animate* animate = Animate::create(animation);
	tail->runAction(RepeatForever::create(animate));
	tail->setPosition(Vec2(0, -60));

	addChild(tail);

	setHP(100);

	return true;
}

void Player::removeFromParent()
{
	Sprite* effect = Sprite::create("res/Damage/playerShip1_damage1.png");
	Animation* animation = Animation::create();
	animation->addSpriteFrameWithFile("res/Damage/playerShip1_damage1.png");
	animation->addSpriteFrameWithFile("res/Damage/playerShip1_damage2.png");
	animation->addSpriteFrameWithFile("res/Damage/playerShip1_damage3.png");
	animation->setDelayPerUnit(0.125f);
	Animate* animate = Animate::create(animation);
	effect->runAction(Sequence::create(animate, RemoveSelf::create(), nullptr));
	effect->setPosition(getPosition());

	Director::getInstance()->getRunningScene()->addChild(effect);
	Node::removeFromParent();
}

Enemy* Enemy::create()
{
	auto ret = new Enemy();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool Enemy::init()
{
	if (!Unit::init(Size(75, 75), ENEMY_MASK , TAG_ENEMY)) return false;

	addChild(spr = Sprite::create("res/ufoRed.png"));

	setHP(50);

	return true;
}

void Enemy::removeFromParent()
{
	removeComponent(body);
	body = nullptr;
	runAction(Sequence::create(
		Spawn::create(FadeOut::create(0.5f), ScaleTo::create(0.5, 0.0f), nullptr),
		RemoveSelf::create(),
		nullptr
	));
}

