#include "../stdafx.h"

/*void ToggleRadio(bool state) {
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()) && PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		AUDIO::SET_VEHICLE_RADIO_ENABLED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), FALSE), state);
}*/

void VehicleSpeed(Vehicle vehicle)
{
	char text[250];
	double speed = ENTITY::GET_ENTITY_SPEED(vehicle);
	double MetersPerHour = speed * 3600;
	if (vehicle != NULL)
	{
		sprintf_s(text, "MPH: %.02f", MetersPerHour);
		UI::SET_TEXT_FONT(0);
		UI::SET_TEXT_OUTLINE();
		UI::SET_TEXT_SCALE(0.4f, 0.4f);
		UI::SET_TEXT_COLOUR(255, 0, 0, 255);
		UI::SET_TEXT_WRAP(0.0f, 1.0f);
		UI::SET_TEXT_CENTRE(TRUE);
		UI::_SET_TEXT_ENTRY("STRING");
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
		UI::_DRAW_TEXT(0.7f, 0.5f);
	}
}

void FlyingCarLoop(Ped ped, bool state)
{
	if (state)
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
		{
			Vehicle Vehid = PED::GET_VEHICLE_PED_IS_IN(ped, 0);
			ENTITY::SET_ENTITY_INVINCIBLE(Vehid, TRUE);
			ENTITY::SET_ENTITY_PROOFS(Vehid, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
			VEHICLE::SET_VEHICLE_STRONG(Vehid, TRUE);
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehid, FALSE);
			VEHICLE::SET_VEHICLE_CAN_BREAK(Vehid, FALSE);
			VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(Vehid, FALSE);

			if (GetAsyncKeyState(0x57)) //Forward
			{
				float Speed = ENTITY::GET_ENTITY_SPEED(Vehid) + 0.5;
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehid, Speed);
			}

			if (GetAsyncKeyState(0x41) && !VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehid)) //Left
			{
				Vector3 Rot = ENTITY::GET_ENTITY_ROTATION(Vehid, 2);
				Rot.z = Rot.z + 1.0;
				ENTITY::SET_ENTITY_ROTATION(Vehid, Rot.x, 0, Rot.z, 2, 1);
			}

			if (GetAsyncKeyState(0x44) && !VEHICLE::IS_VEHICLE_ON_ALL_WHEELS(Vehid)) //Right
			{
				Vector3 Rot = ENTITY::GET_ENTITY_ROTATION(Vehid, 2);
				Rot.z = Rot.z - 1.0;
				ENTITY::SET_ENTITY_ROTATION(Vehid, Rot.x, 0, Rot.z, 2, 1);
			}

			if (GetAsyncKeyState(0x65)) //Numpad 5
			{
				Vector3 Rot = ENTITY::GET_ENTITY_ROTATION(Vehid, 2);
				Rot.x = Rot.x + 1.0;
				ENTITY::SET_ENTITY_ROTATION(Vehid, Rot.x, 0, Rot.z, 2, 1);
			}

			if (GetAsyncKeyState(0x68)) //Numpad 8
			{
				Vector3 Rot = ENTITY::GET_ENTITY_ROTATION(Vehid, 2);
				Rot.x = Rot.x - 1.0;
				ENTITY::SET_ENTITY_ROTATION(Vehid, Rot.x, 0, Rot.z, 2, 1);
			}

			if (GetAsyncKeyState(0x20)) //Spacebar
			{
				VEHICLE::SET_VEHICLE_FORWARD_SPEED(Vehid, 0);
			}

			if (GetAsyncKeyState(0xA0)) //Shift
			{
				Vector3 Rot = ENTITY::GET_ENTITY_ROTATION(Vehid, 2);
				ENTITY::SET_ENTITY_ROTATION(Vehid, Rot.x, 0, Rot.z, 2, 1);
			}
		}
	}
	else
	{
		Vehicle Vehid = PED::GET_VEHICLE_PED_IS_IN(ped, 0);
		ENTITY::SET_ENTITY_INVINCIBLE(Vehid, FALSE);
		ENTITY::SET_ENTITY_PROOFS(Vehid, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);
		VEHICLE::SET_VEHICLE_STRONG(Vehid, FALSE);
		VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(Vehid, TRUE);
		VEHICLE::SET_VEHICLE_CAN_BREAK(Vehid, TRUE);
		VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(Vehid, TRUE);
	}
}

DWORD featureWeaponVehShootLastTime = 0;
void vehicle_rocket(Player player, bool state)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Ped playerGroup = PED::GET_PED_GROUP_INDEX(playerPed);

	if (state && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && GetAsyncKeyState(0x45))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("WEAPON_VEHICLE_ROCKET");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}

void vehicle_minigun(Player player, bool state)
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Ped playerGroup = PED::GET_PED_GROUP_INDEX(playerPed);

	if (state && featureWeaponVehShootLastTime + 150 < GetTickCount() &&
		PLAYER::IS_PLAYER_CONTROL_ON(player) && PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0) && GetAsyncKeyState(0x45))
	{
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		Vector3 v0, v1;
		GAMEPLAY::GET_MODEL_DIMENSIONS(ENTITY::GET_ENTITY_MODEL(veh), &v0, &v1);

		Hash weaponAssetRocket = GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_PLAYER_LAZER");
		if (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
		{
			WEAPON::REQUEST_WEAPON_ASSET(weaponAssetRocket, 31, 0);
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(weaponAssetRocket))
				WAIT(0);
		}

		Vector3 coords0from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -(v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords1from = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, (v1.x + 0.25f), v1.y + 1.25f, 0.1);
		Vector3 coords0to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, -v1.x, v1.y + 100.0f, 0.1f);
		Vector3 coords1to = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(veh, v1.x, v1.y + 100.0f, 0.1f);

		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords0from.x, coords0from.y, coords0from.z,
			coords0to.x, coords0to.y, coords0to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);
		GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(coords1from.x, coords1from.y, coords1from.z,
			coords1to.x, coords1to.y, coords1to.z,
			250, 1, weaponAssetRocket, playerPed, 1, 0, -1.0);

		featureWeaponVehShootLastTime = GetTickCount();
	}
}

void setVehicleVisible(Vehicle vehicle, bool state)
{
	if (state)
	{
		ENTITY::SET_ENTITY_VISIBLE(vehicle, FALSE, 1);
		//ENTITY::SET_ENTITY_ALPHA(vehicle, 0, false);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(vehicle, TRUE, 0);
		//ENTITY::SET_ENTITY_ALPHA(vehicle, 255, false);
	}
}

