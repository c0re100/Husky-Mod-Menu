#include "stdafx.h"
#define DEBUG_ENABLED 0

#pragma region Config

static float MenuX = 0.70f;
static float MenuY = 0.0625f;
static float MenuWidth = 0.2125f;
static float MenuRight = MenuX + MenuWidth;
static float RowHeight = 0.0325f;
static float TitleHeight = RowHeight * 2.5f;
static float MaxHeight = 1.0f - TitleHeight - MenuY * 2.0f;

static int MaxRows = int(round(1 / RowHeight * MaxHeight) - 1);
static int Middle = int(floor(MaxRows * 0.5f - 0.5f)); // We wanna make sure it's BEFORE half way, if MaxRows is even

// The only useful fonts are 0,1,4 and 7 (maybe 3 if you need some symbols, see Useful Files\fonts2.png)

static color_t DefaultTextColor = { 240, 240, 240, 255 };
static color_t DefaultRectColor = { 25, 25, 25, 150 };
static float DefaultTextScale = 0.5f;
static int DefaultFont = 4;
static Alignment DefaultAlignment = Left;

static color_t TitleTextColor = { 255, 255, 255, 255 };
static color_t TitleRectColor = { 0, 150, 255, 180 };
static float TitleTextScale = 0.8f;
static int TitleFont = 7;
static Alignment TitleAlignment = Center;

static color_t ActiveTextColor = DefaultTextColor;
static color_t ActiveRectColor = { 120, 120, 120, DefaultRectColor.alpha };
static float ActiveTextScale = DefaultTextScale;
static int ActiveFont = DefaultFont;
static Alignment ActiveAlignment = DefaultAlignment;

static color_t OnTextColor = { 0, 255, 0, DefaultTextColor.alpha };
static color_t OffTextColor = { 255, 0, 0, DefaultTextColor.alpha };
static color_t IntTextColor = { 255, 255, 0, DefaultTextColor.alpha };
static color_t MenuTextColor = { 255, 69, 0, DefaultTextColor.alpha };

static float TextMargin = 0.005f;
static float LineHeight = 0.002f;

static char* SoundSet = "HUD_FRONTEND_DEFAULT_SOUNDSET"; // HUD_FRONTEND_DEFAULT_SOUNDSET (recommended) or HUD_FREEMODE_SOUNDSET

static bool menuVisible; // Menu Visible and Key Presses
static bool initialized;

static Menu currMenu;
static int currentSelectionIndex = 0;
static int tries = 0;

#pragma endregion

void TestCode()
{
	
}

