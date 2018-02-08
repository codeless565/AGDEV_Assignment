#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "../StateMachine/StatePathFind.h"
#include "../StateMachine/StateAttack.h"

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f,0.0f,0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_SpawnTimer(0.f)
	, m_SwapTimer(0.f)
	, nearby(NULL)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(COLOR _color, Vector3 _Pos, Vector3 _Target)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	SetPosition(_Pos); //<<<<<NEEd to change to distinctive position
	SetTarget(_Target);
	scale.Set(5.f, 5.f, 5.f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 1.0;

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(0.5f * scale.x, 0.5f * scale.y, 0.5f * scale.z), Vector3(-0.5f * scale.x, -0.5f * scale.y, -0.5f * scale.z));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

	//=================== LOD ==========================
	Master_Color = _color;

	InitLOD("MS_High", "MS_Med", "MS_Low"); // TODO LOD

	//Create a Graph for these Blocks
	this->setEntityType(EntityBase::ENTITY_ENEMIES);
	MasterNode = CSceneGraph::GetInstance()->AddNode(this);


	if (Master_Color == COLOR_NONE)
	{
		speed = 20.f;

		sm = new StateMachine();
		sm->AddState(new StatePF_Normal("PathFind", this));
		sm->AddState(new StateAttack("Attack", this));
		sm->SetNextState("PathFind");
	}
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Update
void CEnemy::Update(double dt)
{
	if (Master_Color == COLOR_NONE)
	{
		sm->Update(dt);
		return;
	}

	m_SpawnTimer += (float)dt;
	m_SwapTimer += (float)dt;

	if (m_SpawnTimer >= 1.0f)
	{
		m_SpawnTimer = 0.f;
		SpawnChild();
	}
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus())
	{
		//cout << theDetailLevel << endl;
		if (theDetailLevel != NO_DETAILS)
		{
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

void CEnemy::InitSpawnChild(int _num)
{
	for (int i = 0; i < _num; ++i)
		SpawnChild();
}

void CEnemy::SpawnChild()
{
	vector<CSceneNode*> ChildBlocks = MasterNode->GetChildren();

	std::vector<CSceneNode*>::iterator it, end;
	end = ChildBlocks.end();

	do
	{
		float RandPosX = Math::RandIntMinMax(-3, 3) * scale.x * 2 + position.x;
		float RandPosY = Math::RandIntMinMax(-3, 3) * scale.x * 2 + position.y;

		if (RandPosX == position.x && RandPosY == position.y)
			continue;

		Vector3 TempPos(RandPosX, RandPosY, position.z);

		if (ChildBlocks.empty())
		{
			GenericEntity* childBlock = Create::Entity("cubeSG", TempPos);
			switch (Master_Color)
			{
			case COLOR_RED:
				childBlock->InitLOD("CSRed_High", "CSRed_Med", "CSRed_Low");
				break;
			case COLOR_BLUE:
				childBlock->InitLOD("CSBlue_High", "CSBlue_Med", "CSBlue_Low");
				break;
			case COLOR_GREEN:
				childBlock->InitLOD("CSGreen_High", "CSGreen_Med", "CSGreen_Low");
				break;
			default:
				childBlock->InitLOD("CSYellow_High", "CSYellow_Med", "CSYellow_Low");
			}
			childBlock->SetScale(Vector3(5.f, 5.f, 5.f));
			childBlock->SetCollider(true);
			childBlock->SetAABB(Vector3(0.5f * GetScale().x, 0.5f* GetScale().y, 0.5f* GetScale().z), Vector3(-0.5f* GetScale().x, -0.5f* GetScale().y, -0.5f* GetScale().z));
			MasterNode->AddChild(childBlock);
			return;
		}
		else
		{

			for (auto it : ChildBlocks)
			{
				Vector3 dist = TempPos - it->GetEntity()->GetPosition();

				if (dist.Length() > scale.x * 2)
				{
					GenericEntity* childBlock = Create::Entity("cubeSG", TempPos);
					switch (Master_Color)
					{
					case COLOR_RED:
						childBlock->InitLOD("CSRed_High", "CSRed_Med", "CSRed_Low");
						break;
					case COLOR_BLUE:
						childBlock->InitLOD("CSBlue_High", "CSBlue_Med", "CSBlue_Low");
						break;
					case COLOR_GREEN:
						childBlock->InitLOD("CSGreen_High", "CSGreen_Med", "CSGreen_Low");
						break;
					default:
						childBlock->InitLOD("CSYellow_High", "CSYellow_Med", "CSYellow_Low");
					}
					childBlock->SetScale(Vector3(5.f, 5.f, 5.f));
					childBlock->SetCollider(true);
					childBlock->SetAABB(Vector3(0.5f * GetScale().x, 0.5f* GetScale().y, 0.5f* GetScale().z), Vector3(-0.5f* GetScale().x, -0.5f* GetScale().y, -0.5f* GetScale().z));
					MasterNode->AddChild(childBlock);
					return;
				}
			}
		}
	} while (true);
}

void CEnemy::SwapPosition()
{
	vector<CSceneNode*> ChildBlocks = MasterNode->GetChildren();

	std::vector<CSceneNode*>::iterator it;
	it = ChildBlocks.begin();

	int counter = 0;

	do
	{
		int random = Math::RandIntMinMax(0, ChildBlocks.size() - 1);
		std::vector<CSceneNode*>::iterator randit = it + random;

		if ((*randit)->GetEntity()->IsDone())
			continue;

		if ((*randit)->GetEntity())
		{
			Vector3 tempChildPos = (*randit)->GetEntity()->GetPosition();
			(*randit)->GetEntity()->SetPosition(position);
			SetPosition(tempChildPos);
			return;
		}
		++counter;

	} while (counter < 5);
}

/********************************************************
***************** SETTER & GETTER ***********************
*********************************************************/
// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}