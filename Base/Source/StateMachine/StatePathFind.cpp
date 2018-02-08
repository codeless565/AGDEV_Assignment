#include "StatePathFind.h"

#include "../Message/PostOffice.h"
#include "../Message/ConcreteMessages.h"

#include "../Waypoint/WaypointManager.h"

/****************************************
Dota Minion State: PathFindNormal
****************************************/
StatePF_Normal::StatePF_Normal(const std::string & stateID, CEnemy * go)
	:State(stateID),
	m_go(go)
{
}

StatePF_Normal::~StatePF_Normal()
{
}

void StatePF_Normal::Enter()
{
	m_go->nearby = NULL;

	CWaypoint* nextnode = CWaypointManager::GetInstance()->GetNearestWaypoint(m_go->GetPos());
	m_go->currWaypointID = nextnode->GetID();
	m_go->SetTarget(nextnode->GetPosition());
}

void StatePF_Normal::Update(double dt)
{
	//pathFind
	Vector3 dir = m_go->GetTarget() - m_go->GetPos();
	if (dir.Length() <= m_go->speed * static_cast<float>(dt))
	{//Reached the next node
		m_go->SetPos(m_go->GetTarget());
		PostOffice::GetInstance()->Send("GAME", new MessageWRU(m_go, MessageWRU::MESSAGE_GETNEXTNODE));
	}
	else
	{//Move toward the next node
		m_go->SetPos(m_go->GetPos() + (m_go->GetTarget() - m_go->GetPos()).Normalized() * m_go->speed * static_cast<float>(dt));
	}

	//Check for enemy
	PostOffice::GetInstance()->Send("GAME", new MessageWRU(m_go, MessageWRU::MESSAGE_CHECKNEARBY));

	if (m_go->nearby)
		m_go->sm->SetNextState("Attack");
}

void StatePF_Normal::Exit()
{
}