void BoostBaseVehicleStats(Vehicle vehicle, bool notification)
{
	if (vehicle != NULL)
	{
		RequestControl(vehicle);

		VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
		VEHICLE::_SET_VEHICLE_PAINT_FADE(vehicle, 0.0f);
		VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE); //What seems to be the officer, problem? *le9gagmemeface*
		VEHICLE::SET_VEHICLE_TYRES_CAN_BURST(vehicle, FALSE); //Bulletproof Tires.

		Hash vehicleModel = ENTITY::GET_ENTITY_MODEL(vehicle);
		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicleModel) || VEHICLE::IS_THIS_MODEL_A_BIKE(vehicleModel))
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0); //IDK what this does, but I guess it allows individual mods to be added? It's what the game does before calling SET_VEHICLE_MOD.
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ENGINE, MOD_INDEX_FOUR, FALSE); //6fast9furious
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_BRAKES, MOD_INDEX_THREE, FALSE); //GOTTA STOP FAST
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_TRANSMISSION, MOD_INDEX_THREE, FALSE); //Not when I shift in to MAXIMUM OVERMEME!
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_SUSPENSION, MOD_INDEX_FOUR, FALSE); //How low can you go?
			VEHICLE::SET_VEHICLE_MOD(vehicle, MOD_ARMOR, MOD_INDEX_FIVE, FALSE); //100% armor.
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TURBO, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_ARMOR, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_XENONLIGHTS, TRUE);
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, MOD_TIRESMOKE, TRUE);
			//VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, TIRESMOKE_COLOR_PATRIOT);
			VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, WINDOWTINT_BLACK);
			VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, "DIU7689");
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 0, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 1, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 2, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, 3, TRUE);
			VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, NEON_COLOR_BACKLIGHT);
		}

		RepairVehicle(vehicle);
		SetVehicleModded(vehicle);

		if (notification)
		{
			DrawNotification("Upgraded Vehicle");
		}
	}
}

void RepairVehicle(Vehicle vehicle)
{
	VEHICLE::SET_VEHICLE_FIXED(vehicle);
	VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(vehicle);
	VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_BODY_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, 1000.0f);
	VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.0f);
}

void DumpPlayerFromVehicle(Ped ped, bool notification)
{

	if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
	{
		//Remove PED from vehicle
		AI::CLEAR_PED_TASKS_IMMEDIATELY(ped);

		//need to remove the parachute: 0xFBAB5776
		WEAPON::REMOVE_WEAPON_FROM_PED(ped, 0xFBAB5776);
		if (notification)
		{
			DrawNotification("Player has been kicked from vehicle!");
		}
	}
	else
	{
		if (notification)
		{

			DrawNotification("Player isn't in vehicle!");
		}
	}
}

void SetVehicleModded(Vehicle vehicle)
{
	DECORATOR::DECOR_REGISTER("Veh_Modded_By_Player", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Veh_Modded_By_Player", $(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())));
}

float Height(Vector3 Coordinate)
{
	float waterheight, groundheight;

	WATER::GET_WATER_HEIGHT(Coordinate.x, Coordinate.y, Coordinate.z, &waterheight);
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(Coordinate.x, Coordinate.y, Coordinate.z, &groundheight, 0);

	return waterheight > groundheight ? waterheight : groundheight;
}

void SpawnPlayerVehicle(Ped playerPed, Hash vehicleHash, bool spawnInVehicle, bool baseUpgrade, bool maxUpgrade)
{
	AddEntitySpawn(vehicleHash, [=]
	               {
		               RequestControl(playerPed);

					   STREAMING::REQUEST_MODEL(vehicleHash);
					   while (!STREAMING::HAS_MODEL_LOADED(vehicleHash)) WAIT(0);

		               float currentSpeed = ENTITY::GET_ENTITY_SPEED(playerPed);
		               Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed);
						Vehicle playerVeh = PED::GET_VEHICLE_PED_IS_USING(playerPed);

		               Vector3 spawnPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 6, 0);

		               if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && spawnInVehicle)
		               {
			               DumpPlayerFromVehicle(playerPed, false);
		               }

		               float groundZ = Height(spawnPos);

		               BOOL isPlane = VEHICLE::IS_THIS_MODEL_A_PLANE(vehicleHash);
		               BOOL isHeli = VEHICLE::IS_THIS_MODEL_A_HELI(vehicleHash);

		               if ((isHeli || isPlane) && spawnInVehicle)
		               {
			               spawnPos.z = max(groundZ + 500, spawnPos.z);
		               }

		               Vehicle vehicle = VEHICLE::CREATE_VEHICLE(vehicleHash, spawnPos.x, spawnPos.y, spawnPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

		               RequestControl(vehicle);

		               DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
		               DECORATOR::DECOR_REMOVE(vehicle, "Player_Vehicle");

		               VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

		               if (!(isHeli || isPlane))
		               {
			               VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);
		               }

		               if (baseUpgrade || maxUpgrade)
		               {
						   if (vehicleHash != 0x2EA68690)
						   {
								BoostBaseVehicleStats(vehicle, false);
						   }
						   //VEHICLE::SET_VEHICLE_COLOURS(vehicle, COLOR_CLASSIC_SLATE_BLUE, COLOR_CLASSIC_SLATE_BLUE);
						   //VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, COLOR_CLASSIC_CANDY_RED, COLOR_CLASSIC_SLATE_BLUE);
		               }
		               if (maxUpgrade)
		               {
						   if (vehicleHash != 0x2EA68690)
						   {
							   MaxUpgrade(vehicle);
						   }
		               }

		               if (isHeli)
		               {
			               VEHICLE::SET_HELI_BLADES_FULL_SPEED(vehicle);
		               }

		               if (spawnInVehicle)
		               {
			               ENTITY::SET_ENTITY_VELOCITY(vehicle, currentVelocity.x, currentVelocity.y, max(-5, currentVelocity.z));
			               PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, SEAT_DRIVER);
			               VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, currentSpeed);
						   VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle, FALSE);

			               if (isHeli || isPlane)
			               {
				               VEHICLE::_SET_VEHICLE_LANDING_GEAR(vehicle, 3);
			               }
		               }

					   STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicleHash);
					   ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&playerVeh);
					   
		               DrawNotification(fmt::format("Spawned {0}", UI::_GET_LABEL_TEXT(VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicleHash))));
	               });
}

