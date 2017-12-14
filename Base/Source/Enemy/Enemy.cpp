#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../SceneGraph.h"
#include "../CUpdateTransformation.h"


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
	, Health(0)

{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(11.0f, 0.0f, 0.0f);
	target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.f;
	
	//// Initialise the Collider
	//this->SetCollider(true);
	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	//Body
	Body = Create::Entity("cube", Vector3(position.x, position.y, position.z), Vector3(5.f, 5.f, 5.f));
	Body->SetCollider(true);
	Body->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	Body->InitLOD("cube", "cube", "cube");
	BodyNode = CSceneGraph::GetInstance()->AddNode(Body);

	Head = Create::Entity("sphereHigh", Vector3(position.x, position.y + 5.f, position.z), Vector3(5.f, 5.f, 5.f));
	Head->SetCollider(true);
	Head->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	Head->InitLOD("sphereHigh", "sphereMed", "sphereLow");
	HeadNode = BodyNode->AddChild(Head);

	ArmL = Create::Entity("cubeSG", Vector3(position.x -5.f, position.y, position.z), Vector3(5.f, 5.f, 5.f));
	ArmL->SetCollider(true);
	ArmL->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	ArmL->InitLOD("cubeSG", "cubeSG", "cubeSG");
	ArmLNode = BodyNode->AddChild(ArmL);

	ArmR = Create::Entity("cubeSG", Vector3(position.x + 5.f, position.y, position.z), Vector3(5.f, 5.f, 5.f));
	ArmR->SetCollider(true);
	ArmR->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
	ArmR->InitLOD("cubeSG", "cubeSG", "cubeSG");
	ArmRNode = BodyNode->AddChild(ArmR);

	Health = 100.f;

	// Add to EntityManager
	
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

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

void CEnemy::SetHealth(int _Health)
{
	this->Health = _Health;
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

int CEnemy::GetHealth(void)
{
	return Health;
}

// Update
void CEnemy::Update(double dt)
{
	if (Health <= 0)
	{
		SetIsDone(true);
		for (auto it : BodyNode->getChild())
		{
			it->GetEntity()->SetIsDone(true);
		}
		return;
	}

	Vector3 viewVector = (target - position).Normalized();
	position += viewVector * (float)m_dSpeed * (float)dt;
	//cout << position << "..." << viewVector << endl;

	// Constrain the position
	Constrain();

	// Update the target
	if (position.z > 400.0f)
		target.z = position.z * -1;
	else if (position.z < -400.0f)
		target.z = position.z * -1;

	BodyNode->GetEntity()->SetPosition(Vector3(BodyNode->GetEntity()->GetPosition().x, BodyNode->GetEntity()->GetPosition().y, position.z));

	for (auto it : BodyNode->getChild())
	{
		it->GetEntity()->SetPosition(Vector3(it->GetEntity()->GetPosition().x, it->GetEntity()->GetPosition().y,position.z));
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

// TODO //
// CHANGE TO CREATE::ENTITY //
// TODO // 