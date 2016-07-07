#include "../stdafx.h"

void TeleporttoSelectedPlayerVehicle(Ped playerPed, Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle selectedVehicle = PED::GET_VEHICLE_PED_IS_USING(selectedPed);
		for (int i = SEAT_BACKPASSENGER; i >= SEAT_DRIVER; i--)
		{
			PED::SET_PED_INTO_VEHICLE(playerPed, selectedVehicle, i);
			DrawNotification("Teleported to player's vehicle!");
		}
	}
}

void TeleporttoSelectedPlayer(Ped playerPed, Ped selectedPed)
{
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, FALSE))
	{
		e = PED::GET_VEHICLE_PED_IS_USING(playerPed);
	}

	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(selectedPed, FALSE);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, playerPosition.x, playerPosition.y, playerPosition.z + 2, FALSE, FALSE, TRUE);
	DrawNotification("Teleported to player!");
}

void KickPlayer(Player player)
{
	std::string name = PLAYER::GET_PLAYER_NAME(player);
	
	static bool hosted = false;
	NETWORK::_0xC19F6C8E7865A6FF(1);
	hosted = NETWORK::NETWORK_IS_HOST();

	if (hosted)
	{
		NETWORK::NETWORK_SESSION_KICK_PLAYER(player);
		DrawNotification("Player " + name + " was kicked by you!");
	}
	else
	{
		NETWORK::NETWORK_SESSION_KICK_PLAYER(player);
		DrawNotification("You are not HOST!");
	}
}

void KillPed(Ped ped)
{
	Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	FIRE::ADD_EXPLOSION(position.x, position.y, position.z, EXPLOSION_BLIMP, 1000.0f, TRUE, FALSE, 1.0f);
	DrawNotification("Player EXPLODED!");
}

void FreezePlayer(Ped ped, bool state)
{
	if (state)
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
	}
	else
	{
		DrawNotification("Player UNFREEZED!");
	}
}

void AttachUFOtoPlayer(Ped selectedPed)
{
	//load junk model
	/* Sample objects:
	prop_bskball_01=1840863642  		0xF77CB21C
	prop_cs_bin_02=651101403			0x0F3F3CB0
	prop_defilied_ragdoll_01=-332567508	0x7A2A3826
	prop_bball_arcade_01				0xA50DDDD0
	Garbage can							651101403
	Basketball							1840863642
	*/
	Hash objectModel = 0xB467C540;
	if (!STREAMING::HAS_MODEL_LOADED(objectModel))
	{
		STREAMING::REQUEST_MODEL(objectModel);
	}
	if (STREAMING::HAS_MODEL_LOADED(objectModel) == TRUE)
	{
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(selectedPed, FALSE);
		Object junkObject = OBJECT::CREATE_OBJECT(objectModel, playerPosition.x, playerPosition.y, playerPosition.z, 1, 1, 0);
		OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(junkObject);
		if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
		{
			AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
		}
		RequestControl(junkObject);
		ENTITY::ATTACH_ENTITY_TO_ENTITY(junkObject, selectedPed, PED::GET_PED_BONE_INDEX(selectedPed, SKEL_ROOT),
			0.00f,	//floatx
			0.00f,	//floaty
			1.0f,	//floatz
			0.0f,	//xrot
			180.0f,	//yrot 
			0.0f,	//zrot
			false, false, false, false, 2, true);
			//STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(objectModel);
		DrawNotification("Attaching UFO to Player!");
	}
}

void ForceKill(Ped ped, bool state)
{
	if (state)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
		FIRE::ADD_EXPLOSION(position.x, position.y, position.z, EXPLOSION_BLIMP, 10.0f, TRUE, FALSE, 1.0f);
	}
	else
	{
	}
}

void CrashPlayer(Player player)
{
	const int maxPeds = 250;
	Ped ClonedPed[maxPeds];
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Ped selectedPed = PLAYER::GET_PLAYER_PED(player);
	Vector3 selectedPedPosition = ENTITY::GET_ENTITY_COORDS(selectedPed, FALSE);
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(playerPed, FALSE);

	// check distance so i dont crash myself
	if (GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerPosition.x, playerPosition.y, playerPosition.z, selectedPedPosition.x, selectedPedPosition.y, selectedPedPosition.z, false) > 250.0f)
	{
		DrawNotification("This task will take a minute.");
		//spawn peds
		for (int i = 0; i < maxPeds; i++)
		{
			ClonedPed[i] = PED::CLONE_PED(selectedPed, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1);
			ENTITY::SET_ENTITY_VISIBLE(ClonedPed[i], FALSE, 1);
			ENTITY::SET_ENTITY_ALPHA(ClonedPed[i], 0, false);
			WAIT(30);
			AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);

			if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(player))
			{
				DrawNotification("Player Crashed!");
				break;
			}
		}

		/*if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(player))
		{

			DrawNotification("Clearing Clone Ped!");
			for (int i = 0; i < maxPeds; i++)
			{
				PED::DELETE_PED(&ClonedPed[i]);
			}
			DrawNotification("Crash Area Cleared!");
		}*/

		if (NETWORK::NETWORK_IS_PLAYER_CONNECTED(player))
		{
			DrawNotification("Player not crash YEEEEEEEEEEEEE! Try Again!");
		}
		
	}
	else
	{
		DrawNotification("Too close to the player your trying to crash!");
		DrawNotification("Move farther away!(at least 250M)");
	}
}

