#pragma once
#include "../stdafx.h"

bool IsPlayerFriend(Player player);
void RemoveAllPropsFromPlayer(Ped playerPed);
void ClearWantedLevel(Player player);
void NeverWantedLevel(Player player, bool state);
void ReplenishPlayer(Ped playerPed);
void RequestControl(Entity e);
//void SpawnPedMoney(Ped selectedPed);
char* GetPlayerName(int index);
Player IntToPlayer(int index);
void EverythingProof(Ped ped);
//std::vector<std::pair<std::string, Menu>> ClothingMenus(Ped playerPed);
void setPlayerVisible(Ped playerPed, bool state);
void setPlayerFastSwim(Player player, bool state);
void setPlayerFastRun(Player player, bool state);
void setPlayerSuperRun(Ped ped, bool state);
void esp(Ped ped, bool featurePlayerESP, bool featurePlayerESPBoxes, bool featurePlayerESPBoxes2);
void TeleportToForward();
void teleport(bool marker, vector3_t tpCoords);
Menu TpLocationsMenu();
void Coords(Ped ped, bool state);
void talking(bool state);
void ApplyForceToEntity(Ped ped, int x, int y, int z);
void SuperMode(Ped ped, bool state);
void aimobject(Player player, bool state);
void aimwarning(Ped ped, Vehicle vehicle, bool state);
void offtheradar();
void antikill(Ped ped, bool state);
void TeleporttoMissionObjective();