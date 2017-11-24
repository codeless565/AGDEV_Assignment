#include "SceneNode.h"
#include "EntityManager.h"
#include <algorithm>

#include "GraphicsManager.h"
#include "MatrixStack.h"
#include "SceneGraph.h"
using namespace std;

CSceneNode::CSceneNode(void)
	: ID(-1)
	, theEntity(NULL)
	, theParent(NULL)
{
}

CSceneNode::~CSceneNode()
{
}

void CSceneNode::Destroy(void)
{
	// Destroy the children first
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Destroy();
		delete *it;
		theChildren.erase(it);
	}

	if (theEntity)
	{
		theEntity = NULL;
		delete theEntity;
	}
	if (theParent)
	{
		theParent = NULL;
		delete theParent;
	}
}

void CSceneNode::SetID(const int ID)
{
	this->ID = ID;
}

int CSceneNode::GetID(void) const
{
	return ID;
}

bool CSceneNode::SetEntity(EntityBase * theEntity)
{
	if (theEntity)
	{
		this->theEntity = theEntity;
		return true;
	}
	return false;
}

EntityBase * CSceneNode::GetEntity(void) const
{
	return theEntity;
}

void CSceneNode::SetParent(CSceneNode * theParent)
{
	this->theParent = theParent;
}

CSceneNode * CSceneNode::GetParent(void) const
{
	return theParent;
}

CSceneNode * CSceneNode::AddChild(EntityBase * theEntity)
{
	if (theEntity)
	{
		// Create a new Scene Node
		CSceneNode* aNewNode = new CSceneNode();
		// Set the entity to this new scene node
		aNewNode->SetEntity(theEntity);
		// Store the pointer to the parent
		aNewNode->SetParent(this);
		// Assign an ID to this node
		aNewNode->SetID(CSceneGraph::GetInstance()->GenerateID());
		// Add to vector list
		this->theChildren.push_back(aNewNode);
		// Return this new scene Nod
		return aNewNode;
	}

	return NULL;
}

bool CSceneNode::DeleteChild(EntityBase * theEntity)
{
	// if this node contains theEntity, then we proceed to delete all its children
	if (this->theEntity == theEntity)
	{
		// If this node has children, then we proceed to delete them.
		if (theChildren.size() != 0)
		{
			// Iterate through all the children and delete all of their children and grand children etc
			vector <CSceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "CSceneNode::DeleteChild: Deleted child nodes for theEntity." << endl;
				}
				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true; // return true to say that this Node contains theEntity
	}
	else
	{
		// Search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			std::vector<CSceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				// Check if this child is the one containing theEntity
				if ((*it)->DeleteChild(theEntity))
				{
					// If DeleteChild method call above DID remove theEntity
					// Then we should proceed to removed this child from our vector of children
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					theChildren.erase(it);
					break; // Stop deleting since we have already found and deleted theEntity
				}
			}
		}
	}
	return false;
}

bool CSceneNode::DeleteChild(const int ID)
{
	// if this node contains theEntity, then we proceed to delete all its children
	if (this->ID == ID)
	{
		// If this node has children, then we proceed to delete them.
		if (theChildren.size() != 0)
		{
			// Iterate through all the children and delete all of their children and grand children etc
			vector <CSceneNode*>::iterator it = theChildren.begin();
			while (it != theChildren.end())
			{
				if ((*it)->DeleteAllChildren())
				{
					cout << "CSceneNode::DeleteChild: Deleted child nodes for ID=" << ID << endl;
				}
				(*it)->GetEntity()->SetIsDone(true);
				delete *it;
				it = theChildren.erase(it);
			}
		}
		return true; // return true to say that this Node contains theEntity
	}
	else
	{
		// Search the children for this particular theEntity
		if (theChildren.size() != 0)
		{
			std::vector<CSceneNode*>::iterator it;
			for (it = theChildren.begin(); it != theChildren.end(); ++it)
			{
				// Check if this child is the one containing theEntity
				if ((*it)->DeleteChild(theEntity))
				{
					// If DeleteChild method call above DID remove theEntity
					// Then we should proceed to removed this child from our vector of children
					(*it)->GetEntity()->SetIsDone(true);
					delete *it;
					theChildren.erase(it);
					break; // Stop deleting since we have already found and deleted theEntity
				}
			}
		}
	}
	return false;
}

bool CSceneNode::DeleteAllChildren(void)
{
	bool bResult = false;
	vector <CSceneNode*>::iterator it = theChildren.begin();
	while (it != theChildren.end())
	{
		if ((*it)->DeleteAllChildren())
		{
			cout << "CSceneNode::DeleteChild: Delete child nodes." << endl;
		}
		(*it)->GetEntity()->SetIsDone(true);
		delete *it;
		it = theChildren.erase(it);
		bResult = true;
	}
	return bResult;
}

CSceneNode * CSceneNode::DetachChild(EntityBase * theEntity)
{
	if (theChildren.size() != 0)
	{
		vector <CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			CSceneNode* theNode = (*it)->DetachChild(theEntity);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;
}

CSceneNode * CSceneNode::DetachChild(const int ID)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;
	if (theChildren.size() != 0)
	{
		vector <CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			CSceneNode* theNode = (*it)->DetachChild(ID);
			if (theNode)
			{
				// Remove this node from the children
				theChildren.erase(it);
				return theNode;
			}
			it++;
		}
	}
	return NULL;
}