void sendArmyAttackers(Ped ped)
{
	Hash tankhash = GAMEPLAY::GET_HASH_KEY("rhino");
	Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, 1);
	STREAMING::REQUEST_MODEL(tankhash); //Hydra = 0x39D6E83F 
	while (!STREAMING::HAS_MODEL_LOADED(tankhash)) WAIT(0);
	Vehicle tank = VEHICLE::CREATE_VEHICLE(tankhash, position.x + 5, position.y + 5, position.z, ENTITY::GET_ENTITY_HEADING(ped), TRUE, TRUE);
	RequestControl(tank);
	ENTITY::SET_ENTITY_VISIBLE(tank, TRUE, 0);
	ENTITY::SET_ENTITY_ALPHA(tank, 255, false);

	Hash marinehash = GAMEPLAY::GET_HASH_KEY("S_M_Y_Marine_03");
	STREAMING::REQUEST_MODEL(marinehash); //Marine = 0xF2DAA2ED
	while (!STREAMING::HAS_MODEL_LOADED(marinehash)) WAIT(0);
	Ped driver = PED::CREATE_PED_INSIDE_VEHICLE(tank, 26, marinehash, -1, TRUE, TRUE);
	RequestControl(driver);
	ENTITY::SET_ENTITY_VISIBLE(driver, TRUE, 0);
	ENTITY::SET_ENTITY_ALPHA(driver, 255, false);

	PED::SET_PED_RELATIONSHIP_GROUP_HASH(driver, GAMEPLAY::GET_HASH_KEY("cop"));
	AI::TASK_COMBAT_PED(driver, ped, 0, 16);
	AI::TASK_VEHICLE_CHASE(driver, ped);
}

void TrapPlayer(Ped ped) //This one wasn't created by me, i think it was made by someone over at NGU.
{
	Vector3 selectedCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	Entity firstPrison = OBJECT::CREATE_OBJECT(-733833763, selectedCoords.x, selectedCoords.y, selectedCoords.z - 2, TRUE, TRUE, FALSE);
	Entity secondPrison = OBJECT::CREATE_OBJECT(-733833763, selectedCoords.x, selectedCoords.y, selectedCoords.z - 2, TRUE, TRUE, FALSE);

	ENTITY::SET_ENTITY_HEADING(secondPrison, ENTITY::GET_ENTITY_HEADING(firstPrison) + 90);
	ENTITY::FREEZE_ENTITY_POSITION(firstPrison, TRUE);
	ENTITY::FREEZE_ENTITY_POSITION(secondPrison, TRUE);

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(firstPrison, TRUE, TRUE);
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(secondPrison, TRUE, TRUE);
}

void AttachRapeClone(Ped selectedPed)
{
	Hash justin = GAMEPLAY::GET_HASH_KEY("U_M_Y_Justin");
	Vector3 selectedOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(selectedPed, 0.0f, 2.0f, 0.0f);
	STREAMING::REQUEST_MODEL(justin);
	while (!STREAMING::HAS_MODEL_LOADED(justin)) WAIT(0);

	STREAMING::REQUEST_ANIM_DICT("rcmpaparazzo_2");
	while (!STREAMING::HAS_ANIM_DICT_LOADED("rcmpaparazzo_2")) WAIT(0);
	Ped pedJustin = PED::CREATE_PED(1, justin, selectedOffset.x, selectedOffset.y, selectedOffset.z, 0, TRUE, TRUE);

	ENTITY::SET_ENTITY_INVINCIBLE(pedJustin, TRUE);
	AI::TASK_PLAY_ANIM(pedJustin, "rcmpaparazzo_2", "shag_loop_a", 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
	ENTITY::ATTACH_ENTITY_TO_ENTITY(pedJustin, selectedPed, 0x5c01, 0, -0.28, 0, 1, 0, 0, FALSE, FALSE, FALSE, FALSE, 2, TRUE);
}

void specmode(Ped ped)
{
	if (!NETWORK::NETWORK_IS_IN_SPECTATOR_MODE())
	{
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, ped);
	}
	else
	{
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, ped);
	}
}

