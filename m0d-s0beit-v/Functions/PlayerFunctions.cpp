#include "../stdafx.h"

//char* radioNames[] = { "RADIO_01_CLASS_ROCK", "RADIO_02_POP", "RADIO_03_HIPHOP_NEW", "RADIO_04_PUNK", "RADIO_05_TALK_01", "RADIO_06_COUNTRY", "RADIO_07_DANCE_01", "RADIO_08_MEXICAN", "RADIO_09_HIPHOP_OLD", "RADIO_12_REGGAE", "RADIO_13_JAZZ", "RADIO_14_DANCE_02", "RADIO_15_MOTOWN", "RADIO_20_THELAB", "RADIO_16_SILVERLAKE", "RADIO_17_FUNK", "RADIO_18_90S_ROCK", "RADIO_19_USER", "RADIO_11_TALK_02", "HIDDEN_RADIO_AMBIENT_TV_BRIGHT", "OFF" };

bool IsPlayerFriend(Player player)
{
	BOOL BplayerFriend = false;
	bool bplayerFriend = false;
	int handle[76]; //var num3 = sub_34009(A_0, (A_1) + 264, (A_1) + 272);
	NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
	if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
	{
		BplayerFriend = NETWORK::NETWORK_IS_FRIEND(&handle[0]);
	}
	if (BplayerFriend = 1)
		bplayerFriend = true;
	else
		bplayerFriend = false;

	return bplayerFriend;
}

void RemoveAllPropsFromPlayer(Ped ped)
{
	Vector3 playerPosition = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
	Hash modelHashes[] = {
		0x2E28CA22 /*p_tram_crash_s*/,			0xA50DDDD0 /*prop_bball_arcade_01*/,
		0xEFC4165A /*prop_food_van_01*/,		0x8E8C7A5B /*prop_crashed_heli*/,
		0x456AA864 /*prop_dj_deck_01*/,			0xBE862050 /*prop_portacabin01*/,
		0xB20E5785 /*prop_sculpt_fix*/,			0x58D3B4EA /*prop_micro_01*/,
		0xC42C019A /*prop_ld_ferris_wheel*/,	0x8AF58425 /*prop_lev_des_barge_01*/,
		0x3DC31836 /*prop_tv_flat_01*/,			0xA9BD0D16 /*prop_coke_block_01*/,
		0x1AFA6A0A /*Prop_weed_01*/,			0x4B3D240F /*prop_wheelchair_01*/,
		0x40F52369 /*p_v_43_safe_s*/,			0xF830B63E /*prop_swiss_ball_01*/,
		0xD541462D /*p_ld_soc_ball_01*/,		0x532B1DD1 /*prop_rub_trolley01a*/,
		0x0E3BA450 /*prop_xmas_tree_int*/,		0xFB631122 /*prop_bumper_car_01*/,
		0x5571173D /*prop_beer_neon_01*/,		0x6AD326C2 /*prop_space_rifle*/,
		0x7FFBC1E2 /*prop_dummy_01*/,			0x678FC2DB /*prop_wheelchair_01_s*/,
		0x5869A8F8 /*prop_large_gold*/,			0xE6CB661E /*PROP_CS_DILDO_01*/,
		0x2AE13DFA /*prop_armchair_01*/,		0x29CB0F3C /*prop_armour_pickup*/,
		0x922C2A43 /*prop_big_shit_01*/,		0xFA686C0E /*prop_bin_04a*/,
		0x1F550C17 /*prop_chair_01a*/,			0x5B5C4263 /*prop_chip_fryer*/,
		0x39885BB5 /*prop_chickencoop_a*/,		0x16A39A90 /*prop_dog_cage_01*/,
		0xE3CE09E2 /*prop_dummy_plane*/,		0x927A5723 /*prop_fan_01*/,
		0x34D5D3FD /*prop_golf_bag_01*/,		0xB467C540 /*p_spinning_anus_s*/,
		0x745F3383 /*prop_windmill_01*/,		0x392D62AA /*prop_gold_cont_01*/,
		0x07121AC4 /*prop_xmas_ext*/,			0x0E8032E4 /*prop_weed_pallet*/,
		0xD44295DD /*p_cablecar_s*/,			0x6F9939C7 /*prop_ld_toilet_01*/,
		0x9C762726 /*prop_lev_des_barge_02*/,	0x8973A868 /*prop_air_bigradar*/,
		0xC2BC19CD /*p_cs_mp_jet_01_s*/,		0x26CF04DB /*garbage bin*/,
		0x6DB9599A /*baskedball */,				0x7DC3908F /*U_M_Y_Justin*/};

	int removed = 0;
	for each (Hash modelHash in modelHashes)
	{
		for (int i = 0; i < 500; i++)
		{
			Object obj = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(playerPosition.x, playerPosition.y, playerPosition.z, 50.0f, modelHash, TRUE, 1, 1);
			BOOL check = OBJECT::DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS(playerPosition.x, playerPosition.y, playerPosition.z, 50.0f, modelHash, 0);
			if (ENTITY::DOES_ENTITY_EXIST(obj) || check)
			{
				RequestControl(obj);
				PED::CLEAR_ALL_PED_PROPS(ped);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(obj);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(obj, TRUE, TRUE);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&obj);
				ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&obj);
				ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&obj);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&obj);
				ENTITY::DETACH_ENTITY(obj, 0, 1);
				ENTITY::DELETE_ENTITY(&obj);
				OBJECT::DELETE_OBJECT(&obj);
				PED::DELETE_PED(&obj);
				GAMEPLAY::CLEAR_AREA_OF_OBJECTS(playerPosition.x, playerPosition.y, playerPosition.z, 50.0f, -1);
				removed++;
			}
			else
			{
				break;
			}
		}
	}

	if (STREAMING::HAS_MODEL_LOADED(0x7DC3908F))
	{
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(0x7DC3908F);
		removed++;
	}

	if (removed > 0)
	{
		DrawNotification("Removed " + std::to_string(removed) + " entities");
	}
}

