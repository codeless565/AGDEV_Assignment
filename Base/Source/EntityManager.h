#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity);

	bool RemoveEntity(EntityBase* _existingEntity);

private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);

	std::list<EntityBase*> entityList;
	std::list<EntityBase*> list_fixedEntity;
	std::list<EntityBase*> list_NPC;
	std::list<EntityBase*> list_Projectile;
};

#endif // ENTITY_MANAGER_H