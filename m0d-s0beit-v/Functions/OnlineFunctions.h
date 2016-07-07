#pragma once
#include "../stdafx.h"

void TeleporttoSelectedPlayerVehicle(Ped playerPed, Ped selectedPed);
void TeleporttoSelectedPlayer(Ped playerPed, Ped selectedPed);
void KickPlayer(Player player);
void KillPed(Ped ped);
void FreezePlayer(Ped ped, bool state);
void AttachUFOtoPlayer(Ped selectedPed);
void ForceKill(Ped ped, bool state);
void CrashPlayer(Player player);
void sendArmyAttackers(Ped ped);
void TrapPlayer(Ped ped);
void AttachRapeClone(Ped selectedPed);
void specmode(Ped ped);
void hijack(Ped playerPed, Ped selectedPed);
void tpveh(Ped selectedPed);
void deletevehicle(Ped selectedPed);
void clonevehicle(Ped selectedPed);

//All Player Options
void KillAllPed();
void FreezeAllPlayer(bool state);
void AttachUFOtoAllPlayer();
void AttachRapeCloneToAll();
void GiveAllWeaponsToAll();
void RemoveAllPlayerWeapons();
void DumpAllPlayerFromVehicle(bool notification);
void tpallveh();
void KillAllPed2(bool state);
void PlayerBlip(bool state);
void deleteallvehicle();
void removeallweapon(bool state);