#include "AssaultRifle.h"


CAssaultRifle::CAssaultRifle()
{
}


CAssaultRifle::~CAssaultRifle()
{
}

// Initialise this instance to default values
void CAssaultRifle::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	firingspeed = 500.f;
	// The number of ammunition in a magazine for this weapon
	magRounds = 30;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 30;
	// The current total number of rounds currently carried by this player
	totalRounds = maxMagRounds * 3;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = maxMagRounds * 3;

	// The time between shots
	timeBetweenShots = 0.1;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}