#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(COLOR _color)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(0.0f, 0.0f, 0.0f); //<<<<<NEEd to change to distinctive position
	scale.Set(5.f, 5.f, 5.f);
	target.Set(10.0f, 0.0f, 450.0f); // <<<<< Masterblock no need target
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 1.0;

	// Initialise the Collider
	this->SetCollider(true);
	this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);

	//=================== LOD ==========================
	Master_Color = _color;

	InitLOD("MS_High", "MS_Med", "MS_Low"); // TODO LOD

	//Create a Graph for these Blocks
	this->setEntityType(EntityBase::ENTITY_ENEMIES);
	MasterNode = CSceneGraph::GetInstance()->AddNode(this);

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
	m_SpawnTimer += (float)dt;

	if (m_SpawnTimer >= 0.5f)
	{
		m_SpawnTimer = 0.f;

		bool Spawned = false;

		vector<CSceneNode*> ChildBlocks = MasterNode->GetChildren();

		std::vector<CSceneNode*>::iterator it, end;
		end = ChildBlocks.end();

		do
		{
			float RandPosX = Math::RandIntMinMax(-10, 10) * 2 + position.x;
			float RandPosZ = Math::RandIntMinMax(-10, 10) * 2 + position.z;

			Vector3 TempPos(RandPosX, position.y, RandPosZ);

			if (RandPosX == position.x && RandPosZ == position.z)
				continue;

			if (ChildBlocks.empty())
			{
				GenericEntity* childBlock = Create::Entity("cubeSG", TempPos);
				childBlock->InitLOD("cube", "sphere", "cubeSG");
				childBlock->SetCollider(true);
				childBlock->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
				MasterNode->AddChild(childBlock);
				Spawned = true;
			}
			else
			{

				for (auto it : ChildBlocks)
				{
					Vector3 dist = TempPos - it->GetEntity()->GetPosition();

					if (dist.Length() >= 0)
					{
						GenericEntity* childBlock = Create::Entity("cubeSG", TempPos);
						childBlock->InitLOD("cube", "sphere", "cubeSG");
						childBlock->SetCollider(true);
						childBlock->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
						MasterNode->AddChild(childBlock);
						Spawned = true;
						break;
					}
				}
			}
		} while (Spawned == false);
	}

	//Vector3 viewVector = (target - position).Normalized();
	//position += viewVector * (float)m_dSpeed * (float)dt;

	// Constrain the position
	//Constrain();

	//// Update the target - NO NEED TO MOVE
	//if (position.z > 400.0f)
	//	target.z = position.z * -1;
	//else if (position.z < -400.0f)
	//	target.z = position.z * -1;
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

void CEnemy::SpawnChild()
{
	vector<CSceneNode*> ChildBlocks = MasterNode->GetChildren();

	std::vector<CSceneNode*>::iterator it, end;
	end = ChildBlocks.end();

	do
	{
		float RandPosX = Math::RandIntMinMax(-5, 5) * scale.x * 2 + position.x;
		float RandPosY = Math::RandIntMinMax(2, 5) * scale.x * 2 + position.z;

		Vector3 TempPos(RandPosX, RandPosY, position.z);

		if (RandPosX == position.x && RandPosY == position.y)
			continue;

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
			case COLOR_YELLOW:
				childBlock->InitLOD("CSYellow_High", "CSYellow_Med", "CSYellow_Low");
				break;
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

				if (dist.Length() >= 0)
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
					case COLOR_YELLOW:
						childBlock->InitLOD("CSYellow_High", "CSYellow_Med", "CSYellow_Low");
						break;
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


		SpawnChild();