void hijack(Ped playerPed, Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(selectedPed, FALSE);
		RequestControl(vehicle);
		PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, 0);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(selectedPed);
		WAIT(1000);
		Vehicle seat = PED::GET_VEHICLE_PED_IS_IN(playerPed, FALSE);
		RequestControl(seat);
		PED::SET_PED_INTO_VEHICLE(playerPed, seat, SEAT_DRIVER);
		DrawNotification("Vehicle Hijacked!");
	}
	else
	{
		DrawNotification("Player isn't in vehicle!");
	}
}

void tpveh(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(selectedPed, FALSE);
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(selectedPed, FALSE);
		RequestControl(Vehicle);
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Vehicle, playerPosition.x, playerPosition.y, playerPosition.z + 500, 0, 0, 1);
		DrawNotification("Player's Vehicle teleported to Sky!");
	}
	else
	{
		DrawNotification("Player isn't in vehicle!");
	}
}

void deletevehicle(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(selectedPed, FALSE);
		RequestControl(Vehicle);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vehicle, TRUE, TRUE);
		VEHICLE::DELETE_VEHICLE(&Vehicle);
		DrawNotification("Player's Vehicle deleted!");
	}
	else
	{
		DrawNotification("Player isn't in vehicle!");
	}
}

void clonevehicle(Ped selectedPed)
{
	if (PED::IS_PED_IN_ANY_VEHICLE(selectedPed, FALSE))
	{
		Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);
		Vehicle pedVeh = PED::GET_VEHICLE_PED_IS_IN(selectedPed, TRUE);
		Hash vehicleModelHash = ENTITY::GET_ENTITY_MODEL(pedVeh);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModelHash))
		{
			STREAMING::REQUEST_MODEL(vehicleModelHash); //This should already be loaded since we're stealing it from someone in memory.
			Vehicle clonedVeh = VEHICLE::CREATE_VEHICLE(vehicleModelHash, playerPosition.x + 3, playerPosition.y + 3, playerPosition.z + 3, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), TRUE, TRUE);
			VEHICLE::SET_VEHICLE_MOD_KIT(clonedVeh, 0);
			int primaryColor, secondaryColor;
			VEHICLE::GET_VEHICLE_COLOURS(pedVeh, &primaryColor, &secondaryColor);
			VEHICLE::SET_VEHICLE_COLOURS(clonedVeh, primaryColor, secondaryColor);
			VEHICLE::SET_VEHICLE_WHEEL_TYPE(clonedVeh, VEHICLE::GET_VEHICLE_WHEEL_TYPE(pedVeh));
			for (int i = 0; i <= MOD_BACKWHEELS; i++)
			{
				if (i > MOD_ARMOR && i < MOD_FRONTWHEELS)
					VEHICLE::TOGGLE_VEHICLE_MOD(clonedVeh, i, VEHICLE::IS_TOGGLE_MOD_ON(pedVeh, i));
				else
					VEHICLE::SET_VEHICLE_MOD(clonedVeh, i, VEHICLE::GET_VEHICLE_MOD(pedVeh, i), VEHICLE::GET_VEHICLE_MOD_VARIATION(pedVeh, i));
			}
			int tireSmokeColor[3], pearlescentColor, wheelColor;
			VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(pedVeh, &tireSmokeColor[0], &tireSmokeColor[1], &tireSmokeColor[2]);
			VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(clonedVeh, tireSmokeColor[0], tireSmokeColor[1], tireSmokeColor[2]);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(clonedVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(pedVeh));
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(clonedVeh, VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(pedVeh));
			VEHICLE::GET_VEHICLE_EXTRA_COLOURS(pedVeh, &pearlescentColor, &wheelColor);
			VEHICLE::SET_VEHICLE_EXTRA_COLOURS(clonedVeh, pearlescentColor, wheelColor);
			if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(pedVeh, 0))
			{
				int convertableState = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(pedVeh);
				if (convertableState == 0 || convertableState == 3 || convertableState == 5)
					VEHICLE::RAISE_CONVERTIBLE_ROOF(clonedVeh, 1);
				else
					VEHICLE::LOWER_CONVERTIBLE_ROOF(clonedVeh, 1);
			}
			for (int i = 0; i <= NEON_BACK; i++)
			{
				VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(clonedVeh, i, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(pedVeh, i));
			}
			for (int i = 0; i <= 11; i++)
			{
				if (VEHICLE::DOES_EXTRA_EXIST(pedVeh, i))
					VEHICLE::SET_VEHICLE_EXTRA(clonedVeh, i, !VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(pedVeh, i));
			}
			if ((VEHICLE::GET_VEHICLE_LIVERY_COUNT(pedVeh) > 1) && VEHICLE::GET_VEHICLE_LIVERY(pedVeh) >= 0)
			{
				VEHICLE::SET_VEHICLE_LIVERY(clonedVeh, VEHICLE::GET_VEHICLE_LIVERY(pedVeh));
			}
			int neonColor[3];
			VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(pedVeh, &neonColor[0], &neonColor[1], &neonColor[2]);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(clonedVeh, neonColor[0], neonColor[1], neonColor[2]);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(clonedVeh, VEHICLE::GET_VEHICLE_WINDOW_TINT(pedVeh));
			VEHICLE::SET_VEHICLE_LIGHTS(clonedVeh, true);
			VEHICLE::SET_VEHICLE_ALARM(clonedVeh, true);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicleModelHash);
			BoostBaseVehicleStats(clonedVeh, false);
			VEHICLE::SET_VEHICLE_COLOURS(clonedVeh, primaryColor, secondaryColor);
		}
		else
		{
			DrawNotification("Clone Vehicle Failed!");
		}
	}
	else
	{
		DrawNotification("Player isn't in vehicle!");
	}
}

