#include "stdafx.h"
#include "SceneIngame.h"
#include "Bullet.h"
#include "Unit.h"
#include "EnemyComponent.h"

SceneIngame* SceneIngame::create()
{
	auto ret = new SceneIngame();
	if(ret && ret->init()) ret-> autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneIngame::init()
{
	if (!Scene::initWithPhysics()) return false;

	auto keybd = EventListenerKeyboard::create();
	keybd->onKeyPressed = std::bind(&SceneIngame::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keybd->onKeyReleased = std::bind(&SceneIngame::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybd, this);

	auto contact = EventListenerPhysicsContact::create();
	contact->onContactBegin = std::bind(&SceneIngame::onContactBegin, this, std::placeholders::_1);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact, this);

	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic));
	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::fireLogic), 0.1);

	return true;
}

void SceneIngame::onEnter()
{
	Scene::onEnter();	// 부모 클래스에서 override 해 온 것이므로 호출해야 한다.

	auto world = getPhysicsWorld();
	if(DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	world->setGravity(Vec2::ZERO);

	player = Player::create();
	player->setPosition(Vec2(1280 / 2, 720 / 2 - 200));
	addChild(player);

	{
		auto enemy = Enemy::create();
		enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::NORMAL));
		enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_TOP));
		addChild(enemy);
	}
	{
		auto enemy = Enemy::create();
		enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::DOUBLE));
		enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_LEFT));
		addChild(enemy);
	}
	{
		auto enemy = Enemy::create();
		enemy->addComponent(EnemyAttackRoutine::create(EnemyAttackType::TRIPLE));
		enemy->addComponent(EnemyMovementRoutine::create(EnemyMovementType::FROM_RIGHT));
		addChild(enemy);
	}
}

Unit* SceneIngame::getPlayer()
{
	return player;
}

void SceneIngame::onKeyPressed(EventKeyboard::KeyCode c, Event* e)
{
	const bool value = true;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up =  value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

void SceneIngame::onKeyReleased(EventKeyboard::KeyCode c, Event* e)
{
	const bool value = false;
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up = value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

bool SceneIngame::collision(Unit* u, float damage)
{
	u->addHP(-damage);
	if (u->isHPZero())
	{
		CCLOG("this is error.");
		CCLOG("%f", u->getHP());
		u->removeFromParent();
		return true;
	}
	return false;
}

bool SceneIngame::onContactBegin(PhysicsContact& contact)
{
	int tagA = contact.getShapeA()->getBody()->getTag();
	int tagB = contact.getShapeB()->getBody()->getTag();
	Node* a = contact.getShapeA()->getBody()->getNode();
	Node* b = contact.getShapeB()->getBody()->getNode();

	if (a == nullptr) return true;
	if (b == nullptr) return true;

	if (tagA == TAG_PLAYER_BULLET && tagB == TAG_ENEMY) {
		bool dead = this->collision((Unit*)b, 15);
		a->removeFromParent();
	}
	if (tagB == TAG_PLAYER_BULLET && tagA == TAG_ENEMY) {
		this->collision((Unit*)a, 15);
		b->removeFromParent();
	}

	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY) {
		bool dead = this->collision((Unit*)a, 50);
		if (dead) player = nullptr;
		this->collision((Unit*)b, 50);
	}
	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY) {
		bool dead = this->collision((Unit*)b, 50);
		if (dead) player = nullptr;
		this->collision((Unit*)a, 50);
	}

	if (tagA == TAG_PLAYER && tagB == TAG_ENEMY_BULLET) {
		bool dead = this->collision((Unit*)a, 100);
		if (dead) player = nullptr;
		b->removeFromParent();
	}
	if (tagB == TAG_PLAYER && tagA == TAG_ENEMY_BULLET) {
		bool dead = this->collision((Unit*)b, 100);
		if (dead) player = nullptr;
		a->removeFromParent();
	}

	return true;
}

void SceneIngame::logic(float dt)
{
	if (!player) return;

	Vec2 pos = player->getPosition();
	if (up) pos += Vec2(0, dt * PLAYER_MOVEMENT_SPEED);
	if (down) pos -= Vec2(0, dt * PLAYER_MOVEMENT_SPEED);

	if (left) pos -= Vec2(dt * PLAYER_MOVEMENT_SPEED, 0);
	if (right) pos += Vec2(dt * PLAYER_MOVEMENT_SPEED, 0);

	player->setPosition(pos);

}

void SceneIngame::fireLogic(float dt)
{
	if (!player) return;

	if (fire) {
		auto bullet = Bullet::create(PLAYER_BULLET_MASK, TAG_PLAYER_BULLET);
		bullet->setPosition(player->getPosition() + Vec2(0, 60));
		bullet->getBody()->setVelocity(Vec2(0, 1000));
		bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr));
		addChild(bullet);
	}
}
