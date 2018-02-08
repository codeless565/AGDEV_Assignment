#ifndef STATE_ATTACK_H
#define STATE_ATTACK_H

#include "State.h"
#include "../Enemy/Enemy.h"

/*****************************
Dota Minion State: Attack
*****************************/
class StateAttack : public State
{
	CEnemy *m_go;
public:
	StateAttack(const std::string &stateID, CEnemy *go);
	~StateAttack();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};
#endif