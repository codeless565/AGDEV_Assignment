#include "GenericEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "PlayerInfo\PlayerInfo.h"

GenericEntity::GenericEntity(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
{
}

GenericEntity::~GenericEntity()
{
}

void GenericEntity::Update(double _dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void GenericEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	
	modelStack.PushMatrix();
	modelStack.Scale(scale.x, scale.y, scale.z);
	//Spawn Model
	if (GetLODStatus())
	{
		if (theDetailLevel != NO_DETAILS)
			RenderHelper::RenderMesh(GetLODMesh());
	}
	else
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();

	//Spawn HP Bar for enemy
	if (getEntityType() == EntityBase::ENTITY_ENEMY)
	{
		CPlayerInfo* player = CPlayerInfo::GetInstance();
		modelStack.PushMatrix();
		modelStack.Translate(0, scale.y * 0.5 + 5.f, 0);
		modelStack.Rotate(Math::RadianToDegree(atan2(player->GetPos().x - position.x, player->GetPos().z - position.z)), 0.f, 1.f, 0.f);
		modelStack.Scale(5, 0.5, 0.5);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("HPBar"));
		modelStack.PopMatrix();
	}

	modelStack.PopMatrix();
}

// Set the maxAABB and minAABB
void GenericEntity::SetAABB(Vector3 maxAABB, Vector3 minAABB)
{
	this->maxAABB = maxAABB;
	this->minAABB = minAABB;
}

GenericEntity* Create::Entity(const std::string& _meshName, const Vector3& _position, const Vector3& _scale, const EntityBase::ENTITY_TYPE _type)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);
	result->setEntityType(_type);

	EntityManager::GetInstance()->AddEntity(result, true);
	return result;
}

GenericEntity* Create::Asset(	const std::string& _meshName,
								const Vector3& _position,
								const Vector3& _scale)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	GenericEntity* result = new GenericEntity(modelMesh);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetCollider(false);

	EntityManager::GetInstance()->AddSPLOD(result);

	return result;
}
