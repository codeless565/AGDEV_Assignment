#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include "../SceneNode.h"

class Mesh;

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	int Health;

public:
	void UpdateHealth(int hp)
	{
		Health += hp;
	}

	CEnemy(void);
	virtual ~CEnemy();

	void Init(void);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);
	// Set the Health 
	void SetHealth(int _Health);

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);
	// Get Health
	int GetHealth(void);
	
	GenericEntity* GetHead() { return Head; }

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);

private:
	GenericEntity* Body;
	GenericEntity* Head;
	GenericEntity* ArmL;
	GenericEntity* ArmR;

	CSceneNode* HeadNode;
	CSceneNode* BodyNode;
	CSceneNode* ArmLNode;
	CSceneNode* ArmRNode;
};