//All Player Options

void KillAllPed()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
		Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
		Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
		if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
		{
			FIRE::ADD_EXPLOSION(position.x, position.y, position.z, EXPLOSION_BLIMP, 5.0f, TRUE, FALSE, 1.0f);
		}
	}
	DrawNotification("All Player EXPLODED!");
}

void KillAllPed2(bool state)
{
	if (state)
	{
		for (int index = 0; index < 32; index++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
			Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
			Vector3 position = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
			if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
			{
				FIRE::ADD_EXPLOSION(position.x, position.y, position.z, EXPLOSION_BLIMP, 5.0f, TRUE, FALSE, 1.0f);
			}
		}
	}
	else
	{
	}
}

void FreezeAllPlayer(bool state)
{
	if (state)
	{
		for (int index = 0; index < 32; index++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
			if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
			{
				AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
			}
		}
	}
	else
	{
		DrawNotification("Player UNFREEZED!");
	}
}

void AttachUFOtoAllPlayer()
{
	//load junk model
	/* Sample objects:
	prop_bskball_01=1840863642  		0xF77CB21C
	prop_cs_bin_02=651101403			0x0F3F3CB0
	prop_defilied_ragdoll_01=-332567508	0x7A2A3826
	prop_bball_arcade_01				0xA50DDDD0
	Garbage can							651101403
	Basketball							1840863642
	*/
	Hash objectModel = 0xB467C540;
	if (!STREAMING::HAS_MODEL_LOADED(objectModel))
	{
		STREAMING::REQUEST_MODEL(objectModel);
	}
	if (STREAMING::HAS_MODEL_LOADED(objectModel) == TRUE)
	{
		for (int index = 0; index < 32; index++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
			if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
			{
				Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
				Object junkObject = OBJECT::CREATE_OBJECT(objectModel, playerPosition.x, playerPosition.y, playerPosition.z, 1, 1, 0);
				OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(junkObject);
				if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
				{
					AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);
				}
				RequestControl(junkObject);
				ENTITY::ATTACH_ENTITY_TO_ENTITY(junkObject, ped, PED::GET_PED_BONE_INDEX(ped, SKEL_ROOT),
					0.00f,	//floatx
					0.00f,	//floaty
					1.0f,	//floatz
					0.0f,	//xrot
					180.0f,	//yrot 
					0.0f,	//zrot
					false, false, false, false, 2, true);
				//STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(objectModel);
				DrawNotification("Attaching UFO to Player!");
			}
		}
	}
}

void AttachRapeCloneToAll()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
		if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
		{
			Hash justin = GAMEPLAY::GET_HASH_KEY("U_M_Y_Justin");
			Vector3 selectedOffset = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 2.0f, 0.0f);
			STREAMING::REQUEST_MODEL(justin);
			while (!STREAMING::HAS_MODEL_LOADED(justin)) WAIT(0);

			STREAMING::REQUEST_ANIM_DICT("rcmpaparazzo_2");
			while (!STREAMING::HAS_ANIM_DICT_LOADED("rcmpaparazzo_2")) WAIT(0);
			Ped pedJustin = PED::CREATE_PED(1, justin, selectedOffset.x, selectedOffset.y, selectedOffset.z, 0, TRUE, TRUE);

			ENTITY::SET_ENTITY_INVINCIBLE(pedJustin, TRUE);
			AI::TASK_PLAY_ANIM(pedJustin, "rcmpaparazzo_2", "shag_loop_a", 8.0f, 0.0f, -1, 9, 0, 0, 0, 0);
			ENTITY::ATTACH_ENTITY_TO_ENTITY(pedJustin, ped, 0x5c01, 0, -0.28, 0, 1, 0, 0, FALSE, FALSE, FALSE, FALSE, 2, TRUE);
		}
	}
}

