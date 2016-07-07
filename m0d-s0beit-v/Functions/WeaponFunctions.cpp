#include "../stdafx.h"

void BruteForceWeaponAddons(Ped ped, Hash weaponHash, bool bSilencer)
{
	//Since only I get these anyway, might as well craft it for me.
	const static Hash weaponAddons[] = { COMPONENT_AT_SCOPE_MACRO, COMPONENT_AT_SCOPE_MACRO_02, COMPONENT_AT_SCOPE_SMALL, COMPONENT_AT_SCOPE_SMALL_02, COMPONENT_AT_SCOPE_MEDIUM, COMPONENT_AT_SCOPE_LARGE, COMPONENT_AT_SCOPE_MAX, COMPONENT_AT_RAILCOVER_01, COMPONENT_AT_AR_AFGRIP, COMPONENT_AT_PI_FLSH, COMPONENT_AT_AR_FLSH, COMPONENT_PISTOL_CLIP_02, COMPONENT_COMBATPISTOL_CLIP_02, COMPONENT_APPISTOL_CLIP_02, COMPONENT_MICROSMG_CLIP_02, COMPONENT_SMG_CLIP_02, COMPONENT_ASSAULTRIFLE_CLIP_02, COMPONENT_CARBINERIFLE_CLIP_02, COMPONENT_ADVANCEDRIFLE_CLIP_02, COMPONENT_MG_CLIP_02, COMPONENT_COMBATMG_CLIP_02, COMPONENT_ASSAULTSHOTGUN_CLIP_02, COMPONENT_PISTOL50_CLIP_02, COMPONENT_ASSAULTSMG_CLIP_02, COMPONENT_SNSPISTOL_CLIP_02, COMPONENT_COMBATPDW_CLIP_02, COMPONENT_HEAVYPISTOL_CLIP_02, COMPONENT_SPECIALCARBINE_CLIP_02, COMPONENT_BULLPUPRIFLE_CLIP_02, COMPONENT_VINTAGEPISTOL_CLIP_02, COMPONENT_MARKSMANRIFLE_CLIP_02, COMPONENT_HEAVYSHOTGUN_CLIP_02, COMPONENT_GUSENBERG_CLIP_02 };

	for each (Hash addonHash in weaponAddons)
	{
		if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weaponHash, addonHash)) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, addonHash);
	}
	if (bSilencer)
	{
		static Hash silencers[] = { COMPONENT_AT_PI_SUPP, COMPONENT_AT_PI_SUPP_02, COMPONENT_AT_AR_SUPP, COMPONENT_AT_SR_SUPP, COMPONENT_AT_AR_SUPP_02 };
		//static Hash silencers[] = { COMPONENT_AT_PI_SUPP_02, COMPONENT_AT_AR_SUPP };
		for each (Hash silencerHash in silencers)
		{
			if (WEAPON::DOES_WEAPON_TAKE_WEAPON_COMPONENT(weaponHash, silencerHash))
			{
				if (weaponHash != WEAPON_ADVANCEDRIFLE && WEAPON::GET_WEAPONTYPE_GROUP(weaponHash) != WEAPON_TYPE_GROUP_SHOTGUN) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, silencerHash);
			}
		}
		if (weaponHash == WEAPON_SNIPERRIFLE || weaponHash == WEAPON_MICROSMG) WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(ped, weaponHash, COMPONENT_AT_AR_SUPP_02);
	}
}

