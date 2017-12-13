#pragma once
#include "WeaponInfo.h"

class CGrenadeThrow : public CWeaponInfo
{
public:
	CGrenadeThrow();
	virtual ~CGrenadeThrow();
	// Initialise this instance to default values
	void Init(void);
	// Discharge this weapon
	void CGrenadeThrow::Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source);
	void ResetWeapon();
};