void RequestControl(Entity e)
{
	NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(e);
}

void ClearWantedLevel(Player player)
{
	PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
	DrawNotification("Wanted level cleared");
}

void NeverWantedLevel(Player player, bool state)
{
	if (state)
	{
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(player);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, FALSE);
		PLAYER::SET_POLICE_IGNORE_PLAYER(player , TRUE);
		PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, TRUE);
		PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, FALSE);
		PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, TRUE);
		PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(player, FALSE);
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("am_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("restrictedareas");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_armybase");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_lossantosintl");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prison");
		GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("re_prisonvanbreak");
	}
	else
	{
		PLAYER::SET_POLICE_IGNORE_PLAYER(player, FALSE);
		PLAYER::SET_EVERYONE_IGNORE_PLAYER(player, FALSE);
		PLAYER::SET_PLAYER_CAN_BE_HASSLED_BY_GANGS(player, TRUE);
		PLAYER::SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS(player, FALSE);
		PLAYER::SET_DISPATCH_COPS_FOR_PLAYER(player, TRUE);
	}
}

void ReplenishPlayer(Ped playerPed)
{
	if (ENTITY::DOES_ENTITY_EXIST(playerPed) & !ENTITY::IS_ENTITY_DEAD(playerPed))
	{
		PED::CLEAR_PED_BLOOD_DAMAGE(playerPed);
		PED::ADD_ARMOUR_TO_PED(playerPed, 100);
		PED::SET_PED_ARMOUR(playerPed, 100);
		PLAYER::SET_PLAYER_MAX_ARMOUR(playerPed, 100);
		PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(playerPed, 333.0f);
		ENTITY::SET_ENTITY_HEALTH(playerPed, 328);
		PLAYER::SET_SPECIAL_ABILITY_MULTIPLIER(333.0f);
		PLAYER::SPECIAL_ABILITY_FILL_METER(playerPed, TRUE);
		PLAYER::SPECIAL_ABILITY_LOCK(playerPed);
	}
}

char* GetPlayerName(int index)
{
	return PLAYER::GET_PLAYER_NAME(index);
}

Player IntToPlayer(int index)
{
	return PLAYER::INT_TO_PLAYERINDEX(index);
}

void EverythingProof(Ped ped)
{
	RequestControl(ped);
	ENTITY::SET_ENTITY_PROOFS(ped, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE);
}

void setPlayerFastSwim(Player player, bool state)
{
	if (state)
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1.49);
	else
		PLAYER::SET_SWIM_MULTIPLIER_FOR_PLAYER(player, 1);
}

void setPlayerFastRun(Player player, bool state)
{
	if (state)
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1.49);
	else
		PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(player, 1);
}

