#ifndef STATE_PATHFIND_H
#define STATE_PATHFIND_H

#include "State.h"
#include "../Enemy/Enemy.h"

/*****************************
Dota Minion State: PathFindNormal
*****************************/
class StatePF_Normal : public State
{
	CEnemy *m_go;
public:
	StatePF_Normal(const std::string &stateID, CEnemy *go);
	~StatePF_Normal();

	virtual void Enter();
	virtual void Update(double dt);
	virtual void Exit();
};

#endif