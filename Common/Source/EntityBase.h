#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);

	void SetIsLaser(const bool bLaser);

	bool GetIsLaser(void) const;


	enum ENTITY_TYPE
	{
		ENTITY_NONE = 0,
		ENTITY_FIXED,
		ENTITY_NPCS,
		ENTITY_PROJECTILES,
		ENTITY_ENEMIES,
		TOTAL_ENTITY
	};
	void setEntityType(ENTITY_TYPE _type);

	ENTITY_TYPE getEntityType() { return this->m_entityType; };
protected:
	ENTITY_TYPE m_entityType;

	Vector3 position;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;
	bool bLaser;
};

#endif // ENTITY_BASE_H