/*void SpawnSavedVehicle(Ped playerPed, SavedVehicle savedVehicle, bool spawnInVehicle)
{
	AddEntitySpawn(savedVehicle.hash, [=]
	               {
		               RequestControl(playerPed);

		               float currentSpeed = ENTITY::GET_ENTITY_SPEED(playerPed);
		               Vector3 currentVelocity = ENTITY::GET_ENTITY_VELOCITY(playerPed);

		               Vector3 spawnPos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(playerPed, 0, 6, 0);

		               if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && spawnInVehicle)
		               {
			               DumpPlayerFromVehicle(playerPed, false);
		               }

		               float groundZ = Height(spawnPos);

		               BOOL isPlane = VEHICLE::IS_THIS_MODEL_A_PLANE(savedVehicle.hash);
		               BOOL isHeli = VEHICLE::IS_THIS_MODEL_A_HELI(savedVehicle.hash);

		               if ((isHeli || isPlane) && spawnInVehicle)
		               {
			               spawnPos.z = max(groundZ + 500, spawnPos.z);
		               }

		               Vehicle vehicle = VEHICLE::CREATE_VEHICLE(savedVehicle.hash, spawnPos.x, spawnPos.y, spawnPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), TRUE, TRUE);

		               RequestControl(vehicle);

		               DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
		               DECORATOR::DECOR_REMOVE(vehicle, "Player_Vehicle");

		               VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

		               if (!(isHeli || isPlane))
		               {
			               VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle);
		               }

		               if (isHeli)
		               {
			               VEHICLE::SET_HELI_BLADES_FULL_SPEED(vehicle);
		               }

		               if (spawnInVehicle)
		               {
			               ENTITY::SET_ENTITY_VELOCITY(vehicle, currentVelocity.x, currentVelocity.y, max(-5, currentVelocity.z));
			               PED::SET_PED_INTO_VEHICLE(playerPed, vehicle, SEAT_DRIVER);
			               VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle, currentSpeed);

			               if (isHeli || isPlane)
			               {
				               VEHICLE::_SET_VEHICLE_LANDING_GEAR(vehicle, 3);
			               }
		               }

		               VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, savedVehicle.wheelType);

		               for each (std::pair<int, int> mod in savedVehicle.mods)
		               {
			               SetVehicleMod(vehicle, mod.first, mod.second);
		               }

					   for each (int toggle in savedVehicle.toggles)
		               {
						   VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, toggle, TRUE);
		               }

		               for each (std::pair<int, BOOL> extra in savedVehicle.extras)
		               {
			               VEHICLE::SET_VEHICLE_EXTRA(vehicle, extra.first, extra.second == TRUE ? 0 : -1);
		               }

		               VEHICLE::SET_VEHICLE_COLOURS(vehicle, savedVehicle.primaryColor, savedVehicle.secondaryColor);
		               VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, savedVehicle.pearlColor, savedVehicle.wheelColor);

		               if (savedVehicle.customPrimary)
		               {
			               VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, savedVehicle.primaryRGB.red, savedVehicle.primaryRGB.green, savedVehicle.primaryRGB.blue);
		               }

		               if (savedVehicle.customSecondary)
		               {
			               VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, savedVehicle.secondaryRGB.red, savedVehicle.secondaryRGB.green, savedVehicle.secondaryRGB.blue);
		               }

		               VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, savedVehicle.tireSmoke.red, savedVehicle.tireSmoke.green, savedVehicle.tireSmoke.blue);

		               for each (std::pair<int, BOOL> neon in savedVehicle.neons)
		               {
			               VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, neon.first, neon.second);
		               }

		               VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, savedVehicle.neonColor.red, savedVehicle.neonColor.green, savedVehicle.neonColor.blue);

		               VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, savedVehicle.plateType);

		               VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, savedVehicle.plateText);

		               VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, savedVehicle.windowType);

		               SetVehicleModded(vehicle);

		               ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle);

		               DrawNotification(fmt::format("Spawned {0}", savedVehicle.name));
	               });
}

SavedVehicle SaveVehicle(char* name, Vehicle vehicle)
{

		char* name;
		Hash hash;
		int wheelType;
		std::vector<std::pair<int, int>> mods;
		std::vector<int> toggles;
		std::vector<std::pair<int, bool>> extras;
		char* plateText;
		int plateType;
		int windowType;
		color_t tireSmoke;
		std::vector<std::pair<int, bool>> neons;
		color_t neonColor;
		int primaryColor;
		int secondaryColor;
		int pearlColor;
		int wheelColor;
		bool customPrimary;
		bool customSecondary;
		color_t primaryRGB;
		color_t secondaryRGB;	


	std::vector<std::pair<int, int>> mods;

	for (int i = 0; i < 50; i++)
	{
		if (VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) > 0)
		{
			mods.push_back({i, VEHICLE::GET_VEHICLE_MOD(vehicle, i)});
		}
	}

	std::vector<int> toggles;

	for (int i = 0; i < 50; i++)
	{
		if (VEHICLE::IS_TOGGLE_MOD_ON(vehicle, i))
		{
			toggles.push_back(i);
		}
	}

	std::vector<std::pair<int, BOOL>> extras;

	for (int i = 0; i < 12; i++)
	{
		if (VEHICLE::DOES_EXTRA_EXIST(vehicle, i))
		{
			extras.push_back({i, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, i)});
		}
	}

	int tireR, tireG, tireB;

	VEHICLE::GET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, &tireR, &tireG, &tireB);

	std::vector<std::pair<int, BOOL>> neons;

	for (int i = 0; i < 5; i++)
	{
		neons.push_back({ i, VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(vehicle, i) });
	}

	int neonR, neonG, neonB;

	VEHICLE::_GET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, &neonR, &neonG, &neonB);

	int primary, secondary;

	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &primary, &secondary);

	int pearl, wheel;

	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &pearl, &wheel);

	int primR, primG, primB;

	VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, &primR, &primG, &primB);

	int secR, secG, secB;

	VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, &secR, &secG, &secB);

	return
	{
		name,
		ENTITY::GET_ENTITY_MODEL(vehicle),
		VEHICLE::GET_VEHICLE_WHEEL_TYPE(vehicle),
		mods,
		toggles,
		extras,
		_strdup(VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT(vehicle)),
		VEHICLE::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle),
		VEHICLE::GET_VEHICLE_WINDOW_TINT(vehicle),
		{tireR, tireG, tireB},
		neons,
		{neonR, neonG, neonB},
		primary,
		secondary,
		pearl,
		wheel,
		VEHICLE::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM(vehicle),
		VEHICLE::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM(vehicle),
		{primR, primG, primB},
		{secR, secG, secB}
	};
}

Menu SavedMenu(std::vector<SavedVehicle> savedVehicles, Ped selfPed, bool* spawnInVehicle)
{
	Menu savedMenu = 
	{ "Saved Vehicles", "Vehicle",
		{

		}
	};

	for each (SavedVehicle savedVehicle in savedVehicles)
	{
		savedMenu.rows.push_back(enterrow(fmt::format("Spawn {0}", savedVehicle.name), [=] { SpawnSavedVehicle(selfPed, savedVehicle, *spawnInVehicle); }));
	}

	return savedMenu;
}*/

void SetPlateType(Vehicle vehicle, PlateTypes plateType)
{
	if (vehicle != NULL)
	{
		RequestControl(vehicle);
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(vehicle, plateType);
		SetVehicleModded(vehicle);
	}
}

void SetWindowTint(Vehicle vehicle, WindowTints tint)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_WINDOW_TINT(vehicle, tint);
	SetVehicleModded(vehicle);
}

