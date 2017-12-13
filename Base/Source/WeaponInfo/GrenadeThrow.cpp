#include "GrenadeThrow.h"
#include "../Projectile/Grenade.h"
CGrenadeThrow::CGrenadeThrow()
{
}
CGrenadeThrow::~CGrenadeThrow()
{
}// Initialise this instance to default values
void CGrenadeThrow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();
	firingspeed = 20.f;
	// The number of ammunition in a magazine for this weapon
	magRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	totalRounds = 2;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 2;
	// The time between shots
	timeBetweenShots = 1.0;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
// Discharge this weapon
void CGrenadeThrow::Discharge(const std::string& _meshName, Vector3 position, Vector3 target, float bulletSpeed, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
				// It will last for 3.0 seconds and travel at 500 units per second
				CProjectile* aProjectile = Create::Grenade(_meshName,
					position,
					(target - position).Normalized(),
					7.0f,
					bulletSpeed,
					_source);
			aProjectile->SetCollider(true);

			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = false;
			magRounds--;
		}
	}
}

void CGrenadeThrow::ResetWeapon()
{
	CGrenadeThrow::Init();
}