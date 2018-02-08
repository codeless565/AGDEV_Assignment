#include "StateAttack.h"

#include "../Message/PostOffice.h"
#include "../Message/ConcreteMessages.h"

/****************************************
Dota Minion State: Attack
****************************************/
StateAttack::StateAttack(const std::string & stateID,  CEnemy * go)
	:State(stateID),
	m_go(go)
{
}

StateAttack::~StateAttack()
{
}

void StateAttack::Enter()
{
	m_go->SetTarget(m_go->nearby->GetPos());
}

void StateAttack::Update(double dt)
{
	m_go->SetTarget(m_go->nearby->GetPos());

	Vector3 dir = m_go->GetTarget() - m_go->GetPos();
	if (dir.Length() >= 50 || !m_go->nearby)
	{
		m_go->sm->SetNextState("PathFind");
	}

	m_go->SetPos(m_go->GetPos() + (m_go->GetTarget() - m_go->GetPos()).Normalized() * m_go->speed * static_cast<float>(dt));

}

void StateAttack::Exit()
{
}
