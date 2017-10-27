#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = list_fixedEntity.end();
	for (it = list_fixedEntity.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	end = list_NPC.end();
	for (it = list_NPC.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	end = list_Projectile.end();
	for (it = list_Projectile.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Check for Collision amongst entities with collider properties
	CheckForCollision();

	// Clean up entities that are done
	it = list_fixedEntity.begin();
	end = list_fixedEntity.end();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = list_fixedEntity.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	it = list_NPC.begin();
	end = list_NPC.end();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = list_NPC.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	it = list_Projectile.begin();
	end = list_Projectile.end();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = list_Projectile.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = list_fixedEntity.end();
	for (it = list_fixedEntity.begin(); it != end; ++it)
	{
		(*it)->Render();
	}

	// Render all entities
	end = list_NPC.end();
	for (it = list_NPC.begin(); it != end; ++it)
	{
		(*it)->Render();
	}

	// Render all entities
	end = list_Projectile.end();
	for (it = list_Projectile.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	switch (_newEntity->getEntityType())
	{
	case EntityBase::ENTITY_FIXED:
		list_fixedEntity.push_back(_newEntity);
		break;
	case EntityBase::ENTITY_NPCS:
		list_NPC.push_back(_newEntity);
		break;
	case EntityBase::ENTITY_PROJECTILES:
		list_Projectile.push_back(_newEntity);
		break;
	default:
		entityList.push_back(_newEntity);
		break;
	}
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter;
	switch (_existingEntity->getEntityType())
	{
	case EntityBase::ENTITY_FIXED:
		findIter = std::find(list_fixedEntity.begin(), list_fixedEntity.end(), _existingEntity);
		// Delete the entity if found
		if (findIter != list_fixedEntity.end())
		{
			delete *findIter;
			findIter = list_fixedEntity.erase(findIter);
			return true;
		}
		break;
	case EntityBase::ENTITY_NPCS:
		findIter = std::find(list_NPC.begin(), list_NPC.end(), _existingEntity);
		// Delete the entity if found
		if (findIter != list_NPC.end())
		{
			delete *findIter;
			findIter = list_NPC.erase(findIter);
			return true;
		}
		break;
	case EntityBase::ENTITY_PROJECTILES:
		findIter = std::find(list_Projectile.begin(), list_Projectile.end(), _existingEntity);
		// Delete the entity if found
		if (findIter != list_Projectile.end())
		{
			delete *findIter;
			findIter = list_Projectile.erase(findIter);
			return true;
		}
		break;
	default: 
		findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);
		// Delete the entity if found
		if (findIter != entityList.end())
		{
			delete *findIter;
			findIter = entityList.erase(findIter);
			return true;
		}
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	// Check if this object is overlapping that object
	if ((thatMinAABB >= thisMinAABB && thatMinAABB <= thisMaxAABB)
		||
		(thatMaxAABB >= thisMinAABB && thatMaxAABB <= thisMaxAABB))
	{
		return true;
	}

	// Check if that object is overlapping this object

	if ((thisMinAABB >= thatMinAABB && thisMinAABB <= thatMaxAABB)
		||
		(thisMaxAABB >= thatMinAABB && thisMaxAABB <= thatMaxAABB))
	{
		return true;
	}

	// Check if this object is within that object
	if ((thisMinAABB >= thatMinAABB && thisMaxAABB <= thatMaxAABB)
		&&
		(thisMaxAABB >= thatMinAABB && thisMaxAABB <= thatMaxAABB))
		return true;

	// Check if that object is within this object
	if ((thatMinAABB >= thisMinAABB && thatMinAABB <= thisMaxAABB)
		&&
		(thatMaxAABB >= thisMinAABB && thatMaxAABB <= thisMaxAABB))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;
	
	colliderThisEnd = list_Projectile.end();
	for (colliderThis = list_Projectile.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if (!(*colliderThis)->HasCollider())
			continue;

		EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

		colliderThatEnd = list_fixedEntity.end();
		//int counter = 0; // use when collided
		for (colliderThat = list_fixedEntity.begin(); colliderThat != colliderThatEnd; ++colliderThat)
		{
			EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThis);
			if (CheckSphereCollision(thisEntity, thatEntity) == true)
			{
				if (!(*colliderThat)->HasCollider())
					continue;

				std::cout << "Sphere" << std::endl;
				if (CheckAABBCollision(thisEntity, thatEntity) == true)
				{
					std::cout << "AABB" << std::endl;
					thisEntity->SetIsDone(true);
					thatEntity->SetIsDone(true);
				}
			}
		}
	}

	//colliderThisEnd = entityList.end();
	//for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	//{
	//	if ((*colliderThis)->HasCollider())
	//	{
	//		// This object was derived from a CCollider class, then it will have Collision Detection methods
	//		//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
	//		EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

	//		// Check for collision with another collider class
	//		colliderThatEnd = entityList.end();
	//		int counter = 0;
	//		for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
	//		{
	//			if (colliderThat == colliderThis)
	//				continue;

	//			if ((*colliderThat)->HasCollider())
	//			{
	//				// This object was derived from a CCollider class, then it will have Collision Detection methods
	//				EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
	//				if (CheckSphereCollision(thisEntity, thatEntity) == true)
	//				{
	//					if (CheckAABBCollision(thisEntity, thatEntity) == true)
	//					{
	//						thisEntity->SetIsDone(true);
	//						thatEntity->SetIsDone(true);
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	return false;
}