void GiveAllWeaponsToAll()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
		if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
		{
			const static Hash weaponList[] = { WEAPON_APPISTOL, WEAPON_ASSAULTSHOTGUN, WEAPON_BAT, WEAPON_COMBATMG,
			WEAPON_COMBATPDW, WEAPON_CROWBAR, WEAPON_FIREWORK,
			WEAPON_FLAREGUN, WEAPON_GOLFCLUB, WEAPON_GRENADE, WEAPON_GRENADELAUNCHER,
			WEAPON_GUSENBERG, WEAPON_HEAVYSHOTGUN, WEAPON_HEAVYSNIPER, WEAPON_HOMINGLAUNCHER,
			WEAPON_MARKSMANRIFLE, WEAPON_MG, WEAPON_MICROSMG, WEAPON_MOLOTOV, WEAPON_MUSKET,
			WEAPON_PETROLCAN, WEAPON_PISTOL50, WEAPON_PROXMINE, WEAPON_RPG,
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
		WAIT(100);
	}
}

void RemoveAllPlayerWeapons()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
		if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
		{
			WEAPON::REMOVE_ALL_PED_WEAPONS(ped, TRUE);
		}
	}
}

void DumpAllPlayerFromVehicle(bool notification)
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
		if (bPlayerOnlineExists && index != PLAYER::PLAYER_ID())
		{
			if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
			{
				//Remove PED from vehicle
				AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);

				//need to remove the parachute: 0xFBAB5776
				WEAPON::REMOVE_WEAPON_FROM_PED(ped, 0xFBAB5776);

			}
			if (notification)
			{
				DrawNotification("All Player has been kicked from vehicle!");
			}
		}
	}
}

void tpallveh()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE) && index != PLAYER::PLAYER_ID())
		{
			Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
			Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
			RequestControl(Vehicle);
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(Vehicle, playerPosition.x, playerPosition.y, playerPosition.z + 500, 0, 0, 1);
			DrawNotification("All Player's Vehicle teleported to Sky!");
		}
	}
}

playerinfo playerdb[32];
bool blipFound[32];

void PlayerBlip(bool state)
{
	if (state)
	{
		for (int index = 0; index < 32; index++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
			if (!blipFound[index] && bPlayerOnlineExists)
			{
				playerdb[index].blip = UI::ADD_BLIP_FOR_ENTITY(ped);
				UI::SET_BLIP_COLOUR(playerdb[index].blip, 0);
				UI::SET_BLIP_SCALE(playerdb[index].blip, 0.8f);
				UI::SET_BLIP_NAME_TO_PLAYER_NAME(playerdb[index].blip, index);
				UI::SET_BLIP_CATEGORY(playerdb[index].blip, 7);
				UI::SET_BLIP_HIGH_DETAIL(playerdb[index].blip, 1);
				blipFound[index] = true;
			}
			if(UI::DOES_BLIP_EXIST(playerdb[index].blip) && !bPlayerOnlineExists && ped != PLAYER::PLAYER_PED_ID() && !PED::IS_PED_A_PLAYER(ped))
			{
				UI::REMOVE_BLIP(&playerdb[index].blip);
				blipFound[index] = false;
			}
		}
	}
	else
	{
		blipFound[32] = false;
	}
}

void deleteallvehicle()
{
	for (int index = 0; index < 32; index++)
	{
		Ped ped = PLAYER::GET_PLAYER_PED(index);
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
		{
			Vehicle Vehicle = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
			RequestControl(Vehicle);
			ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vehicle, TRUE, TRUE);
			VEHICLE::DELETE_VEHICLE(&Vehicle);
			DrawNotification("All Player's Vehicle deleted!");
		}
	}
}

void removeallweapon(bool state)
{
	if (state)
	{
		for (int index = 0; index < 32; index++)
		{
			Ped ped = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(ped);
			if (bPlayerOnlineExists)
			{
				WEAPON::REMOVE_ALL_PED_WEAPONS(ped, TRUE);
			}
		}
	}
	else
	{
	}
}