#pragma once

#include <vector>

#include "Vector3.h"

#include "EntityBase.h"
#include "Transform.h"

class CSceneNode : public CTransform
{
public:
	CSceneNode();
	virtual ~CSceneNode();

	// Release all memory for this node and its children
	void Destroy(void);

	// Set the ID for this node
	void SetID(const int ID);
	// Get the ID for this node
	int GetID(void) const;

	// Set the entity for this node
	bool SetEntity(EntityBase* theEntity);
	// Get the entity for this node
	EntityBase* GetEntity(void) const;

	//////////////////////////PARENT//////////////////////////
	// Set a parent to this node
	void SetParent(CSceneNode* theParent);
	// Get parent to this nod
	CSceneNode* GetParent(void) const;

	//////////////////////////CHILD//////////////////////////
	// Add a child to this node
	CSceneNode* AddChild(EntityBase* theEntity = NULL);
	// Delete a child from this node using the pointer to the entity
	bool DeleteChild(EntityBase* theEntity = NULL);
	// Delete a child from this node using its ID
	bool DeleteChild(const int ID);
	// Delete all children from this node using its ID
	bool DeleteAllChildren(void);
	// Detach a child from this node using the pointer to the node
	CSceneNode* DetachChild(EntityBase* theEntity = NULL);
	// Detach a child from this node using its ID
	CSceneNode* DetachChild(const int ID);
	// Get the entity inside this Scene Graph
	CSceneNode* GetEntity(EntityBase* theEntity);
	// Get the entity inside this Scene Graph using its ID
	CSceneNode* GetEntity(const int ID);
	int GetNumOfChild(void);

	// PrintSelf for debug purposes
	void PrintSelf(const int numTabs = 0);

	void Update(void);
	void Render(void);
protected:
	int ID;
	EntityBase* theEntity;
	CSceneNode* theParent;

	std::vector<CSceneNode*> theChildren;
};