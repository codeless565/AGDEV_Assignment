#pragma once

#include "Vector3.h"
#include "..//Projectile/Projectile.h"
#include "..//Projectile/Laser.h"
#include <list>

class CPlayerInfo;

class CWeaponInfo
{
public:
	CWeaponInfo();
	virtual ~CWeaponInfo();

	float firingspeed;
	// The number of ammunition in a magazine for this weapon
	int magRounds;
	// The maximum number of ammunition for this magazine for this weapon
	int maxMagRounds;
	// The current total number of rounds currently carried by this player
	int totalRounds;
	// The max total number of rounds currently carried by this player
	int maxTotalRounds;

	// The time between shots in milliseconds
	double timeBetweenShots;
	// The elapsed time (between shots) in milliseconds
	double elapsedTime;
	// Boolean flag to indicate if weapon can fire now
	bool bFire;
	std::list<CProjectile*>bullets;

	// Set the number of ammunition in the magazine for this player
	virtual void SetMagRound(const int magRounds);
	// Set the maximum number of ammunition in the magazine for this weapon
	virtual void SetMaxMagRound(const int magRounds);
	// The current total number of rounds currently carried by this player
	virtual void SetTotalRound(const int totalRounds);
	// The max total number of rounds currently carried by this player
	virtual void SetMaxTotalRound(const int maxTotalRounds);
	virtual void SetFiringSpeed(const float speed);

	// Get the number of ammunition in the magazine for this player
	virtual int GetMagRound(void) const;
	// Get the maximum number of ammunition in the magazine for this weapon
	virtual int GetMaxMagRound(void) const;
	// Get the current total number of rounds currently carried by this player
	virtual int GetTotalRound(void) const;
	// Get the max total number of rounds currently carried by this player
	virtual int GetMaxTotalRound(void) const;
	virtual float GetFiringSpeed(void) const;

	// Set the time between shots
	virtual void SetTimeBetweenShots(const double timeBetweenShots);
	// Set the firing rate in rounds per min
	virtual void SetFiringRate(const int firingRate);
	// Set the firing flag
	virtual void SetCanFire(const bool bFire);

	// Get the time between shots
	virtual double GetTimeBetweenShots(void) const;
	// Get the firing rate
	virtual int GetFiringRate(void) const;
	// Get the firing flag
	virtual bool GetCanFire(void) const;

	// Initialise this instance to default values
	virtual void Init(void);
	// Update the elapsed time
	void Update(const double dt);
	// Discharge this weapon
	virtual void Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source = NULL);
	// Reload this weapon
	void Reload(void);
	// Add rounds
	void AddRounds(const int newRounds);

	// Print Self
	void PrintSelf(void);

	//GetProjectiles
	std::list<CProjectile*> GetProj();
};