void setPlayerSuperRun(Ped ped, bool state)
{
	if (state)
	{
		if (AI::IS_PED_RUNNING(ped) || AI::IS_PED_SPRINTING(ped) && !PED::IS_PED_RUNNING_RAGDOLL_TASK(ped))
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(ped, true, 0, 1, 0, 0, 0, 0, true, true, true, true, false, true);
		}
	}
	else
	{
	}
}

void setPlayerVisible(Ped playerPed, bool state)
{
	if (state)
	{
		ENTITY::SET_ENTITY_VISIBLE(playerPed, FALSE, 1);
		ENTITY::SET_ENTITY_ALPHA(playerPed, 0, false);
	}
	else
	{
		ENTITY::SET_ENTITY_VISIBLE(playerPed, TRUE, 0);
		ENTITY::SET_ENTITY_ALPHA(playerPed, 255, false);
	}
}

void esp(Ped ped,bool featurePlayerESP,bool featurePlayerESPBoxes, bool featurePlayerESPBoxes2)
{
	// player esp
	if (featurePlayerESP)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			Vehicle currentOnlineVeh = PED::GET_VEHICLE_PED_IS_USING(playerOnline);
			Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
			if (playerOnline == ped) continue;
			char* name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(index));
			//if (!strcmp(name, "c0re100")) continue;
			int health = ENTITY::GET_ENTITY_HEALTH(playerOnline);
			float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerCoords.x, playerCoords.y, playerCoords.z, playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z, TRUE);
			int armor = PED::GET_PED_ARMOUR(playerOnline);
			char text[250];
			if (distance < 5000.0f && bPlayerOnlineExists)
			{
				float xa;
				float ya;
				BOOL screenCoords = GRAPHICS::_WORLD3D_TO_SCREEN2D(playerOnlineCoords.x, playerOnlineCoords.y, playerOnlineCoords.z, &xa, &ya);
				if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
				{
					sprintf_s(text, "%s ~y~%.02f\n~g~%d ~b~%d", name, distance, health, armor);
					UI::SET_TEXT_FONT(0);
					UI::SET_TEXT_OUTLINE();
					UI::SET_TEXT_SCALE(0.23f, 0.23f);
					UI::SET_TEXT_COLOUR(0, 255, 0, 255);
					UI::SET_TEXT_WRAP(0.0f, 1.0f);
					UI::SET_TEXT_CENTRE(TRUE);
					UI::_SET_TEXT_ENTRY("STRING");
					UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
				}
				else
				{
					sprintf_s(text, "%s\n ~y~%.02f", name, distance);
					UI::SET_TEXT_FONT(0);
					UI::SET_TEXT_OUTLINE();
					UI::SET_TEXT_SCALE(0.15f, 0.15f);
					UI::SET_TEXT_COLOUR(255, 0, 0, 75);
					UI::SET_TEXT_WRAP(0.0f, 1.0f);
					UI::SET_TEXT_CENTRE(TRUE);
					UI::_SET_TEXT_ENTRY("STRING");
					UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text);
				}
				UI::_DRAW_TEXT(xa, ya);
			}
		}
	}

	// 3D esp boxes
	if (featurePlayerESPBoxes)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			char* name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(index));
			//if (!strcmp(name, "c0re100")) continue;
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 0, 255, 0, 255);
			}
			else
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y - 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z - 0.75f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y + 0.5f, playerOnlineCoords.z + 0.75f, 255, 0, 0, 75);
			}
		}
	}

	// 2D esp boxes
	if (featurePlayerESPBoxes2)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			char* name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(index));
			//if (!strcmp(name, "c0re100")) continue;
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 100) && bPlayerOnlineExists)
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, 0, 255, 0, 255);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, 0, 255, 0, 255);
			}
			else
			{
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, 0, 255, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, 0, 255, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, playerOnlineCoords.x + 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, 0, 255, 0, 75);
				GRAPHICS::DRAW_LINE(playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z - 0.7f, playerOnlineCoords.x - 0.5f, playerOnlineCoords.y, playerOnlineCoords.z + 0.7f, 0, 255, 0, 75);
			}
		}
	}

	/*if (featurePlayerESPTrace)
	{
		for (int index = 0; index < 32; index++)
		{
			Player playerOnline = PLAYER::GET_PLAYER_PED(index);
			BOOL bPlayerOnlineExists = ENTITY::DOES_ENTITY_EXIST(playerOnline);
			Vector3 playerOnlineCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
			if (ENTITY::HAS_ENTITY_CLEAR_LOS_TO_ENTITY(ped, playerOnline, 17) && bPlayerOnlineExists)
			{
				GRAPHICS::DRAW_LINE(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).x, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).y, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).z, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).x, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).y, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).z, 0, 255, 0, 255);
			}
			else
			{
				GRAPHICS::DRAW_LINE(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).x, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).y, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::GET_PLAYER_PED(index), 0.0f, 0.0f, 0.0f).z, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).x, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).y, ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0f, 0.0f, 0.0f).z, 255, 0, 0, 75);
			}
		}
	}*/
}