void GiveAllWeapons(Ped ped)
{
	const static Hash weaponList[] = { WEAPON_APPISTOL, WEAPON_ASSAULTSHOTGUN, WEAPON_BAT, WEAPON_COMBATMG,
		WEAPON_COMBATPDW, WEAPON_CROWBAR, WEAPON_FIREWORK,
		WEAPON_FLAREGUN, WEAPON_GOLFCLUB, WEAPON_GRENADE, WEAPON_GRENADELAUNCHER,
		WEAPON_GUSENBERG, WEAPON_HEAVYSHOTGUN, WEAPON_HEAVYSNIPER, WEAPON_HOMINGLAUNCHER,
		WEAPON_MARKSMANRIFLE, WEAPON_MG, WEAPON_MICROSMG, WEAPON_MOLOTOV, WEAPON_MUSKET,
		WEAPON_PETROLCAN, WEAPON_PISTOL, WEAPON_PISTOL50, WEAPON_PROXMINE, WEAPON_RPG,
		WEAPON_SMOKEGRENADE, WEAPON_SNIPERRIFLE, WEAPON_STICKYBOMB,
		WEAPON_STUNGUN, WEAPON_MINIGUN, WEAPON_BZGAS, WEAPON_RAILGUN, WEAPON_FLARE,
		WEAPON_MARKSMANPISTOL, WEAPON_REMOTESNIPER, WEAPON_FIREEXTINGUISHER, WEAPON_FLASHLIGHT, WEAPON_MACHETE,
		WEAPON_MACHINEPISTOL, WEAPON_CARBINERIFLE, WEAPON_SNOWBALL, WEAPON_KNUCKLE, WEAPON_SPECIALCARBINE };

	for each (Hash weapon in weaponList)
	{
		if (WEAPON::HAS_PED_GOT_WEAPON(ped, weapon, FALSE) == FALSE)
		{
			//WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, WEAPON::GET_MAX_AMMO(ped, weapon, &maxAmmo) == TRUE ? maxAmmo : 9999, FALSE, TRUE);
			WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, 9999, FALSE, TRUE);
			BruteForceWeaponAddons(ped, weapon, true); //This doesn't work for people who are not the player running the commands. You can take their weapons, but if you try to add attachments? FUCK YOU! I AIIIIIIIINNN'T HAVIN' THAT SHIT!
			WEAPON::SET_PED_WEAPON_TINT_INDEX(ped, weapon, WEAPONTINT_NORMAL);
		}
		else
		{
			WEAPON::SET_PED_AMMO(ped, weapon, 9999);
		}
		if (WEAPON::GET_WEAPONTYPE_GROUP(weapon) == WEAPON_TYPE_GROUP_THROWABLE)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(ped, weapon);
			//WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, WEAPON::GET_MAX_AMMO(ped, weapon, &maxAmmo) == TRUE ? maxAmmo : 9999, FALSE, TRUE);
			WEAPON::GIVE_WEAPON_TO_PED(ped, weapon, 9999, FALSE, TRUE);
		}
	}
}

void RemoveAllWeapons(Ped ped)
{
	WEAPON::REMOVE_ALL_PED_WEAPONS(ped, TRUE);
}

/*void ReplenishAmmo(Ped playerPed)
{
	Hash currentWeapon;
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &currentWeapon, 1))
	{
		if (WEAPON::IS_WEAPON_VALID(currentWeapon))
		{
			int maxAmmo;
			if (WEAPON::GET_MAX_AMMO(playerPed, currentWeapon, &maxAmmo))
			{
				WEAPON::SET_PED_AMMO(playerPed, currentWeapon, maxAmmo);
			}
		}
	}
}*/

void InfiniteAmmo()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (ENTITY::DOES_ENTITY_EXIST(playerPed))
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo))
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, maxAmmo);
					maxAmmo = WEAPON::GET_MAX_AMMO_IN_CLIP(playerPed, cur, 1);
					if (maxAmmo > 0)
						WEAPON::SET_AMMO_IN_CLIP(playerPed, cur, maxAmmo);
				}
			}
		}
	}
}

void FullAmmo()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	if (ENTITY::DOES_ENTITY_EXIST(playerPed))
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &cur, 1))
		{
			if (WEAPON::IS_WEAPON_VALID(cur))
			{
				int maxAmmo;
				if (WEAPON::GET_MAX_AMMO(playerPed, cur, &maxAmmo) < 9999)
				{
					WEAPON::SET_PED_AMMO(playerPed, cur, 9999);
				}
			}
		}
	}
}

