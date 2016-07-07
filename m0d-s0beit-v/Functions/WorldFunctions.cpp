#include "../stdafx.h"

/*float Height(Vector3 Coordinate)
{
	float waterheight, groundheight;

	WATER::GET_WATER_HEIGHT(Coordinate.x, Coordinate.y, Coordinate.z, &waterheight);
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(Coordinate.x, Coordinate.y, Coordinate.z, &groundheight);

	return waterheight > groundheight ? waterheight : groundheight;
}

void SetWeather(char* weatherType)
{
	GAMEPLAY::SET_WEATHER_TYPE_NOW(weatherType);
}

Menu WeatherMenu()
{
	Menu savedMenu =
	{ "Set weather", "SetWeather",
		{

		}
	};

	for each (char * weatherType in WeatherHashes)
	{
		savedMenu.rows.push_back(enterrow(fmt::format("{0}", weatherType), [=] { SetWeather(weatherType); }));
	}

	return savedMenu;
}

void ToggleWalkRadio(bool state) {
	if (ENTITY::DOES_ENTITY_EXIST(PLAYER::PLAYER_PED_ID()))
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(state);
}*/

void AntiCrash()
{
	const int arrSize2 = 1024;
	Ped vehs[arrSize2];

	int nearbyVehicles = worldGetAllVehicles(vehs, arrSize2);
	for (int i = 0; i < nearbyVehicles; i++)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(vehs[i]))
		{
			continue;
		}

		Vehicle VehiclesToDelete = vehs[i];
		RequestControl(VehiclesToDelete);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(VehiclesToDelete, TRUE, TRUE);
		//if (nearbyVehicles >= 30)
		//{
			VEHICLE::DELETE_VEHICLE(&VehiclesToDelete);
		//}
	}
}

void AntiCrash2()
{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), FALSE);

	const int arrSize = 1024;
	Ped peds[arrSize];

	int nearbyPeds = worldGetAllPeds(peds, arrSize);
	for (int i = 0; i < nearbyPeds; i++)
	{

		if (!ENTITY::DOES_ENTITY_EXIST(peds[i]) || PED::IS_PED_A_PLAYER(peds[i]))
		{
			continue;
		}

		Ped pedToDelete = peds[i];
		RequestControl(pedToDelete);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pedToDelete, TRUE, TRUE);
		
		//if (nearbyPeds >= 30)
		//{
			PED::DELETE_PED(&pedToDelete);
			//ENTITY::SET_ENTITY_COORDS_NO_OFFSET(PLAYER::PLAYER_PED_ID(), playerPosition.x, playerPosition.y, playerPosition.z + 150.0f, 0, 0, 1);
		//}
	}
}

void AntiCrash3()
{
	const int arrSize = 1024;
	Ped objects[arrSize];

	int nearbyObjects = worldGetAllObjects(objects, arrSize);
	for (int i = 0; i < nearbyObjects; i++)
	{

		if (!ENTITY::DOES_ENTITY_EXIST(objects[i]))
		{
			continue;
		}

		Object objectToDelete = objects[i];
		RequestControl(objectToDelete);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(objectToDelete, TRUE, TRUE);

		//if (nearbyObjects >= 20)
		//{
		ENTITY::DELETE_ENTITY(&objectToDelete);
		OBJECT::DELETE_OBJECT(&objectToDelete);
		//}
	}
}