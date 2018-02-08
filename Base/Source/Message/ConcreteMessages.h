#ifndef CONCRETE_MESSAGE_H
#define CONCRETE_MESSAGE_H

#include "../Enemy/Enemy.h"

class Message
{
public:
	Message() {}
	virtual ~Message() {}
};

struct MessageWRU : public Message
{
	enum SEARCH_TYPE
	{
		MESSAGE_NONE = 0,

		MESSAGE_CHECKNEARBY,
		MESSAGE_GETNEXTNODE,

		MESSAGE_TOTAL
	};

	MessageWRU(CEnemy *goValue, SEARCH_TYPE typeValue, float thresholdValue = 0) : go(goValue), type(typeValue), threshold(thresholdValue) {}
	virtual ~MessageWRU() {}

	CEnemy *go;
	SEARCH_TYPE type;
	float threshold;
};

#endif