void ResprayVehicle(Vehicle vehicle, int primary, int secondary, int pearl, int wheel)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, primary, secondary);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, pearl, wheel);
	SetVehicleModded(vehicle);
	DECORATOR::DECOR_SET_BOOL(vehicle, "Sprayed_Veicle_Decorator", TRUE);
	DECORATOR::DECOR_SET_TIME(vehicle, "Sprayed_Vehicle_Timer_Dec", NETWORK::GET_NETWORK_TIME());
}

void SetVehiclePrimary(Vehicle vehicle, int color)
{
	int currPrim, currSec;
	RequestControl(vehicle);
	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &currPrim, &currSec);
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, color, currSec);
}

void SetVehicleSecondary(Vehicle vehicle, int color)
{
	int currPrim, currSec;
	RequestControl(vehicle);
	VEHICLE::GET_VEHICLE_COLOURS(vehicle, &currPrim, &currSec);
	VEHICLE::SET_VEHICLE_COLOURS(vehicle, currPrim, color);
}

void SetVehiclePearl(Vehicle vehicle, int color)
{
	int currPearl, currWheelCol;
	RequestControl(vehicle);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &currPearl, &currWheelCol);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, color, currWheelCol);
}

void SetVehicleWheelColor(Vehicle vehicle, int color)
{
	int currPearl, currWheelCol;
	RequestControl(vehicle);
	VEHICLE::GET_VEHICLE_EXTRA_COLOURS(vehicle, &currPearl, &currWheelCol);
	VEHICLE::SET_VEHICLE_EXTRA_COLOURS(vehicle, currPearl, color);
}

void ToggleExtra(Vehicle vehicle, int extraID)
{
	if (VEHICLE::DOES_EXTRA_EXIST(vehicle, extraID))
	{
		RequestControl(vehicle);
		VEHICLE::SET_VEHICLE_EXTRA(vehicle, extraID, VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(vehicle, extraID) ? -1 : 0);
		SetVehicleModded(vehicle);
	}
}

void ClaimVehicle(Vehicle vehicle)
{
	RequestControl(vehicle);

	//DECORATOR::DECOR_REGISTER("Player_Vehicle", 3);
	//DECORATOR::DECOR_REGISTER("PV_Slot", 3);
	DECORATOR::DECOR_SET_INT(vehicle, "Player_Vehicle", NETWORK::_0xBC1D768F2F5D6C05(PLAYER::PLAYER_ID()));
	DECORATOR::DECOR_SET_INT(vehicle, "PV_Slot", 0);
	DECORATOR::DECOR_SET_INT(vehicle, "Not_Allow_As_Saved_Veh", $(PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID())));
}

void SetNumberPlate(Vehicle vehicle, char* text)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(vehicle, text);
	SetVehicleModded(vehicle);
}

bool DoesVehicleExist(char* name)
{
	//return !AreStringsEqual("CARNOTFOUND", VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL($(name)));
	return STREAMING::IS_MODEL_A_VEHICLE($(name)) == TRUE;
}

void SetVehicleMod(Vehicle vehicle, int modType, int modIndex)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_MOD(vehicle, modType, modIndex, TRUE);
}

void ToggleVehicleMod(Vehicle vehicle, int modType)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, !VEHICLE::IS_TOGGLE_MOD_ON(vehicle, modType));
}

void SetVehicleToggle(Vehicle vehicle, int modType, BOOL toggle)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, modType, toggle);
}

void SetVehicleLivery(Vehicle vehicle, int livery)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_LIVERY(vehicle, livery);
}

void SetTireSmoke(Vehicle vehicle, int r, int g, int b)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(vehicle, r, g, b);
}

void ToggleNeonLight(Vehicle vehicle, int index)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
	VEHICLE::_SET_VEHICLE_NEON_LIGHT_ENABLED(vehicle, index, !VEHICLE::_IS_VEHICLE_NEON_LIGHT_ENABLED(vehicle, index));
}

void MaxUpgrade(Vehicle vehicle)
{
	RequestControl(vehicle);
	VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

	for (int i = 0; i < 70; i ++)
	{
		SetVehicleMod(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1);
	}
}