CSceneNode * CSceneNode::GetEntity(EntityBase * theEntity)
{
	// if it is inside this node, then return this node
	if (this->theEntity == theEntity)
		return this;
	if (theChildren.size() != 0)
	{
		std::vector<CSceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			CSceneNode* theNode = (*it)->GetEntity(theEntity);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;
}

CSceneNode * CSceneNode::GetEntity(const int ID)
{
	// if it is inside this node, then return this node
	if (this->ID == ID)
		return this;
	// Check the children
	if (theChildren.size() != 0)
	{
		std::vector<CSceneNode*>::iterator it;
		for (it = theChildren.begin(); it != theChildren.end(); ++it)
		{
			CSceneNode* theNode = (*it)->GetEntity(ID);
			if (theNode)
			{
				return theNode;
			}
		}
	}
	return NULL;

}

int CSceneNode::GetNumOfChild(void)
{
	// Start with this node's children
	int NumOfChild = theChildren.size();
	// Ask the children to feedback how many children they have
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		NumOfChild += (*it)->GetNumOfChild();
	}
	return NumOfChild;
}

void CSceneNode::PrintSelf(const int numTabs)
{
	if (numTabs == 0)
	{
		cout << "===========================\n Start of theRoot::PrintSelf()" << endl;
		cout << "CSceneNode::Print: ID=" << ID << "/Children=" << theChildren.size() << endl;;
		cout << "Printing out the children:" << endl;
		vector<CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf((numTabs + 1));
			it++;
		}
		cout << "End of theRoot::PrintSelf()\n ============================" << endl;
	}
	else
	{
		for (int i = 0; i < numTabs; i++)
			cout << "\t";
		cout << "CSceneNode::PrintSelf: ID=" << ID << "/theEntity=" << theEntity << "/Children=" << theChildren.size() << endl;
		for (int i = 0; i < numTabs; i++)
			cout << "\t";
		cout << "Printing out the children" << endl;
		vector<CSceneNode*>::iterator it = theChildren.begin();
		while (it != theChildren.end())
		{
			(*it)->PrintSelf((numTabs + 1));
			it++;
		}
	}
}

void CSceneNode::Update(void)
{
	// Update the Transformation between this node and its children
	if (theUpdateTransformation)
	{
		ApplyTransform(GetUpdateTransformation());
	}

	/*
	Mtx44 orig = GetTransform();
	Mtx44 update = GetUpdateTransform();
	cout << "======================================================================" << endl;
	cout << "Orig" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "[\t" << orig.a[0] << "\t" << orig.a[4] << "\t" << orig.a[8] << "\t" << orig.a[12] << "\t]" << endl;
	cout << "[\t" << orig.a[1] << "\t" << orig.a[5] << "\t" << orig.a[9] << "\t" << orig.a[13] << "\t]" << endl;
	cout << "[\t" << orig.a[2] << "\t" << orig.a[6] << "\t" << orig.a[10] << "\t" << orig.a[14] << "\t]" << endl;
	cout << "[\t" << orig.a[3] << "\t" << orig.a[7] << "\t" << orig.a[11] << "\t" << orig.a[15] << "\t]" << endl;
	cout << "======================================================================" << endl;

	cout << "======================================================================" << endl;
	cout << "Update" << endl;
	cout << "----------------------------------------------------------------------" << endl;
	cout << "[\t" << update.a[0] << "\t" << update.a[4] << "\t" << update.a[8] << "\t" << update.a[12] << "\t]" << endl;
	cout << "[\t" << update.a[1] << "\t" << update.a[5] << "\t" << update.a[9] << "\t" << update.a[13] << "\t]" << endl;
	cout << "[\t" << update.a[2] << "\t" << update.a[6] << "\t" << update.a[10] << "\t" << update.a[14] << "\t]" << endl;
	cout << "[\t" << update.a[3] << "\t" << update.a[7] << "\t" << update.a[11] << "\t" << update.a[15] << "\t]" << endl;
	cout << "======================================================================" << endl;
	*/

	// Update the children
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Update();
	}
}

void CSceneNode::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();

	if (theEntity)
	{
		if (!theEntity->IsDone())
		{//modelStack.LoadMatrix(this->GetTransform());
			modelStack.MultMatrix(this->GetTransform());

			/*
			Mtx44 Mtx = modelStack.Top();
			cout << "======================================================================" << endl;
			cout << "CSceneNode::PrintSelf" << endl;
			cout << "----------------------------------------------------------------------" << endl;
			cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" << Mtx.a[12] << "\t]" << endl;
			cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" << Mtx.a[13] << "\t]" << endl;
			cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" << Mtx.a[14] << "\t]" << endl;
			cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" << Mtx.a[15] << "\t]" << endl;
			cout << "======================================================================" << endl;
			*/


			// Render the entity
			theEntity->Render();
		}
	}

	// Render the children
	std::vector<CSceneNode*>::iterator it;
	for (it = theChildren.begin(); it != theChildren.end(); ++it)
	{
		(*it)->Render();
	}

	modelStack.PopMatrix();

}