void OneHitKillEnable(Player PlayerPed, bool state)
{
	if (state)
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PlayerPed, 999999);
	else
		PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PlayerPed, 1);
}

Vector3 rot_to_direction(Vector3* rot)
{
	float radiansZ = rot->z * 0.0174532924f;
	float radiansX = rot->x * 0.0174532924f;
	float num = std::abs((float)std::cos((double)radiansX));
	Vector3 dir;
	dir.x = (float)((double)((float)(-(float)std::sin((double)radiansZ))) * (double)num);
	dir.y = (float)((double)((float)std::cos((double)radiansZ)) * (double)num);
	dir.z = (float)std::sin((double)radiansX);
	return dir;
}

Vector3 add(Vector3* vectorA, Vector3* vectorB)
{
	Vector3 result;
	result.x = vectorA->x;
	result.y = vectorA->y;
	result.z = vectorA->z;

	result.x += vectorB->x;
	result.y += vectorB->y;
	result.z += vectorB->z;

	return result;
}

Vector3 multiply(Vector3* vector, float x)
{
	Vector3 result;
	result.x = vector->x;
	result.y = vector->y;
	result.z = vector->z;

	result.x *= x;
	result.y *= x;
	result.z *= x;

	return result;
}

float get_distance(Vector3* pointA, Vector3* pointB)
{
	float a_x = pointA->x;
	float a_y = pointA->y;
	float a_z = pointA->z;

	float b_x = pointB->x;
	float b_y = pointB->y;
	float b_z = pointB->z;

	double x_ba = (double)(b_x - a_x);
	double y_ba = (double)(b_y - a_y);
	double z_ba = (double)(b_z - a_z);

	double y_2 = y_ba * y_ba;
	double x_2 = x_ba * x_ba;
	double sum_2 = y_2 + x_2;

	return (float)std::sqrt(sum_2 + z_ba);
}

float get_vector_length(Vector3* vector)
{
	double x = (double)vector->x;
	double y = (double)vector->y;
	double z = (double)vector->z;

	return (float)std::sqrt(x * x + y * y + z * z);
}

Vector3 GetCoordsInfrontOfCam(float distance) //GetCoordAimingAhead
{
	Vector3 GameplayCamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	Vector3 GameplayCamCoord = CAM::GET_GAMEPLAY_CAM_COORD();

	float tan = cos(GameplayCamRot.y) * distance;
	float xPlane = (sin(GameplayCamRot.z * -1.0f) * tan) + GameplayCamCoord.y;
	float yPlane = (cos(GameplayCamRot.z * -1.0f) * tan) + GameplayCamCoord.y;
	float zPlane = (sin(GameplayCamRot.y) * distance) + GameplayCamCoord.z;

	Vector3 Output;
	Output.x = xPlane;
	Output.y = yPlane;
	Output.z = zPlane;

	return Output;
}

Vector3 GetCoordsFromCam(float distance)
{
	Vector3 Rot = CAM::GET_GAMEPLAY_CAM_ROT(2);
	static Vector3 Coord = CAM::GET_GAMEPLAY_CAM_COORD();

	Rot.y = distance * cos(Rot.x);
	Coord.x = Coord.x + Rot.y * sin(Rot.z * -1.0f);
	Coord.y = Coord.y + Rot.y * cos(Rot.z * -1.0f);
	Coord.z = Coord.z + distance * sin(Rot.x);

	return Coord;
}

void VectorToFloat(Vector3 unk, float *Out)
{
	Out[0] = unk.x;
	Out[1] = unk.y;
	Out[2] = unk.z;
}

void force(Player player, bool state)
{
	if (state)
	{
		Entity EntityTarget;
		DWORD equippedWeapon;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &equippedWeapon, 1);
		

		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 dir = rot_to_direction(&rot);
		Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		float spawnDistance = get_distance(&camPosition, &playerPosition);
		spawnDistance += 5;
		Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &EntityTarget))
		{

			if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1))
			{
				EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
			}

			RequestControl(EntityTarget);

			if (GetAsyncKeyState(0x45))
			{
				ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
			}
		}
	}
	else
	{
	}
}