std::vector<std::pair<std::string, Menu>> VehicleCustomMenu(Vehicle vehicle)
{
#pragma region PartMaps

	std::map<std::string, int> VehicleModMap =
		{
				{"Spoiler", 0},
				{"Front Bumper", 1},
				{"Rear Bumper", 2},
				{"Skirts", 3},
				{"Exhaust", 4},
				{"Chassis", 5},
				{"Grille", 6},
				{"Hood", 7},
				{"Fender", 8},
				{"Fender Ornaments", 9},
				{"Roof", 10},
				{"Engine", 11},
				{"Brakes", 12},
				{"Transmission", 13},
				{"Horn", 14},
				{"Suspension", 15},
				{"Armour", 16},
				//{"Unknown1", 17},
				//{"Unknown2", 18},
				//{"Unknown3", 19},
				//{"Unknown4", 20},
				//{"Unknown5", 21},
				//{"Unknown6", 22},
				{"Wheels", 23},
				{"Second Wheel", 24},
				{"Plateholder", 25},
				{"Vanity Plates", 26},
				{"Trim Design", 27},
				{"Bobbleheads", 28},
				{"Unknown", 29},
				{"Dials", 30},
				{"Speakers", 31},
				{"Seats", 32},
				{"Steering Wheels", 33},
				{"Column Shifter", 34},
				{"Plaques", 35},
				{"Speakers", 36},
				{"Trunk Speakers", 37},
				{"Hydraulics", 38},
				{"Engine Covers", 39},
				{"Engine Filters", 40},
				{"Unknown7", 41},
				{"Arch Covers", 42},
				{"Aerials", 43},
				{"Trim", 44},
				{"Tanks", 45},
				{"Unknown8", 46},
				{"Unknown9", 47},
				{"Livery", 48},
		};

	std::map<std::string, int> VehicleModToggles =
		{
				{"Turbo", 18},
				{"Xenon Lights", 22},
		};

	std::map<int, std::map<int, char*>> manualStrings =
		{
				{1,
						{
								{0, "Custom Front Bumper"},
						}
				},
				{2,
						{
								{0, "Custom Rear Bumper"},
						}
				},
				{3,
						{
								{0, "Custom Skirts"},
						}
				},
				{11,
						{
								{-1, "Engine Upgrade, Level 0"},
								{0,  "Engine Upgrade, Level 1"},
								{1,  "Engine Upgrade, Level 2"},
								{2,  "Engine Upgrade, Level 3"},
								{3,  "Engine Upgrade, Level 4"},
						}
				},
				{12,
						{
								{-1, "Stock Brakes"},
								{0,  "Street Brakes"},
								{1,  "Sport Brakes"},
								{2,  "Race Brakes"},
						}
				},
				{13,
						{
								{-1, "Stock Transmisison"},
								{0,  "Street Transmission"},
								{1,  "Sports Transmission"},
								{2,  "Race Transmission"},
						}
				},
				{14,
						{
								{-1, "Stock"},
								{0,  "Truck Horn"},
								{1,  "Cop Horn"},
								{2,  "Clown Horn"},
								{8,  "Sad Trombone"},
								{3,  "Musical Horn 1"},
								{4,  "Musical Horn 2"},
								{5,  "Musical Horn 3"},
								{6,  "Musical Horn 4"},
								{7,  "Musical Horn 5"},
								{9,  "Classical Horn 1"},
								{10, "Classical Horn 2"},
								{11, "Classical Horn 3"},
								{12, "Classical Horn 4"},
								{13, "Classical Horn 5"},
								{14, "Classical Horn 6"},
								{15, "Classical Horn 7"},
								{33, "Classical Horn 8"},
								{32, "Classical Horn Loop 1"},
								{34, "Classical Horn Loop 2"},
								{16, "Scale - Do"},
								{17, "Scale - Ra"},
								{18, "Scale - Mi"},
								{19, "Scale - Fa"},
								{20, "Scale - Sol"},
								{21, "Scale - La"},
								{22, "Scale - Ti"},
								{23, "Scale - Do (High)"},
								{24, "Jazz Horn 1"},
								{25, "Jazz Horn 2"},
								{26, "Jazz Horn 3"},
								{27, "Jazz Horn Loop"},
								{28, "Star Spangled Banner 1"},
								{29, "Star Spangled Banner 2"},
								{30, "Star Spangled Banner 3"},
								{31, "Star Spangled Banner 4"},
								{38, "Halloween Loop 1"},
								{40, "Halloween Loop 2"},
								{42, "San Andreas"},
								{44, "Liberty City"},
								{35, "Classical Horn Loop 1 (Non Loop)	"},
								{37, "Classical Horn Loop 2 (Non Loop)"},
								{36, "Classical Horn 8 (Start)"},
								{39, "Halloween Loop 1 (Non Loop)"},
								{41, "Halloween Loop 2 (Non Loop)"},
								{43, "San Andreas (Non Loop)"},
								{45, "Liberty City (Non Loop)"},
								{46, "Xmas 1"},
								{47, "Xmas 2"},
								{ 48, "48Liberty City (Non Loop)" },
								{ 49, "49Liberty City (Non Loop)" },
						}
				},
				{15,
						{
								{-1, "Stock Suspension"},
								{0,  "Lowered Suspension"},
								{1,  "Street Suspension"},
								{2,  "Sport Suspension"},
								{3,  "Competition Suspension"},
								{4,  "Race Suspension"},
						}
				},
				{16,
						{
								{-1, "No Armor"},
								{0,  "20% Armor"},
								{1,  "40% Armor"},
								{2,  "60% Armor"},
								{3,  "80% Armor"},
								{4,  "100% Armor"},
						}
				},
				{18,
					{
						{-1, "None"},
						{0,  "Turbo Tuning" }
					}
				},
				{22,
					{
						{-1, "Stock Lights"},
						{0,  "Xenon Lights" }
					}
				},
		};

#pragma region Paint

	std::map<std::string, int> VehicleColorsClassic =
		{
				{"Black", 0},
				{"Graphite", 1},
				{"Black Steel", 2},
				{"Dark Steel", 3},
				{"Silver", 4},
				{"Bluish Silver", 5},
				{"Rolled Steel", 6},
				{"Shadow Silver", 7},
				{"Stone Silver", 8},
				{"Midnight Silver", 9},
				{"Cast Iron Silver", 10},
				{"Anhracite Black", 11},
				{"Red", 27},
				{"Torino Red", 28},
				{"Formula Red", 29},
				{"Blaze Red", 30},
				{"Grace Red", 31},
				{"Garnet Red", 32},
				{"Sunset Red", 33},
				{"Cabernet Red", 34},
				{"Candy Red", 35},
				{"Sunrise Orange", 36},
				{"Gold", 37},
				{"Orange", 38},
				{"Dark Green", 49},
				{"Racing Green", 50},
				{"Sea Green", 51},
				{"Olive Green", 52},
				{"Bright Green", 53},
				{"Gasoline Green", 54},
				{"Pale Dark Green", 56},
				{"Midnight Green", 59},
				{"Metallic Dark Blue", 60},
				{"Galaxy Blue", 61},
				{"Dark Blue", 62},
				{"Saxon Blue", 63},
				{"Blue", 64},
				{"Mariner Blue", 65},
				{"Harbor Blue", 66},
				{"Diamond Blue", 67},
				{"Surf Blue", 68},
				{"Nautical Blue", 69},
				{"Ultra Blue", 70},
				{"Schafter Purple", 71},
				{"Spinnaker Purple", 72},
				{"Racing Blue", 73},
				{"Light Blue", 74},
				{"Slate Blue", 81},
				{"Baby Blue", 87},
				{"Yellow", 88},
				{"Race Yellow", 89},
				{"Bronze", 90},
				{"Dew Yellow", 91},
				{"Lime Green", 92},
				{"Champagne", 93},
				{"Feltzer Brown", 94},
				{"Creeen Brown", 95},
				{"Chocolate Brown", 96},
				{"Maple Brown", 97},
				{"Saddle Brown", 98},
				{"Straw Brown", 99},
				{"Moss Brown", 100},
				{"Bison Brown", 101},
				{"Woodbeech Brown", 102},
				{"Beechwood Brown", 103},
				{"Sienna Brown", 104},
				{"Sandy Brown", 105},
				{"Bleached Brown", 106},
				{"Cream", 107},
				{"Ice White", 111},
				{"Frost White", 112},
				{"Dark Brown", 115},
				{"Pea Green", 125},
				{"Police Blue", 127},
				{"Pure White", 134},
				{"Hot Pink", 135},
				{"Salmon Pink", 136},
				{"Pfsiter Pink", 137},
				{"Bright Orange", 138},
				{"Green", 139},
				{"Flourescent Blue", 140},
				{"Midnight Blue", 141},
				{"Midnight Purple", 142},
				{"Wine Red", 143},
				{"Hunter Green", 144},
				{"Bright Purple", 145},
				{"Midnight Galaxy Blue", 146},
				{"Carbon Black", 147},
				{"Lava Red", 150},
				{"Epsilon Blue", 157},
		};

	std::map<std::string, int> VehicleColorsWorn =
		{
				{"Black", 21},
				{"Graphite", 22},
				{"Silver Grey", 23},
				{"Silver", 24},
				{"Blue Silver", 25},
				{"Shadow Silver", 26},
				{"Red", 46},
				{"Golden Red", 47},
				{"Dark Red", 48},
				{"Green", 57},
				{"Sea Wash", 58},
				{"Dark Blue", 85},
				{"Blue", 86},
				{"Honey Beige", 113},
				{"Brown", 114},
				{"Straw Beige", 116},
				{"Off White", 121},
				{"Orange", 123},
				{"Light Orange", 124},
				{"Taxi Yellow", 126},
				{"Pale Orange", 130},
				{"White", 132},
				{"Army Olive Green", 133},
		};

	std::map<std::string, int> VehicleColorsUtil =
		{
				{"Black", 15},
				{"Black Poly", 16},
				{"Dark Silver", 17},
				{"Gun Metal", 19},
				{"Shadow Silver", 20},
				{"Red", 43},
				{"Bright Red", 44},
				{"Garnet Red", 45},
				{"Midnight Blue", 75},
				{"Blue", 76},
				{"Sea Foam Blue", 77},
				{"Lightning Blue", 78},
				{"Maui Blue Poly", 79},
				{"Bright Blue", 80},
				{"Brown", 108},
				{"Medium Brown", 109},
				{"Light Brown", 110},
				{"Off White", 122},
		};

	std::map<std::string, int> VehicleColorsMatte =
		{
				{"Black", 12},
				{"Gray", 13},
				{"Light Gray", 14},
				{"Red", 39},
				{"Dark Red", 40},
				{"Orange", 41},
				{"Yellow", 42},
				{"Lime Green", 55},
				{"Dark Blue", 82},
				{"Blue", 83},
				{"Midnight Blue", 84},
				{"Green", 128},
				{"Brown", 129},
				{"Ice White", 131},
				{"Schafter Purple", 148},
				{"Midnight Purple", 149},
				{"Forest Green", 151},
				{"Olive Drab", 152},
				{"Dark Earth", 153},
				{"Desert Tan", 154},
				{"Foliage Green", 155},
		};

	std::map<std::string, int> VehicleColorsMetals =
		{
				{"Pearlescent Steel", 18},
				{"Brushed Steel", 117},
				{"Brushed Black Steel", 118},
				{"Brushed Aluminum", 119},
				{"Default Alloy", 156},
				{"Pure Gold", 158},
				{"Brushed Gold", 159},
				{"Pearlescent Gold", 160},
				{"Chrome", 120},
		};

#pragma endregion

#pragma endregion

	std::vector<std::pair<std::string, Menu>> menus = {};

	Menu menu = {"shopui_title_carmod2@shopui_title_carmod2@SPRITE", "Vehicle", {}};

	if (vehicle != NULL)
	{
		VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);

#pragma region Vehicle Mods

		for each (std::pair<std::string, int> mod in VehicleModMap)
		{
			int modIndex = mod.second;
			int modCount = VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, modIndex);

			char* key = strchr(fmt::format("VehicleMod{0}", mod.first));

			if (modCount > 0)
			{
				Menu modMenu = {strchr(mod.first) , "VehicleCustomisation", {}};

				std::map<int, char*> predefined = manualStrings[modIndex];

				modMenu.rows.push_back(enterrow(predefined[-1] == nullptr ? "Default" : std::string(predefined[-1]), [=]
				                                {
					                                SetVehicleMod(vehicle, modIndex, -1);
				                                }));

				for (int i = 0; i < modCount; i++)
				{
					char* modCodeName = VEHICLE::GET_MOD_TEXT_LABEL(vehicle, modIndex, i);
					char* modDisplayName = UI::_GET_LABEL_TEXT(modCodeName);

					if (AreStringsEqual(modDisplayName, "NULL"))
					{
						if (predefined[i] != nullptr)
						{
							modDisplayName = predefined[i];
						}
						else
						{
							modDisplayName = strchr(fmt::format("{0} {1}", mod.first, i));
						}
					}

					modMenu.rows.push_back(enterrow(std::string(modDisplayName), [=]
					                                {
						                                SetVehicleMod(vehicle, modIndex, i);
					                                }));
				}



				menu.rows.push_back(menurow(fmt::format("{0} ({1})", mod.first, modCount), key));

				menus.push_back({key, modMenu});
			}
		}

