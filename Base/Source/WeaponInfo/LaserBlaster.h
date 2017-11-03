#pragma once

#include "WeaponInfo.h"

class CLaserBlaster :
	public CWeaponInfo
{
public:
	CLaserBlaster();
	virtual ~CLaserBlaster();

	// Initialise this instance to default values
	void Init(void);
};