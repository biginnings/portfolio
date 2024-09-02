#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H_

#define DEBUG_MODE false		//Debug draw�� ������. �ڵ带 ��� �ϼ��ϰ� ������ false�� �ٲ㼭 ����� ��ο찡 �� �������� ��.

#define PLAYER_MOVEMENT_SPEED 400
#define ENEMY_BULLET_SPEED 100

// 011(2)
#define PLAYER_MASK 0x00000003
// 110(2)
#define ENEMY_MASK 0x00000006
// 100(2) 
#define PLAYER_BULLET_MASK 0x00000004
// 001(2)
#define ENEMY_BULLET_MASK 0x00000001

#define TAG_PLAYER 1
#define TAG_ENEMY 2
#define TAG_PLAYER_BULLET 3
#define TAG_ENEMY_BULLET 4

#define PI 3.14159265358
#define GET_RAD(X) (X * (PI/180.0))

#endif