#pragma endregion

#pragma region Wheel Type

		Menu wheelMenu =
			{"Wheel Type", "VehicleCustomisation",
					{
						enterrow("Sport", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 0);
						         }),
						enterrow("Muscle", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 1);
						         }),
						enterrow("Lowrider", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 2);
						         }),
						enterrow("SUV", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 3);
						         }),
						enterrow("Offroad", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 4);
						         }),
						enterrow("Tuner", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 5);
						         }),
						enterrow("Bike Wheels", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 6);
						         }),
						enterrow("High End", [=]
						         {
							         VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 7);
						         }),
						enterrow("Benny's originals", [=]
								 {
									 VEHICLE::SET_VEHICLE_WHEEL_TYPE(vehicle, 8);
								 }),
					}
			};

		menus.push_back({"VehicleModWheelType", wheelMenu});

		menu.rows.push_back(menurow("Wheel Type", "VehicleModWheelType"));

#pragma endregion

#pragma region Respray

		Menu resprayMenu =
			{"Respray", "VehicleCustomisation",
					{
						menurow("Primary", "VehiclePrimSel"),
						menurow("Secondary", "VehicleSecSel"),
						menurow("Pearl", "VehiclePearlSel"),
						menurow("Wheel", "VehicleWheelSel"),
					}
			};

		menus.push_back({"VehicleRespray", resprayMenu});

		menu.rows.push_back(menurow("Respray", "VehicleRespray"));