void TeleportToForward()
{
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	Vector3 coords = ENTITY::GET_ENTITY_COORDS(e, FALSE);
	Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(e);
	RequestControl(e);
	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x + forward.x/2, coords.y + forward.y/2, coords.z, 0, 0, 1);
	DrawNotification("Teleported!");
}

void teleport(bool marker, vector3_t tpCoords)
{
	// get entity to teleport
	Entity e = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(e, 0))
		e = PED::GET_VEHICLE_PED_IS_USING(e);
	// get coords
	Vector3 coords;
	bool success = false;
	if (marker) // marker
	{
		bool blipFound = false;
		// search for marker blip

		int blipIterator = UI::_GET_BLIP_INFO_ID_ITERATOR();
		for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(blipIterator); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(blipIterator))
		{
			if (UI::GET_BLIP_INFO_ID_TYPE(i) == 4)
			{
				coords = UI::GET_BLIP_INFO_ID_COORD(i);
				blipFound = true;
				break;
			}
		}
		if (blipFound)
		{
			// load needed map region and check height levels for ground existence
			bool groundFound = false;
			static float groundCheckHeight[] = {
				100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
				450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
			};
			for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, groundCheckHeight[i], 0, 0, 1);
				WAIT(100);
				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, groundCheckHeight[i], &coords.z, 0))
				{
					groundFound = true;
					coords.z += 3.0;
					break;
				}
			}
			// if ground not found then set Z in air and give player a parachute
			if (!groundFound)
			{
				coords.z = 1000.0;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
			}
			success = true;
		}
		else
		{
			DrawNotification("Map marker isn't set");
		}

	}
	else // predefined coords
	{
		coords.x = tpCoords.x;
		coords.y = tpCoords.y;
		coords.z = tpCoords.z;
		success = true;
	}
	// set player pos
	if (success)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(e, coords.x, coords.y, coords.z, 0, 0, 1);
		WAIT(0);
		DrawNotification("Teleported!");
	}
}

Menu TpLocationsMenu()
{
	Menu savedMenu =
	{ "Locations", "Teleport",
	{

	}
	};

	for each (std::pair<char*, vector3_t> loc in locations)
	{
		savedMenu.rows.push_back(enterrow(fmt::format("{0}", loc.first), [=] { teleport(false, loc.second); }));
	}

	return savedMenu;
}

void Coords(Ped ped, bool state)
{
	if (state)
	{
		char text[250];
		Vector3 CurrentCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
		sprintf_s(text, "~r~X: %.02f ~g~, Y: %.02f ~b~, Z: %.02f", CurrentCoords.x, CurrentCoords.y, CurrentCoords.z);
		static color_t coords = { 0, 0, 255, 255 };
		DrawMsgUI(text, 0.15f, 0.75f, 0.3f, coords);
	}
	else
	{
	}
}

void talking(bool state)
{
	if (state)
	{
		static color_t hash = { 0, 255, 0, 255 };
		static color_t get = { 255, 0, 0, 255 };
		DrawMsgUI("Show Who's Talking:", 0.15f, 0.40f, 0.3f, hash);

		float pos = 0.40f;
		for (int i = 0; i < 32; i++)
		{
			if (NETWORK::NETWORK_IS_PLAYER_TALKING(i) == true)
			{
				char text[250];
				char* Talking = GetPlayerName(i);
				sprintf_s(text, "%s", Talking);
				DrawMsgUI(text, 0.15f, (pos += 0.02), 0.3f, get);
			}
		}
	}
	else
	{
	}
}

