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

	firingspeed = 100.f;
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

// Discharge this weapon
void CLaserBlaster::Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CLaser* aLaser = Create::Laser(_meshName,
				position,
				(target - position).Normalized(),
				10.f,
				2.0f,
				bulletSpeed,
				_source);
			aLaser->SetCollider(true);
			aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;

			bullets.push_back(aLaser);
		}
	}
}
