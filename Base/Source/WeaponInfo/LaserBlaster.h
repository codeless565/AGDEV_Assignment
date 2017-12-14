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

	void Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source = NULL);
	void ResetWeapon();
};