void ApplyForceToEntity(Ped ped, int x, int y, int z)
{
	ENTITY::APPLY_FORCE_TO_ENTITY(ped, 1, 0, x, y, z, 0, 0, 0, 1, 1, 1, 0, 1);
}

void SuperMode(Ped ped, bool state)
{
	if (state)
	{
		ENTITY::SET_ENTITY_INVINCIBLE(ped, true);
		PED::SET_PED_TO_RAGDOLL_WITH_FALL(ped, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
		GAMEPLAY::SET_SUPER_JUMP_THIS_FRAME(PLAYER::PLAYER_ID());

		if (ENTITY::IS_ENTITY_IN_AIR(ped) && !PED::IS_PED_RAGDOLL(ped))
		{
			if (GetAsyncKeyState(0x57)) // W key
			{
				ApplyForceToEntity(ped, 3, 0, 0);
			}

			if (GetAsyncKeyState(0x53)) // S key
			{
				ApplyForceToEntity(ped, 3, 6, 0);
			}
			if (GetAsyncKeyState(VK_SHIFT))
			{
				ApplyForceToEntity(ped, 6, 0, 0);
			}
		}
	}
	else
	{
		ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
	}
}

void aimobject(Player player, bool state)
{
	if (state)
	{
		Entity objTarget;

		static color_t hash = { 0, 255, 0, 255 };
		DrawMsgUI("Object Hash:", 0.15f, 0.13f, 0.3f, hash);

		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &objTarget))
		{
			Hash model = ENTITY::GET_ENTITY_MODEL(objTarget);
			char _hex[32];
			sprintf_s(_hex, "%X", model);
			DrawMsgUI(_hex, 0.15f, 0.15f, 0.3f, hash);
		}
	}
	else
	{
	}
}

void aimwarning(Ped ped, Vehicle vehicle, bool state)
{
	if (state)
	{
		static color_t aimmsg = { 0, 255, 0, 255 };
		static color_t aimname = { 0, 255, 0, 255 };
		DrawMsgUI("Aiming Warning:", 0.45f, 0.15f, 0.3f, aimmsg);
		
		float wpos = 0.15f;
		for (int i = 0; i < 32; i++)
		{
			if (PLAYER::IS_PLAYER_TARGETTING_ENTITY(i, ped) || PLAYER::IS_PLAYER_TARGETTING_ENTITY(i, vehicle))
			{
				Player playerOnline = PLAYER::GET_PLAYER_PED(i);
				Vector3 PlayerCoords = ENTITY::GET_ENTITY_COORDS(playerOnline, FALSE);
				Vector3 myCoords = ENTITY::GET_ENTITY_COORDS(ped, FALSE);
				float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(myCoords.x, myCoords.y, myCoords.z, PlayerCoords.x, PlayerCoords.y, PlayerCoords.z, TRUE);
				char* name = PLAYER::GET_PLAYER_NAME(PLAYER::INT_TO_PLAYERINDEX(i));
				char list[250];
				sprintf_s(list, "~r~%s - ~y~%.02fM", name, distance);
				DrawMsgUI(list, 0.45f, (wpos += 0.02), 0.3f, aimname);
				wchar_t* play = L"Play boom.mp3";
				mciSendString(play, NULL, 0, NULL);
			}
		}
	}
	else
	{
	}
}

void offtheradar()
{
	GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("freemode");
	//GAMEPLAY::TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME("freemode_init");
}

void antikill(Ped ped, bool state)
{
	if (state)
	{
		Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(ped, TRUE);
		float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerPos.x, playerPos.y, playerPos.z, -1813.999f, -3268.999f, 6.999f, TRUE);
		
		if (distance >= 20000.0f || playerPos.x <= -8000.0f || playerPos.y <= -8000.0f || playerPos.z >= 2600.0f || playerPos.x >= 8000.0f || playerPos.y >= 9000.0f)
		{
			if (distance >= 20000.0f)
			{
				DrawNotification("Distance is greater than 20000");
			}
			else if (playerPos.x <= -8000.0f)
			{
				DrawNotification("X is less than -8000");
			}
			else if (playerPos.y <= -8000.0f)
			{
				DrawNotification("Y is less than -8000");
			}
			else if (playerPos.z >= 2600.0f)
			{
				DrawNotification("Z is greater than 2600");
			}
			else if (playerPos.x >= 8000.0f)
			{
				DrawNotification("X is greater than 8000");
			}
			else if (playerPos.y >= 9000.0f)
			{
				DrawNotification("Y is greater than 9000");
			}

			if (PED::IS_PED_IN_ANY_VEHICLE(ped, FALSE))
			{
				Vehicle myVehicle = PED::GET_VEHICLE_PED_IS_IN(ped, FALSE);
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(myVehicle, 180.000f, -3320.000f, 6.000f, 0, 0, 1);
				DrawNotification("Out Of Area Detected!");
			}
			else
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ped, 180.000f, -3320.000f, 6.000f, 0, 0, 1);
				DrawNotification("Out Of Area Detected!");
			}
		}
	}
	else
	{
	}
}

