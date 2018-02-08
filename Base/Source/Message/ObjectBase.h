#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include <string>
#include "ConcreteMessages.h"

class ObjectBase
{
public:
	virtual ~ObjectBase() {}
	virtual bool Handle(Message *message) = 0;
};

#endif