void Rapid(Ped ped, bool state)
{
	if (state)
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			Hash cur;
			if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &cur, 1))
			{
				if (WEAPON::IS_WEAPON_VALID(cur))
				{
					WEAPON::SET_PED_INFINITE_AMMO(ped, TRUE, cur);
					WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, TRUE);
				}
			}
		}
	}
	else
	{
		if (ENTITY::DOES_ENTITY_EXIST(ped))
		{
			Hash cur;
			if (WEAPON::GET_CURRENT_PED_WEAPON(ped, &cur, 1))
			{
				if (WEAPON::IS_WEAPON_VALID(cur))
				{
					WEAPON::SET_PED_INFINITE_AMMO(ped, FALSE, cur);
					WEAPON::SET_PED_INFINITE_AMMO_CLIP(ped, FALSE);
				}
			}
		}
	}
}

bool grav_target_locked = false;

void gravity(Player player, bool state)
{
	if (state)
	{
		Entity EntityTarget;
		DWORD equippedWeapon;
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &equippedWeapon, 1);

		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 dir = rot_to_direction(&rot);
		Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, 1);
		float spawnDistance = get_distance(&camPosition, &playerPosition);
		spawnDistance += 20;
		Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

		if (equippedWeapon)
		{
			if (grav_target_locked)
			{
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &EntityTarget))
				{
					Vector3 EntityTargetPos = ENTITY::GET_ENTITY_COORDS(EntityTarget, 0);
					PLAYER::DISABLE_PLAYER_FIRING(player, true);
					if (ENTITY::IS_ENTITY_A_PED(EntityTarget) && PED::IS_PED_IN_ANY_VEHICLE(EntityTarget, 1))
					{
						EntityTarget = PED::GET_VEHICLE_PED_IS_IN(EntityTarget, 0);
					}

					RequestControl(EntityTarget);

					if (ENTITY::IS_ENTITY_A_VEHICLE(EntityTarget))
					{
						ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(player));
					}

					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(EntityTarget, spawnPosition.x, spawnPosition.y, spawnPosition.z, 0, 0, 0);

					if (GetAsyncKeyState(0x45))
					{
						AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Foot_Swish", EntityTarget, "docks_heist_finale_2a_sounds", 0, 0);
						ENTITY::SET_ENTITY_HEADING(EntityTarget, ENTITY::GET_ENTITY_HEADING(player));
						ENTITY::APPLY_FORCE_TO_ENTITY(EntityTarget, 1, dir.x * 10000.0f, dir.y * 10000.0f, dir.z * 10000.0f, 0.0f, 0.0f, 0.0f, 0, 0, 1, 1, 0, 1);
						grav_target_locked = false;
						PLAYER::DISABLE_PLAYER_FIRING(player, false);
					}
				}
			}
			if (!PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &EntityTarget))
			{
				grav_target_locked = true;
				PLAYER::DISABLE_PLAYER_FIRING(player, false);
			}
		}
	}
	else
	{
	}
}

void firework(Ped playerPed, bool state)
{
	if (state)
	{
		Vector3 rot = CAM::GET_GAMEPLAY_CAM_ROT(0);
		Vector3 dir = rot_to_direction(&rot);
		Vector3 camPosition = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 1);
		float spawnDistance = get_distance(&camPosition, &playerPosition);
		spawnDistance += 0.25;
		Vector3 spawnPosition = add(&camPosition, &multiply(&dir, spawnDistance));

		float endDistance = get_distance(&camPosition, &playerPosition);
		endDistance += 1000;
		Vector3 endPosition = add(&camPosition, &multiply(&dir, endDistance));

		if (PED::IS_PED_ON_FOOT(playerPed) && PED::IS_PED_SHOOTING(playerPed))
		{
			Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_FIREWORK");
			if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
			{
				WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
				while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
					WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(spawnPosition.x, spawnPosition.y, spawnPosition.z, endPosition.x, endPosition.y, endPosition.z, 250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		}
	}
	else
	{
	}
}