#pragma region Respray Primary

		static int primaryR, primaryG, primaryB;

		Menu primMenu =
			{"Primary Colors", "VehicleRespray",
					{
						menurow("Classic", "VehiclePrimClassic"),
						menurow("Worn", "VehiclePrimWorn"),
						menurow("Utils", "VehiclePrimUtils"),
						menurow("Matte", "VehiclePrimMatte"),
						menurow("Metals", "VehiclePrimMetals"),
						introw("Primary Red", &primaryR, 0, 255, 1),
						introw("Primary Green", &primaryG, 0, 255, 1),
						introw("Primary Blue", &primaryB, 0, 255, 1),
						enterrow("Set Primary Color", [&, vehicle]
						         {
							         VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle, primaryR, primaryG, primaryB);
						         }),
						enterrow("Clear Custom Primary", [=]
						         {
							         VEHICLE::CLEAR_VEHICLE_CUSTOM_PRIMARY_COLOUR(vehicle);
						         }),
					}
			};

		menus.push_back({"VehiclePrimSel", primMenu});

		Menu primMenuClassic = {"Classic", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			primMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehiclePrimary(vehicle, color.second);
			                                        }));
		}

		sort(primMenuClassic.rows.begin(), primMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimClassic", primMenuClassic});

		Menu primMenuWorn = {"Worn", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			primMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehiclePrimary(vehicle, color.second);
			                                     }));
		}

		sort(primMenuWorn.rows.begin(), primMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimWorn", primMenuWorn});

		Menu primMenuUtils = {"Utils", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			primMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePrimary(vehicle, color.second);
			                                      }));
		}

		sort(primMenuUtils.rows.begin(), primMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimUtils", primMenuUtils});

		Menu primMenuMatte = {"Matte", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			primMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePrimary(vehicle, color.second);
			                                      }));
		}

		sort(primMenuMatte.rows.begin(), primMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimMatte", primMenuMatte});

		Menu primMenuMetals = {"Metals", "VehiclePrimSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			primMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePrimary(vehicle, color.second);
			                                       }));
		}

		sort(primMenuMetals.rows.begin(), primMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePrimMetals", primMenuMetals});

#pragma endregion

#pragma region Respray Secondary

		static int secondaryR, secondaryG, secondaryB;

		Menu secMenu =
			{"Secondary Colors", "VehicleRespray",
					{
						menurow("Classic", "VehicleSecClassic"),
						menurow("Worn", "VehicleSecWorn"),
						menurow("Utils", "VehicleSecUtils"),
						menurow("Matte", "VehicleSecMatte"),
						menurow("Metals", "VehicleSecMetals"),
						introw("Secondary Red", &secondaryR, 0, 255, 1),
						introw("Secondary Green", &secondaryG, 0, 255, 1),
						introw("Secondary Blue", &secondaryB, 0, 255, 1),
						enterrow("Set Secondary Color", [&, vehicle]
						         {
							         VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle, secondaryR, secondaryG, secondaryB);
						         }),
						enterrow("Clear Custom Secondary", [=]
						         {
							         VEHICLE::CLEAR_VEHICLE_CUSTOM_SECONDARY_COLOUR(vehicle);
						         }),
					}
			};

		menus.push_back({"VehicleSecSel", secMenu});

		Menu secMenuClassic = {"Classic", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			secMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleSecondary(vehicle, color.second);
			                                       }));
		}

		sort(secMenuClassic.rows.begin(), secMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecClassic", secMenuClassic});

		Menu secMenuWorn = {"Worn", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			secMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                    {
				                                    SetVehicleSecondary(vehicle, color.second);
			                                    }));
		}

		sort(secMenuWorn.rows.begin(), secMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecWorn", secMenuWorn});

		Menu secMenuUtils = {"Utils", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			secMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehicleSecondary(vehicle, color.second);
			                                     }));
		}

		sort(secMenuUtils.rows.begin(), secMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecUtils", secMenuUtils});

		Menu secMenuMatte = {"Matte", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			secMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                     {
				                                     SetVehicleSecondary(vehicle, color.second);
			                                     }));
		}

		sort(secMenuMatte.rows.begin(), secMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecMatte", secMenuMatte});

		Menu secMenuMetals = {"Metals", "VehicleSecSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			secMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehicleSecondary(vehicle, color.second);
			                                      }));
		}

		sort(secMenuMetals.rows.begin(), secMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehicleSecMetals", secMenuMetals});

#pragma endregion

#pragma region Respray Pearl

		Menu pearlMenu =
			{"Pearl Colors", "VehicleRespray",
					{
						menurow("Classic", "VehiclePearlClassic"),
						menurow("Worn", "VehiclePearlWorn"),
						menurow("Utils", "VehiclePearlUtils"),
						menurow("Matte", "VehiclePearlMatte"),
						menurow("Metals", "VehiclePearlMetals"),
					}
			};

		menus.push_back({"VehiclePearlSel", pearlMenu});

		Menu pearlMenuClassic = {"Classic", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			pearlMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                         {
				                                         SetVehiclePearl(vehicle, color.second);
			                                         }));
		}

		sort(pearlMenuClassic.rows.begin(), pearlMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlClassic", pearlMenuClassic});

		Menu pearlMenuWorn = {"Worn", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			pearlMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehiclePearl(vehicle, color.second);
			                                      }));
		}

		sort(pearlMenuWorn.rows.begin(), pearlMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlWorn", pearlMenuWorn});

		Menu pearlMenuUtils = {"Utils", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			pearlMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePearl(vehicle, color.second);
			                                       }));
		}

		sort(pearlMenuUtils.rows.begin(), pearlMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlUtils", pearlMenuUtils});

		Menu pearlMenuMatte = {"Matte", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			pearlMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehiclePearl(vehicle, color.second);
			                                       }));
		}

		sort(pearlMenuMatte.rows.begin(), pearlMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlMatte", pearlMenuMatte});

		Menu pearlMenuMetals = {"Metals", "VehiclePearlSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			pearlMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehiclePearl(vehicle, color.second);
			                                        }));
		}

		sort(pearlMenuMetals.rows.begin(), pearlMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehiclePearlMetals", pearlMenuMetals});

#pragma endregion

#pragma region Respray Wheel

		Menu wheelPaintMenu =
			{"Wheel Colors", "VehicleRespray",
					{
						menurow("Classic", "VehicleWheelClassic"),
						menurow("Worn", "VehicleWheelWorn"),
						menurow("Utils", "VehicleWheelUtils"),
						menurow("Matte", "VehicleWheelMatte"),
						menurow("Metals", "VehicleWheelMetals"),
					}
			};

		menus.push_back({"VehicleWheelSel", wheelPaintMenu});

		Menu wheelMenuClassic = {"Classic", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsClassic)
		{
			wheelMenuClassic.rows.push_back(enterrow(color.first, [=]
			                                         {
				                                         SetVehicleWheelColor(vehicle, color.second);
			                                         }));
		}

		sort(wheelMenuClassic.rows.begin(), wheelMenuClassic.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelClassic", wheelMenuClassic});

		Menu wheelMenuWorn = {"Worn", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsWorn)
		{
			wheelMenuWorn.rows.push_back(enterrow(color.first, [=]
			                                      {
				                                      SetVehicleWheelColor(vehicle, color.second);
			                                      }));
		}

		sort(wheelMenuWorn.rows.begin(), wheelMenuWorn.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelWorn", wheelMenuWorn});

		Menu wheelMenuUtils = {"Utils", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsUtil)
		{
			wheelMenuUtils.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleWheelColor(vehicle, color.second);
			                                       }));
		}

		sort(wheelMenuUtils.rows.begin(), wheelMenuUtils.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelUtils", wheelMenuUtils});

		Menu wheelMenuMatte = {"Matte", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMatte)
		{
			wheelMenuMatte.rows.push_back(enterrow(color.first, [=]
			                                       {
				                                       SetVehicleWheelColor(vehicle, color.second);
			                                       }));
		}

		sort(wheelMenuMatte.rows.begin(), wheelMenuMatte.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelMatte", wheelMenuMatte});

		Menu wheelMenuMetals = {"Metals", "VehicleWheelSel", {}};

		for each (std::pair<std::string, int> color in VehicleColorsMetals)
		{
			wheelMenuMetals.rows.push_back(enterrow(color.first, [=]
			                                        {
				                                        SetVehicleWheelColor(vehicle, color.second);
			                                        }));
		}

		sort(wheelMenuMetals.rows.begin(), wheelMenuMetals.rows.end(), MenuRowSort);

		menus.push_back({"VehicleWheelMetals", wheelMenuMetals});

#pragma endregion

#pragma endregion

#pragma region Livery

		int liveryCount = VEHICLE::GET_VEHICLE_LIVERY_COUNT(vehicle);

		if (liveryCount != -1)
		{
			Menu liveryMenu = {"Livery" , "VehicleCustomisation", {}};

			for (int i = 0; i < liveryCount; i++)
			{
				liveryMenu.rows.push_back(enterrow(fmt::format("Livery {0}", i), [=]
				                                   {
					                                   SetVehicleLivery(vehicle, i);
				                                   }));
			}

			menus.push_back({"VehicleModLivery", liveryMenu});

			menu.rows.push_back(menurow("Livery", "VehicleModLivery"));
		}

#pragma endregion 

#pragma region Extras

		Menu extrasMenu = {"Extras" , "VehicleCustomisation", {}};

		for (int i = 1; i <= 12; i++) // 10-12 are mainly small interior features
		{
			if (VEHICLE::DOES_EXTRA_EXIST(vehicle, i))
			{
				extrasMenu.rows.push_back(enterrow(fmt::format("Toggle Extra {0}", i), [=]
				                                   {
					                                   ToggleExtra(vehicle, i);
				                                   }));
			}
		}

		if (extrasMenu.rows.size() > 0)
		{
			menus.push_back({"VehicleModExtra", extrasMenu});

			menu.rows.push_back(menurow("Extras", "VehicleModExtra"));
		}

#pragma endregion 

#pragma region Window Tint

		Menu windowTintMenu =
			{"Window Tint" , "VehicleCustomisation",
					{
						enterrow("Stock", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_STOCK);
						         }),
						enterrow("None", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_NONE);
						         }),
						enterrow("Light Smoke", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_LIGHTSMOKE);
						         }),
						enterrow("Dark Smoke", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_DARKSMOKE);
						         }),
						enterrow("Black", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_BLACK);
						         }),
						enterrow("Green", [=]
						         {
							         SetWindowTint(vehicle, WINDOWTINT_GREEN);
						         }),
					}
			};


		menus.push_back({"VehicleModWindowTint", windowTintMenu});

		menu.rows.push_back(menurow("Window Tint", "VehicleModWindowTint"));

