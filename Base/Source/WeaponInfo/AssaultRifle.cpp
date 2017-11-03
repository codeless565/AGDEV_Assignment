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

// Discharge this weapon
void CAssaultRifle::Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Projectile(_meshName,
				position,
				(target - position).Normalized(),
				2.0f,
				bulletSpeed,
				_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;

			bullets.push_back(aProjectile);
		}
	}
}