void UpdateMenu()
{

	#pragma region Row Variables

	static bool
		spawnInVehicle = true,
		baseUpgrades = true,
		maxUpgrades = true,
		/*customBounty,
		enableSnow,
		enableRace,
		christmasContent,
		valentinesContent,
		independenceContent,
		sellCap,
		fireworksLauncher,*/
		playerGodMode,
		ShowVehicleSpeed,
		vehicleGodMode,
		vehicleInvisible,
		vehspeed,
		vehicleSeatBelt = true,
		disableRagdoll,
		neverWanted,
		/*nightVision,
		heatVision,
		rpLoop,
		superBunker2,
		frezeTime,
		disableRadio,
		enableWalkRadio,*/
		infiniteAmmo,
		fullAmmo,
		oneHitKill,
		fireammo,
		expammo,
		playerInvisible,
		oplayerFreeze,
		fastSwim,
		fastRun,
		superjump,
		featurePlayerESP,
		featurePlayerESPBoxes,
		featurePlayerESPBoxes2,
		//featurePlayerESPTrace;
		force2kill,
		showcroods,
		showtalk = true,
		supermode,
		flyingcar,
		removegun,
		aimobj,
		aimwarn = true,
		forcegun,
		thirdperson,
		idlekick,
		votekick,
		allfreeze,
		rapidfire,
		gravitygun,
		superrun,
		fireworkgun,
		vehrpg,
		vehmini,
		forceAllkill,
		showblip,
		antiforcekill,
		removewp;

	

	static int rpmMultipler = 1, torqueMultiplier = 1, forwardspeed = 1, runspdMultiplier = 0, currTunable;

	static std::map<char*, int> prevSelections;

	static Player currentPlayer = 0;
	Ped currentPed = PLAYER::GET_PLAYER_PED(currentPlayer);
	Vehicle currentVehicle = PED::GET_VEHICLE_PED_IS_IN(currentPed, FALSE);

	/*static std::vector<SavedVehicle> savedVehicles;*/

	#pragma endregion 

	Player selfPlayer = PLAYER::PLAYER_ID();
	Ped selfPed = PLAYER::PLAYER_PED_ID();
	Vehicle selfVehicle = PED::GET_VEHICLE_PED_IS_IN(selfPed, FALSE);
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(selfPed, TRUE);
	/*
		For a list of font colors (e.g ~HUD_COLOUR_GREEN~) see 'Useful Files\hudcolor.dat'

		[]: no variables defined. Attempting to use any external variables in the lambda is an error.
		[x, &y]: x is captured by value, y is captured by reference
		[&]: any external variable is implicitly captured by reference if used
		[=]: any external variable is implicitly captured by value if used
		[&, x]: x is explicitly captured by value. Other variables will be captured by reference
		[=, &z]: z is explicitly captured by reference. Other variables will be captured by value
	*/

	#pragma region Menu

	static std::map<std::string, Menu> Menus =
	{
		{"Main", // Key
			{"Husky Mod Menu", "Main", // Title, Key for Previous Menu
				{
					menurow("Self", "Self"),
					menurow("Teleport", "Teleport"),
					menurow("Weapons", "Weapon"),
					menurow("Vehicle", "Vehicle"),
					menurow("Online", "Online"),
					menurow("All Player", "All Player"),
					menurow("Tunables", "Tunables"),
					menurow("Protect", "Protect"),
					//menurow("World", "World"),
					menurow("Misc",	"Misc"),
				}
			}
		},
		{"Self",
			{"Self", "Main",
				{
					//dynmenurow("Clothing", "Clothing", [&] { for each (std::pair<std::string, Menu> menu in ClothingMenus(selfPed)) { Menus[menu.first] = menu.second; } }),
					menurow("ESP", "ESP"),
					boolrow("God Mode",				 &playerGodMode),
					boolrow("Disable Ragdoll",		 &disableRagdoll),
					boolrow("Never Wanted",			 &neverWanted),
					enterrow("Replenish Self",		 [&] { ReplenishPlayer(selfPed);	}),
					boolrow("Invisible",			 &playerInvisible),
					boolrow("Fast swim",			 &fastSwim),
					boolrow("Fast run",				 &fastRun),
					boolrow("Super Jump",			 &superjump),
					enterrow("Clear Wanted Level",	 [&] { ClearWantedLevel(selfPlayer);		}),
					enterrow("Clear Prop from self", [&] { RemoveAllPropsFromPlayer(selfPed); }),
					boolrow("Show Your Croods",		 &showcroods),
					boolrow("Show Who's Talking",	 &showtalk),
					boolrow("Aiming Warning",		 &aimwarn),
					boolrow("Force Third Person",	 &thirdperson),
					enterrow("Reset Skin",			 [=] { PED::SET_PED_DEFAULT_COMPONENT_VARIATION(selfPed); }),
					boolrow("Anti Out Of Area",		 &antiforcekill),
					boolrow("Super Run",			 &superrun),
					boolrow("Super Mode",			 &supermode),
					enterrow("Off The Radar",		 [] { offtheradar(); }),

				}
			}
		},
		{ "Teleport",
			{ "Teleport", "Main",
				{
					enterrow("Marker",					[&] { teleport(true, vector3_t()); }),
					enterrow("Forward Teleport",		[&] { TeleportToForward(); }),
					enterrow("Teleport to Mission Objective",	[] { TeleporttoMissionObjective(); }),
					dynmenurow("Locations", "Location", [&] { Menus["Location"] = TpLocationsMenu(); }),
				}
			}
		},
		{ "Weapon",
			{ "Weapon", "Main",
				{
					enterrow("Give All Weapons",	[&] { GiveAllWeapons(selfPed); }),
					boolrow("Infinite Ammo",		&infiniteAmmo),
					boolrow("Full Ammo",			&fullAmmo),
					boolrow("One hit kill",			&oneHitKill ),
					boolrow("Fire Ammo",			&fireammo),
					boolrow("Explosive Ammo",		&expammo),
					boolrow("Rapid Fire",			&rapidfire),
					boolrow("Force Gun",			&forcegun),
					boolrow("Gravity Gun",			&gravitygun),
					boolrow("Firework Gun",			&fireworkgun),
				}
			}
		},
		{"ESP",
			{"ESP", "Self",
				{
					/*boolfuncrow("Night Vision", &nightVision, [] { GRAPHICS::SET_NIGHTVISION(nightVision); }),
					boolfuncrow("Heat Vision", &heatVision, [] { GRAPHICS::SET_SEETHROUGH(heatVision); }),
					boolrow("ESP Trace",		&featurePlayerESPTrace),*/
					boolrow("ESP",				&featurePlayerESP),
					boolrow("3D ESP Boxes",		&featurePlayerESPBoxes),
					boolrow("2D ESP Boxes",		&featurePlayerESPBoxes2),
				}
			}
		},
		{"Vehicle",
			{"Vehicle", "Main",
				{
					menurow("Spawn Vehicle",	"VehicleSpawn"),
					/*dynmenurow("Saved Vehicles", "VehicleSaved", [&] { Menus["VehicleSaved"] = SavedMenu(savedVehicles, selfPed, &spawnInVehicle); }),*/
					dynmenurow("Customization", "VehicleCustomisation", [&] { for each (std::pair<std::string, Menu> menu in VehicleCustomMenu(selfVehicle)) { Menus[menu.first] = menu.second; } }),
					boolrow("Show Vehicle Speed",			&ShowVehicleSpeed),
					boolrow("God Mode",			&vehicleGodMode),
					boolrow("Invisible",		&vehicleInvisible),
					enterrow("Repair Vehicle",	[&] { RepairVehicle(selfVehicle); }),
					enterrow("Base Upgrades",	[&] { BoostBaseVehicleStats(selfVehicle, true); }),
					enterrow("Max Upgrades",	[&] { MaxUpgrade(selfVehicle); }),
					boolrow("Spawn In Vehicle", &spawnInVehicle),
					boolrow("Base Upgrades",	&baseUpgrades),
					boolrow("Seat Belt",		&vehicleSeatBelt),
					boolrow("Max Upgrades",		&maxUpgrades),
					introw("RPM Multiplier",	&rpmMultipler, 1, 200, 1),
					introw("Torque Multiplier", &torqueMultiplier, 1, 200, 1),
					boolrow("Force Forward",	&vehspeed),
					introw("Forward Speed",		&forwardspeed, 1, 200, 1),
					boolrow("Flying Car",		&flyingcar),
					boolrow("RPG Mode",			&vehrpg),
					boolrow("Lazer Mode",		&vehmini),
					//boolrow("Drive On Water",	&drivingonwater),
					//inputrow("Save Current Vehicle", 16, [&](char* result) { if (PED::IS_PED_IN_ANY_VEHICLE(selfPed, FALSE)) { savedVehicles.push_back(SaveVehicle(result, selfVehicle)); } }),
				}
			}
		},
		{"VehicleSpawn",
			{"Vehicle Spawn", "Vehicle",
				{
					menurow("Airplanes",		"SpawnAirplane"),
					menurow("Bicycles",			"SpawnBicycle"),
					menurow("Boats",			"SpawnBoats"),
					menurow("Commercial",		"SpawnCommercial"),
					menurow("Compacts",			"SpawnCompacts"),
					menurow("Coupes",			"SpawnCoupes"),
					menurow("Emergency",		"SpawnEmergency"),
					menurow("Helicopters",		"SpawnHelicopter"),
					menurow("Industrial",		"SpawnIndustrial"),
					menurow("Motorcycles",		"SpawnMotorcycle"),
					menurow("Military",			"SpawnMilitary"),
					menurow("Muscle",			"SpawnMuscle"),
					menurow("Off-Road",			"SpawnOffroad"),
					menurow("Sedans",			"SpawnSedans"),
					menurow("Service",			"SpawnService"),
					menurow("Sports",			"SpawnSports"),
					menurow("Sports Classics",	"SpawnSportsClassics"),
					menurow("Super",			"SpawnSuper"),
					menurow("SUVs",				"SpawnSUVs"),
					menurow("Utility",			"SpawnUtility"),
					menurow("Vans/Pickups",		"SpawnVansPickups"),
					menurow("Others",			"SpawnOther"),
					inputrow("Input Name", 20, [&] (char* result) { if (DoesVehicleExist(result)) { SpawnPlayerVehicle(selfPed, $(result), spawnInVehicle, baseUpgrades, maxUpgrades); } else { DrawNotification("Invalid Vehicle Name"); } }),
				}
			}
		},
		{"Tunables",
			{"Tunables", "Main",
				{
					/*boolfuncrow("Race tunable",	&enableRace,[] {
						if (enableRace) {
							SetTunable(2407, 9999.0); //WIN_DEATHMATCH_MODIFIER  
							SetTunable(2408, 9999.0); //KILLSTREAK_BONUS_ON_DEATHMATCH_MODIFIER
						}
						else {
							SetTunable(2407, 1.0); //WIN_DEATHMATCH_MODIFIER  
							SetTunable(2408, 1.0); //KILLSTREAK_BONUS_ON_DEATHMATCH_MODIFIER
					}}),
					
					boolfuncrow("Snow",	&enableSnow,[] {
					if (enableSnow) { 
						SetTunable(4857, 1); SetTunable(7204, 0); SetTunable(7211, 10); SetTunable(7212, 10); }
					else { SetTunable(4857, 0); SetTunable(7204, 1); SetTunable(7211, 0); SetTunable(7212, 0);
					}}),
					boolfuncrow("Christmas Content", &christmasContent,	[] {
						if (christmasContent) { SetTunable(4863, 1); SetTunable(7212, 0); SetTunable(7213, 0); SetTunable(7214, 0); } 
						else { SetTunable(4863, 0); SetTunable(7212, 1); SetTunable(7213, 1); SetTunable(7214, 1); }}),
					boolfuncrow("Valentines Content", &valentinesContent, [] {
							if (valentinesContent) { SetTunable(4971, 1); } 
							else { SetTunable(4971, 0); }
					}),
					boolfuncrow("Independence Day Content", &independenceContent,	[] {
								if (independenceContent) { SetTunable(6151, 1); SetTunable(6159, 1); SetTunable(6160, 1); SetTunable(6159, 0);	SetTunable(6155, 0); SetTunable(6158, 0); } 
								else { SetTunable(6146, 0); SetTunable(6159, 0); SetTunable(6160, 0); SetTunable(6154, 1);	SetTunable(6155, 1); SetTunable(6158, 1); }}),
					boolfuncrow("Fireworks Launcher", &fireworksLauncher,		[] {
									if (fireworksLauncher) { SetTunable(7259, 1); SetTunable(7260, 1); SetTunable(7261, 1); SetTunable(5029, 1); } 
									else { SetTunable(7259, 0); SetTunable(7260, 0); SetTunable(7261, 0); SetTunable(5029, 0); }}),*/
					boolfuncrow("Anti Idle Kick", &idlekick, [] {
						if (idlekick) { SetTunable(86, 999999999); SetTunable(87, 999999999); SetTunable(88, 999999999); SetTunable(89, 999999999); DrawNotification("Anti Idle Kick Enabled"); }
						else { SetTunable(86, 120000); SetTunable(87, 300000); SetTunable(88, 600000); SetTunable(89, 900000); DrawNotification("Anti Idle Kick Disabled"); }}),
					boolfuncrow("Anti Vote Kick", &votekick, [] {
						if (votekick) { SetTunable(6, 999); DrawNotification("Anti Vote Kick Enabled"); }
						else { SetTunable(6, 0.66); DrawNotification("Anti Vote Kick Disabled"); }}),
				}
			}
		},
		/*{ "World",
			{"World", "Main",
				{
					
				}
			}
		},
		{"Recovery",
			{"Recovery", "Main",
				{
					boolrow("RP Loop", &rpLoop),
				}
			}
		},*/
		{"Online",
			{"Online", "Main",
				{
				}
			}
		},
		{ "Selected Player",
			{ "Selected Player",  "Online",
				{
					enterrow("Kick From Vehicle",			 [&] { DumpPlayerFromVehicle(currentPed, TRUE); }),
					enterrow("Repair Vehicle",				 [&] { RepairVehicle(currentVehicle); }),
					enterrow("Kill by Explosion",			 [&] { KillPed(currentPed); }),
					boolrow("Explosion LOOP",				 &force2kill),
					enterrow("Give All Weapons",			 [&] { GiveAllWeapons(currentPed); }),
					enterrow("Remove All Weapons",			 [&] { RemoveAllWeapons(currentPed); }),
					enterrow("Teleport to Player",			 [&] { TeleporttoSelectedPlayer(selfPed, currentPed); }),
					enterrow("Teleport to Player's Vehicle", [&] { TeleporttoSelectedPlayerVehicle(selfPed, currentPed); }),
					enterrow("KICK ASS!",					 [&] { KickPlayer(currentPlayer); }),
					boolrow("Freeze Player",				 &oplayerFreeze),
					enterrow("Attach UFO to Player",		 [&] { AttachUFOtoPlayer(currentPed); }),
					enterrow("Crash Player",				 [&] { CrashPlayer(currentPlayer); }),
					enterrow("Tank Spawn",					 [&] { sendArmyAttackers(currentPed); }),
					enterrow("Trap Player",					 [&] { TrapPlayer(currentPed); }),
					enterrow("Justin Troll",				 [&] { AttachRapeClone(currentPed); }),
					enterrow("Teleport Vehicle to Sky",		 [&] { tpveh(currentPed); }),
					enterrow("Delete Player's Vehicle",		 [&] { deletevehicle(currentPed); }),
					enterrow("Clone Player's Vehicle",		 [&] { clonevehicle(currentPed); }),
					enterrow("Hijack Vehicle", [&] { hijack(selfPed, currentPed); }),
					enterrow("Spectate Mode", [&] { specmode(currentPed); }),
				}
			}
		},
		{"All Player",
			{"All Player", "Main",
				{
					enterrow("Kick From Vehicle",	 [] { DumpAllPlayerFromVehicle(TRUE); }),
					enterrow("Kill by Explosion",	 [] { KillAllPed(); }),
					enterrow("Give All Weapons",	 [] { GiveAllWeaponsToAll(); }),
					enterrow("Remove All Weapons",	 [] { RemoveAllPlayerWeapons(); }),
					boolrow("Freeze Player",		 &allfreeze),
					enterrow("Attach UFO to Player", [] { AttachUFOtoAllPlayer(); }),
					enterrow("Justin Troll",		 [] { AttachRapeCloneToAll(); }),
					enterrow("Teleport All Vehicle to Sky", [] { tpallveh(); }),
					boolrow("Explosion LOOP",		 &forceAllkill),
					boolrow("Player Blip",			 &showblip),
					enterrow("Delete All Vehicle",	 [] { deleteallvehicle(); }),
					boolrow("Remove Weapons Loop",	 &removewp),
				}
			}
		},
		{"Misc",
			{"Miscellaneous", "Main",
				{
					/*menurow("Time",	"SetTime"),
					menurow("Weather","SetWeather"),
					menurow("Radio","ToggleRadio"),*/
					enterrow("Reset",		   [] { initialized = false; }),
					enterrow("Clear Peds",	   [&] { GAMEPLAY::CLEAR_AREA_OF_PEDS(playerPos.x, playerPos.y, playerPos.z, 50.0f, -1); }),
					enterrow("Clear Ped Props",[&] { PED::CLEAR_ALL_PED_PROPS(selfPed); }),
					enterrow("Clear Objects",  [&] { GAMEPLAY::CLEAR_AREA_OF_OBJECTS(playerPos.x, playerPos.y, playerPos.z, 50.0f, -1); }),
					enterrow("Clear Vehicles", [&] { GAMEPLAY::CLEAR_AREA_OF_VEHICLES(playerPos.x, playerPos.y, playerPos.z, 50.0f, 0, 0, 0, 0, 0); }),
					enterrow("Clear Projectiles", [&] { GAMEPLAY::CLEAR_AREA_OF_PROJECTILES(playerPos.x, playerPos.y, playerPos.z, 50.0f, -1); }),
					boolrow("Object Hash", &aimobj),
					inputrow("Get Hash", 999,   [](char* result) { DrawNotification(fmt::format("Hash of {0} = {1:#X}", result, $(result))); }),
					enterrow("Dev Test",		[] { TestCode(); }),
					introw("Tunable",			&currTunable, 0, 10000, 1),
					enterrow("Get Tunable Int", [] { DrawNotification(std::to_string(GetTunable<int>(currTunable))); }),
					enterrow("Get Tunable Float", [] { DrawNotification(std::to_string(GetTunable<float>(currTunable))); }),
					enterrow("Set Tunable To 1", [] { SetTunable(currTunable, 1); }),
					enterrow("Set Tunable To 0", [] { SetTunable(currTunable, 0); }),
				}
			}
		},
		/*{ "SetTime",
			{"SetTime", "Misc",
				{
					enterrow("Hour forward",		   [] { TIME::SET_CLOCK_TIME(TIME::GET_CLOCK_HOURS() + 1,TIME::GET_CLOCK_MINUTES(),TIME::GET_CLOCK_SECONDS()); }),
					enterrow("Hour backward",		   [] { TIME::SET_CLOCK_TIME(TIME::GET_CLOCK_HOURS() - 1,TIME::GET_CLOCK_MINUTES(),TIME::GET_CLOCK_SECONDS()); }),
					boolfuncrow("Freeze time", &frezeTime,		[] { TIME::PAUSE_CLOCK(frezeTime);}),
				}
			}
		},
		{ "SetWeather", 
			{ "SetWeather", "Misc",
				{
					dynmenurow("Set weather", "Change Weather", [&] { Menus["Change Weather"] = WeatherMenu(); }),
				}
			}
		},
		{ "ToggleRadio",
			{ "ToggleRadio", "Misc",
				{
					enterrow("Next Track", [] { 
						if (ENTITY::DOES_ENTITY_EXIST( PLAYER::PLAYER_PED_ID() ) ) AUDIO::SKIP_RADIO_FORWARD(); }),
					boolfuncrow("Disable radio", &disableRadio , [] {ToggleRadio(!disableRadio);}),
					boolfuncrow("Enable walk radio", &enableWalkRadio , [] {ToggleWalkRadio(enableWalkRadio);}),
				}
			}
		},*/
	};

	#pragma endregion

	if (ENTITY::DOES_ENTITY_EXIST(selfPed) == TRUE) // Only draw/update the menu if the player actaully exists.
	{
		#pragma region Initialization

		

		if (!initialized) // Put stuff that needs to be generated on startup here.
		{	
			WAIT(0); // The menu might flash, but i feel like it should make refreshing (especially when there are more things to initialize) faster

			DrawNotification("Initializing Menu");

			clock_t elapsed;

			elapsed = clock();

			currMenu = Menus["Main"];

			prevSelections.clear();

			std::vector<std::pair<std::string, Menu>> spawnMenus =
			{
				{ "SpawnCompacts",		 { "Compacts",			"VehicleSpawn", { } } },
				{ "SpawnCoupes",		 { "Coupes",			"VehicleSpawn", { } } },
				{ "SpawnSedans",		 { "Sedans",			"VehicleSpawn", { } } },
				{ "SpawnSUVs",			 { "SUVs",				"VehicleSpawn", { } } },
				{ "SpawnMuscle",		 { "Muscle",			"VehicleSpawn", { } } },
				{ "SpawnSportsClassics", { "Sports Classics",	"VehicleSpawn", { } } },
				{ "SpawnSports",		 { "Sports",			"VehicleSpawn", { } } },
				{ "SpawnSuper",			 { "Super",				"VehicleSpawn", { } } },
				{ "SpawnMotorcycle",	 { "Motorcycles",		"VehicleSpawn", { } } },
				{ "SpawnOffroad",		 { "Off-Road",			"VehicleSpawn", { } } },
				{ "SpawnIndustrial",	 { "Industrial",		"VehicleSpawn", { } } },
				{ "SpawnUtility",		 { "Utility",			"VehicleSpawn", { } } },
				{ "SpawnVansPickups",	 { "Vans/Pickups",		"VehicleSpawn", { } } },
				{ "SpawnBicycle",		 { "Bicycles",			"VehicleSpawn", { } } },
				{ "SpawnBoats",			 { "Boats",				"VehicleSpawn", { } } },
				{ "SpawnHelicopter",	 { "Helicopters",		"VehicleSpawn", { } } },
				{ "SpawnAirplane",		 { "Airplanes",			"VehicleSpawn", { } } },
				{ "SpawnService",		 { "Service",			"VehicleSpawn", { } } },
				{ "SpawnEmergency",		 { "Emergency",			"VehicleSpawn", { } } },
				{ "SpawnMilitary",		 { "Military",			"VehicleSpawn", { } } },
				{ "SpawnCommercial",	 { "Commercial",		"VehicleSpawn", { } } },
				{ "SpawnOther",			 { "Other",				"VehicleSpawn", { } } },
			};

			for each (char* vehicleName in VehicleHashes)
			{
				Hash hash = $(vehicleName);
				char* codeName = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
				char* displayName = UI::_GET_LABEL_TEXT(codeName);
				int vehicleClass = int(min(VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(hash), spawnMenus.size() - 1));

				std::string name = fmt::format("{0}", vehicleName);

				spawnMenus[vehicleClass].second.rows.push_back(enterrow(name, [&, hash] { SpawnPlayerVehicle(selfPed, hash, spawnInVehicle, baseUpgrades, maxUpgrades); }));
			}

			sort(spawnMenus.begin(), spawnMenus.end(), MenuPairSort);

			for each(std::pair<std::string, Menu> menu in spawnMenus)
			{
				sort(menu.second.rows.begin(), menu.second.rows.end(), MenuRowSort);
				Menus.insert(menu);
			}

				spawnInVehicle = true,
				baseUpgrades = true,
				maxUpgrades = true,
				playerGodMode = false,
				ShowVehicleSpeed = false,
				vehicleGodMode = false,
				vehicleSeatBelt = true,
				disableRagdoll = false,
				neverWanted = false,
				infiniteAmmo = false,
				fullAmmo = false,
				oneHitKill = false,
				fireammo = false,
				expammo = false,
				playerInvisible = false,
				oplayerFreeze = false,
				fastSwim = false,
				fastRun = false,
				superjump = false,
				featurePlayerESP = false,
				featurePlayerESPBoxes = false,
				featurePlayerESPBoxes2 = false,
				force2kill = false,
				showcroods = false,
				showtalk = true,
				supermode = false,
				flyingcar = false,
				removegun = false,
				aimobj = false,
				aimwarn = true,
				forcegun = false,
				vehicleInvisible = false,
				idlekick = false,
				votekick = false,
				allfreeze = false,
				rapidfire = false,
				gravitygun = false,
				fireworkgun = false,
				vehrpg = false,
				forceAllkill = false,
				thirdperson = false,
				antiforcekill = false,
				rpmMultipler = 1,
				torqueMultiplier = 1,
				forwardspeed = 1;

			DrawNotification(fmt::format("Finished initializing in {0} miliseconds", clock() - elapsed));

			initialized = true;
		}

		#pragma endregion

		UpdateKeyPresses();

		bool f5JustPressed = IsKeyJustUp(VK_F5);
		bool f6JustPressed = IsKeyJustUp(VK_F6);
		bool f7JustPressed = IsKeyJustUp(VK_F7);
		bool f8JustPressed = IsKeyJustUp(VK_F8);
		bool f9JustPressed = IsKeyJustUp(VK_F9);
		bool f10JustPressed = IsKeyJustUp(VK_F10);
		bool f11JustPressed = IsKeyJustUp(VK_F11);
		bool f12JustPressed = IsKeyJustUp(VK_F12);
		bool addJustPressed = IsKeyJustUp(VK_ADD);
		

		if (f5JustPressed)
		{
			menuVisible = !menuVisible; // If F5 is just pressed, toggle the menu.
			AUDIO::PLAY_SOUND_FRONTEND(-1, "TOGGLE_ON", SoundSet, 1);
		}

		if (f6JustPressed)
		{
			RepairVehicle(selfVehicle);
			DrawNotification("Vehicle Repaired!");
		}

		if (f7JustPressed)
		{
			ClearWantedLevel(selfPlayer);
		}

		if (f8JustPressed)
		{
			fastRun = !fastRun;
			if (fastRun)
			{
				DrawNotification("Fast Run Mode Enabled!");
			}
			else
			{
				DrawNotification("Fast Run Mode Disabled!");
			}
		}

		if (f9JustPressed)
		{
			superjump = !superjump;
			if (superjump)
			{
				DrawNotification("Super Jump Mode Enabled!");
			}
			else
			{
				DrawNotification("Super Jump Mode Disabled!");
			}
		}

		if (f10JustPressed)
		{
			playerInvisible = !playerInvisible;
			if (playerInvisible)
			{
				DrawNotification("Invisibe Mode Enabled!");
			}
			else
			{
				DrawNotification("Invisibe Mode Disabled!");
			}
		}

		if (f11JustPressed)
		{
			AntiCrash();
			AntiCrash2();
			AntiCrash3();
			GAMEPLAY::CLEAR_AREA(playerPos.x, playerPos.y, playerPos.z, 50.0f, true, false, false, false);
			GAMEPLAY::_CLEAR_AREA_2(playerPos.x, playerPos.y, playerPos.z, 50.0f, true, false, false, false);
			DrawNotification("All Entity Cleared");
		}
		

		if (f12JustPressed)
		{
			teleport(true, vector3_t());
		}

		if (addJustPressed)
		{
			RemoveAllPropsFromPlayer(selfPed);
		}

		if (menuVisible)
		{
			bool enterJustPressed = IsKeyJustUp(VK_NUMPAD5),
				 upJustPressed    = IsKeyJustUp(VK_NUMPAD8),
				 downJustPressed  = IsKeyJustUp(VK_NUMPAD2),
				 leftJustPressed  = IsKeyDown(VK_NUMPAD4),
				 rightJustPressed = IsKeyDown(VK_NUMPAD6),
				 backJustPressed  = IsKeyJustUp(VK_NUMPAD0),
				 homeJustPressed  = IsKeyJustUp(VK_DECIMAL);
				 

			#pragma region Play Sounds

			if (enterJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", SoundSet, TRUE);
			if (backJustPressed || homeJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", SoundSet, TRUE);
			if (upJustPressed || downJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", SoundSet, TRUE);
			if (leftJustPressed || rightJustPressed)
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", SoundSet, TRUE);

			#pragma endregion			

			if (currMenu.rows.size() == 0)
			{
				backJustPressed = true;
			}
			if (homeJustPressed)
			{
				prevSelections.clear();
				currMenu = Menus["Main"];				
			}
			if (backJustPressed)
			{
				prevSelections[currMenu.title] = 0;
				currMenu = Menus[currMenu.previous];
			}

			currentSelectionIndex = prevSelections[currMenu.title];

			// Move the current selection up/down depending on the pressed keys.
			if (upJustPressed)
			{
				currentSelectionIndex -= 1;
			}			
			if (downJustPressed)
			{
				currentSelectionIndex += 1;
			}

			int currSize = int(currMenu.rows.size()); // Current Menu Size

			currentSelectionIndex = modulo(currentSelectionIndex, currSize); // Using custom modulo for negative numbers

			prevSelections[currMenu.title] = currentSelectionIndex;

			int end = min(currSize, max(0, currentSelectionIndex - Middle) + MaxRows);
			float start = max(0, end - MaxRows);
			float length = end - start; // Length = start - end

			float drawnY = MenuY;
			
			#pragma region Draw Menu

			DrawMenuLine(currMenu.title, MenuX, drawnY, MenuWidth, TitleHeight, TitleFont, TitleTextScale, TextMargin, true, Center, TitleTextColor, TitleRectColor);		
			drawnY += TitleHeight;

			for (int i = 0; i < length; i++)
			{
				int current = i + start; // Get the index of the row to be drawn

				MenuRow currRow = currMenu.rows[current];

				if (current == currentSelectionIndex)
					DrawMenuLine(strchr(currRow.name), MenuX, drawnY, MenuWidth, RowHeight, ActiveFont, ActiveTextScale, TextMargin, true, Left, ActiveTextColor, ActiveRectColor);
				else
					DrawMenuLine(strchr(currRow.name), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, true, Left, DefaultTextColor, DefaultRectColor);

				switch (currRow.type)
				{
					case TOGGLE:
						if (*currRow.boolptr)
							DrawMenuLine("On", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OnTextColor, DefaultRectColor);
						else
							DrawMenuLine("Off", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OffTextColor, DefaultRectColor);
						break;
					case TOGGLEF:
						if (*currRow.boolptr)
							DrawMenuLine("On", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OnTextColor, DefaultRectColor);
						else
							DrawMenuLine("Off", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, OffTextColor, DefaultRectColor);
						break;
					case INTSLIDER:
						DrawMenuLine(strchr(std::to_string(*currRow.intptr)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, IntTextColor, DefaultRectColor);
						break;
					case INTSLIDERF:
						DrawMenuLine(strchr(std::to_string(*currRow.intptr)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, IntTextColor, DefaultRectColor);
						break;
					case SUBMENU:
						DrawMenuLine(">>", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, MenuTextColor, DefaultRectColor);
						break;
					case DYNAMICSUBMENU:
						DrawMenuLine(">>", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, MenuTextColor, DefaultRectColor);
						break;
					case GETINPUT:
						DrawMenuLine("> _", MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, false, Right, DefaultTextColor, DefaultRectColor);
						break;
					default:
						break;
				}

				drawnY += RowHeight;
			}

			DrawLine(MenuX, drawnY, MenuWidth, LineHeight, TitleRectColor);
			drawnY += LineHeight;

			DrawMenuLine(strchr(fmt::format("{0} - {1}/{2}", XorStr("Husky Mod Menu v1.7ib"), currentSelectionIndex + 1, currSize)), MenuX, drawnY, MenuWidth, RowHeight, DefaultFont, DefaultTextScale, TextMargin, true, Right, DefaultTextColor, DefaultRectColor);
						
			drawnY += RowHeight;

			#pragma endregion

			#pragma region Update Selection

			MenuRow currRow = currMenu.rows[currentSelectionIndex];

			switch (currRow.type)
			{
				case SUBMENU:
					if (enterJustPressed)
					{
						if (Menus[currRow.nextmenu].rows.size() > 0)
						{
							currMenu = Menus[currRow.nextmenu];
						}
					}
					break;
				case DYNAMICSUBMENU:
					if (enterJustPressed)
					{
						currRow.enterFunction();
						if (Menus[currRow.nextmenu].rows.size() > 0)
						{
							currMenu = Menus[currRow.nextmenu];
						}
					}
					break;
				case ENTER:
					if (enterJustPressed)
					{
						currRow.enterFunction();
					}
					break;
				case TOGGLE:
					if (enterJustPressed || leftJustPressed || rightJustPressed)
					{
						*currRow.boolptr ^= true;
					}
					break;
				case TOGGLEF:
					if (enterJustPressed || leftJustPressed || rightJustPressed)
					{
						*currRow.boolptr ^= true;
						currRow.enterFunction();
					}
					break;
				case INTSLIDER:
					if (leftJustPressed)
					{
						*currRow.intptr -= currRow.increment;
					}
					if (rightJustPressed)
					{
						*currRow.intptr += currRow.increment;
					}
					if (*currRow.intptr > currRow.max)
					{
						*currRow.intptr = currRow.min;
					}
					if (*currRow.intptr < currRow.min)
					{
						*currRow.intptr = currRow.max;
					}
					break;
				case INTSLIDERF:
					if (leftJustPressed)
					{
						*currRow.intptr -= currRow.increment;
					}
					if (rightJustPressed)
					{
						*currRow.intptr += currRow.increment;
					}
					if (*currRow.intptr > currRow.max)
					{
						*currRow.intptr = currRow.min;
					}
					if (*currRow.intptr < currRow.min)
					{
						*currRow.intptr = currRow.max;
					}
					if (enterJustPressed || ((leftJustPressed || rightJustPressed) && currRow.autoEnter))
					{
						currRow.enterFunction();
					}
					break;
				case GETINPUT:
					if (enterJustPressed)
					{
						currRow.enterFunction();
					}
					break;
				default:
					break;
			}

			#pragma endregion

			#pragma region Update Functions

			Menus["Online"].rows.clear();

			for (int i = 0; i < 32; i++)
			{
				Player playerNum = IntToPlayer(i);
				int Host = NETWORK::NETWORK_GET_HOST_OF_SCRIPT("Freemode", -1, 0);
				char* playerName = GetPlayerName(i);
				if (!GAMEPLAY::ARE_STRINGS_EQUAL(playerName, "**Invalid**") && Host == playerNum)
				{
					std::string Hoster = std::string(playerName) + "(Most likely HOST)";
					Menus["Online"].rows.push_back(enterrow(Hoster, [=] { currentPlayer = playerNum; Menus["Selected Player"].title = playerName; currMenu = Menus["Selected Player"]; }));
				}
				else if (!GAMEPLAY::ARE_STRINGS_EQUAL(playerName, "**Invalid**") && Host != playerNum)
				{
					Menus["Online"].rows.push_back(enterrow(playerName, [=] { currentPlayer = playerNum; Menus["Selected Player"].title = playerName; currMenu = Menus["Selected Player"]; }));
				}
			}

			Menus["Self"].title = PLAYER::GET_PLAYER_NAME(selfPlayer);

			UpdateEntitySpawns();

			//Self
			ENTITY::SET_ENTITY_INVINCIBLE(selfPed, playerGodMode);
			ENTITY::SET_ENTITY_PROOFS(selfPed, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode, playerGodMode);
			PED::SET_PED_CAN_RAGDOLL(selfPed, !playerGodMode);
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(selfPed, !playerGodMode);
			PLAYER::SPECIAL_ABILITY_FILL_METER(selfPlayer, playerGodMode);
			
			//Self Vehicle
			ENTITY::SET_ENTITY_INVINCIBLE(selfVehicle, vehicleGodMode);
			ENTITY::SET_ENTITY_PROOFS(selfVehicle, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode, vehicleGodMode);
			VEHICLE::SET_VEHICLE_STRONG(selfVehicle, vehicleGodMode); 
			VEHICLE::SET_VEHICLE_CAN_BE_VISIBLY_DAMAGED(selfVehicle, vehicleGodMode); 
			VEHICLE::SET_VEHICLE_CAN_BREAK(selfVehicle, vehicleGodMode); 
			VEHICLE::SET_VEHICLE_ENGINE_CAN_DEGRADE(selfVehicle, vehicleGodMode);

			PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(selfPed, vehicleSeatBelt); // 0 = can, 1 = cant;
			
			if (vehicleSeatBelt)
			{
				PED::SET_PED_CONFIG_FLAG(selfPed, 32, FALSE);
			}

			#pragma endregion
		}

		/*ToggleRadio(!disableRadio);
		ToggleWalkRadio(enableWalkRadio);*/

		if (ShowVehicleSpeed)
		{
			VehicleSpeed(selfVehicle);
		}

		setPlayerVisible(selfPed, playerInvisible);
		setVehicleVisible(selfVehicle, vehicleInvisible);
		
		//Online Player

		if (oplayerFreeze)
		{
			FreezePlayer(currentPed, oplayerFreeze);
			if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(currentPlayer))
			{
				DrawNotification("Freeze Disabled! Player left!");
				oplayerFreeze = false;
			}
		}

		if (allfreeze)
		{
			FreezeAllPlayer(allfreeze);
		}

		if (force2kill)
		{
			ForceKill(currentPed, force2kill);
			if (!NETWORK::NETWORK_IS_PLAYER_CONNECTED(currentPlayer))
			{
				DrawNotification("Explosion LOOP Off! Player left!");
				force2kill = false;
			}
		}

		if (superjump)
		{
			GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(selfPlayer);
		}

		if (fireammo)
		{
			GAMEPLAY::SET_FIRE_AMMO_THIS_FRAME(selfPlayer);
		}

		if (expammo)
		{
			GAMEPLAY::SET_EXPLOSIVE_AMMO_THIS_FRAME(selfPlayer);
		}

		if (forcegun)
		{
			force(selfPlayer, forcegun);
		}

		if (rapidfire)
		{
			Rapid(selfPed, rapidfire);
		}

		if (gravitygun)
		{
			gravity(selfPlayer, gravitygun);
		}

		if (fireworkgun)
		{
			firework(selfPed, fireworkgun);
		}

		if (vehspeed) 
		{
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(selfVehicle, float(forwardspeed));
		}

		if (showcroods)
		{
			Coords(selfPed, showcroods);
		}

		if (showtalk)
		{
			talking(showtalk);
		}

		if (supermode)
		{
			SuperMode(selfPed, supermode);
		}

		if (flyingcar)
		{
			FlyingCarLoop(selfPed, flyingcar);
		}

		setPlayerFastRun(selfPlayer, fastRun);
		setPlayerFastSwim(selfPlayer, fastSwim);
		esp(selfPed, featurePlayerESP, featurePlayerESPBoxes, featurePlayerESPBoxes2);
		OneHitKillEnable(selfPlayer, oneHitKill);

		if (superrun)
		{
			setPlayerSuperRun(selfPed, superrun);
		}

		if (infiniteAmmo)
		{
			InfiniteAmmo();
		}

		if (fullAmmo)
		{
			FullAmmo();
		}

		if (neverWanted)
		{
			NeverWantedLevel(selfPlayer, neverWanted);
		}

		if (aimobj)
		{
			aimobject(selfPlayer, aimobj);
		}

		if (aimwarn)
		{
			aimwarning(selfPed, selfVehicle, aimwarn);
		}

		if (thirdperson)
		{
			CAM::_DISABLE_VEHICLE_FIRST_PERSON_CAM_THIS_FRAME();
			CAM::_DISABLE_FIRST_PERSON_CAM_THIS_FRAME();
			CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(2);
			CAM::SET_FOLLOW_VEHICLE_CAM_VIEW_MODE(2);
		}

		if (vehrpg)
		{
			vehicle_rocket(selfPlayer, vehrpg);
		}

		if (vehmini)
		{
			vehicle_minigun(selfPlayer, vehmini);
		}

		if (forceAllkill)
		{
			KillAllPed2(forceAllkill);
		}

		if (showblip)
		{
			PlayerBlip(showblip);
		}

		if (antiforcekill)
		{
			antikill(selfPed, antiforcekill);
		}

		if (removewp)
		{
			removeallweapon(removewp);
		}

		PED::SET_PED_CAN_RAGDOLL(selfPed, !disableRagdoll);

		VEHICLE::_SET_VEHICLE_ENGINE_TORQUE_MULTIPLIER(selfVehicle, float(torqueMultiplier));
		VEHICLE::_SET_VEHICLE_ENGINE_POWER_MULTIPLIER(selfVehicle, float(rpmMultipler));

		//if (superBunker)
		//{
		//	CAM::SET_CAM_ACTIVE(CAM::GET_RENDERING_CAM(), FALSE);
		//	CAM::SET_CAM_ACTIVE(bunkerCam, TRUE);
		//	CAM::RENDER_SCRIPT_CAMS(TRUE, TRUE, 3000, bunkerCam, FALSE, FALSE);
		//}

		/*if (rpLoop)
		{
			WantedLevelLoop(selfPlayer);
		}

		if (superBunker2)
		{
			STREAMING::_SET_FOCUS_AREA(9999.0f, 9999.0f, -9999.0f, 0.0f, 0.0f, 0.0f);
		}*/

	}
}