void TeleporttoMissionObjective()
{
	static Vector3 v3BlipCoords, v3OldLocation;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Entity ent = PLAYER::PLAYER_PED_ID();
	if (PED::IS_PED_IN_ANY_VEHICLE(ent, FALSE)) ent = PED::GET_VEHICLE_PED_IS_USING(ent);
	bool bBlipFound = false, bSuccess = false;

	if (bBlipFound == false)
	{
		
		for (Blip j = UI::GET_FIRST_BLIP_INFO_ID(BLIP_RACEFLAG); UI::DOES_BLIP_EXIST(j) != 0; j = UI::GET_NEXT_BLIP_INFO_ID(BLIP_RACEFLAG))
		{
			if (UI::GET_BLIP_COLOUR(j) == BLIPCOLOR_NONE)
			{
				v3BlipCoords = UI::GET_BLIP_COORDS(j);
				bBlipFound = true;
				v3OldLocation = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
				break;
			}
		}

		for (Blip i = UI::GET_FIRST_BLIP_INFO_ID(BLIP_CIRCLE); UI::DOES_BLIP_EXIST(i) != 0; i = UI::GET_NEXT_BLIP_INFO_ID(BLIP_CIRCLE))
		{
			if (UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_MISSION || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_YELLOWMISSION || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_YELLOWMISSION2 || UI::GET_BLIP_COLOUR(i) == BLIPCOLOR_GREEN)
			{
				v3BlipCoords = UI::GET_BLIP_COORDS(i);
				bBlipFound = true;
				v3OldLocation = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
				break;
			}
		}

		for (Blip k = UI::GET_FIRST_BLIP_INFO_ID(BLIP_DOLLARSIGN); UI::DOES_BLIP_EXIST(k) != 0; k = UI::GET_NEXT_BLIP_INFO_ID(BLIP_DOLLARSIGN))
		{
			if (UI::GET_BLIP_COLOUR(k) == BLIPCOLOR_YELLOWMISSION2)
			{
				v3BlipCoords = UI::GET_BLIP_COORDS(k);
				bBlipFound = true;
				v3OldLocation = ENTITY::GET_ENTITY_COORDS(ent, FALSE);
				break;
			}
		}

		if (bBlipFound)
		{
			bool bGroundFound = false;
			static float fGroundCheckHeight[] = { 100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0, 450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0 };
			for (int i = 0; i < sizeof(fGroundCheckHeight) / sizeof(float); i++)
			{
				ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, v3BlipCoords.x, v3BlipCoords.y, fGroundCheckHeight[i], FALSE, FALSE, TRUE);
				WAIT(100);
				if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(v3BlipCoords.x, v3BlipCoords.y, fGroundCheckHeight[i], &v3BlipCoords.z, 0))
				{
					bGroundFound = true;
					v3BlipCoords.z += 3.0f;
					break;
				}
			}
			if (!bGroundFound)
			{
				v3BlipCoords.z = 1000.0f;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, 0xFBAB5776, 1, 0);
			}
			bSuccess = true;
		}
		else
		{
			DrawNotification("Map marker isn't set!");
		}
	}
	else
	{
		v3BlipCoords.x = v3OldLocation.x;
		v3BlipCoords.y = v3OldLocation.y;
		v3BlipCoords.z = v3OldLocation.z;
		DrawNotification("Teleporting to old location!");
		bSuccess = true;
	}

	if (bSuccess)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(ent, v3BlipCoords.x, v3BlipCoords.y, v3BlipCoords.z, FALSE, FALSE, TRUE);
		WAIT(0);
		DrawNotification("Teleported!");
		return;
	}
}