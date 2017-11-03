#include "LaserBlaster.h"


CLaserBlaster::CLaserBlaster()
{
}


CLaserBlaster::~CLaserBlaster()
{
}

// Initialise this instance to default values
void CLaserBlaster::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	firingspeed = 500.f;
	// The number of ammunition in a magazine for this weapon
	magRounds = 5;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 5;
	// The current total number of rounds currently carried by this player
	totalRounds = maxMagRounds * 2;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = maxMagRounds * 2;

	// The time between shots
	timeBetweenShots = 1.f;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}