#pragma endregion

#pragma region License Plate

		Menu licensePlateMenu =
			{"License Plate" , "VehicleCustomisation",
					{
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE1);
						         }),
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE2);
						         }),
						enterrow("Blue On White 1", [=]
						         {
							         SetPlateType(vehicle, PLATE_BLUEONWHITE3);
						         }),
						enterrow("Yellow On Blue", [=]
						         {
							         SetPlateType(vehicle, PLATE_YELLOWONBLUE);
						         }),
						enterrow("Yellow On Black", [=]
						         {
							         SetPlateType(vehicle, PLATE_YELLOWONBLACK);
						         }),
						enterrow("Yankton", [=]
						         {
							         SetPlateType(vehicle, PLATE_YANKTON);
						         }),
						inputrow("Set Number Plate", 8, [=] (char* result)
						         {
							         SetNumberPlate(vehicle, result);
						         }),
					}
			};

		menus.push_back({"VehicleModLicensePlate", licensePlateMenu});

		menu.rows.push_back(menurow("License Plate", "VehicleModLicensePlate"));

#pragma endregion

#pragma region Tire Smoke

		static int tireR, tireG, tireB;

		Menu tireSmokeMenu =
			{"Tire Smoke", "VehicleCustomisation",
					{
						introw("Tire Smoke Red", &tireR, 0, 255, 1),
						introw("Tire Smoke Green", &tireG, 0, 255, 1),
						introw("Tire Smoke Blue", &tireB, 0, 255, 1),
						enterrow("Set Tire Smoke", [&, vehicle]
						         {
							         SetTireSmoke(vehicle, tireR, tireG, tireB);
						         }),
					}
			};

		menus.push_back({"VehicleModTireSmoke", tireSmokeMenu});

		menu.rows.push_back(menurow("Tire Smoke", "VehicleModTireSmoke"));

#pragma endregion

#pragma region Neon Light

		static int neonR, neonG, neonB;

		Menu neonMenu =
			{"Neon Lights", "VehicleCustomisation",
					{
						enterrow("Toggle Front Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 2);
						         }),
						enterrow("Toggle Back Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 3);
						         }),
						enterrow("Toggle Left Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 0);
						         }),
						enterrow("Toggle Right Neon", [=]
						         {
							         ToggleNeonLight(vehicle, 1);
						         }),
						introw("Neon Red", &neonR, 0, 255, 1),
						introw("Neon Green", &neonG, 0, 255, 1),
						introw("Neon Blue", &neonB, 0, 255, 1),
						enterrow("Set Neon Color", [&, vehicle]
						         {
							         VEHICLE::_SET_VEHICLE_NEON_LIGHTS_COLOUR(vehicle, neonR, neonG, neonB);
						         }),
					}
			};

		menus.push_back({"VehicleModNeon", neonMenu});

		menu.rows.push_back(menurow("Neon Lights", "VehicleModNeon"));

#pragma endregion

#pragma region Vehicle Toggle Mods

		for each (std::pair<std::string, int> toggle in VehicleModToggles)
		{	

			char* key = strchr(fmt::format("VehicleToggle{0}", toggle.first));

			//menu.rows.push_back(enterrow(fmt::format("Toggle {0}", toggle.first), [=]
			//                             {
			//	                             ToggleVehicleMod(vehicle, toggle.second);
			//                             }));

			Menu toggleMenu = { strchr(toggle.first) , "VehicleCustomisation",{} };

			std::map<int, char*> predefined = manualStrings[toggle.second];

			toggleMenu.rows.push_back(enterrow(predefined[-1] == nullptr ? "Default" : std::string(predefined[-1]), [=]
			{
				SetVehicleToggle(vehicle, toggle.second, FALSE);
			}));

			char* modCodeName = VEHICLE::GET_MOD_TEXT_LABEL(vehicle, toggle.second, 0);
			char* modDisplayName = UI::_GET_LABEL_TEXT(modCodeName);

				if (AreStringsEqual(modDisplayName, "NULL"))
				{
					if (predefined[0] != nullptr)
					{
						modDisplayName = predefined[0];
					}
					else
					{
						modDisplayName = strchr(fmt::format("{0} {1}", toggle.first, 0));
					}
				}

				toggleMenu.rows.push_back(enterrow(std::string(modDisplayName), [=]
				{
					SetVehicleToggle(vehicle, toggle.second, TRUE);
				}));				

			menu.rows.push_back(menurow(toggle.first, key));

			menus.push_back({ key, toggleMenu });
		}

#pragma endregion

		sort(menus.begin(), menus.end(), MenuPairSort);
	}

	menus.push_back({"VehicleCustomisation", menu});

	return menus;
}