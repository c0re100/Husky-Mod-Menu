#pragma once

typedef struct
{
	int red;
	int green;
	int blue;
} Color;

typedef struct
{
	char* name;
	Hash hash;
	int wheelType;
	std::vector<std::pair<int, int>> mods;
	std::vector<int> toggles;
	std::vector<std::pair<int, BOOL>> extras;
	char* plateText;
	int plateType;
	int windowType;
	Color tireSmoke;
	std::vector<std::pair<int, BOOL>> neons;
	Color neonColor;
	int primaryColor;
	int secondaryColor;
	int pearlColor;
	int wheelColor;
	BOOL customPrimary;
	BOOL customSecondary;
	Color primaryRGB;
	Color secondaryRGB;
} SavedVehicle;

void BoostBaseVehicleStats(Vehicle vehicle, bool notification);
void DumpPlayerFromVehicle(Ped PlayerID, bool notification);
void SpawnPlayerVehicle(Ped playerPed, Hash vehicleHash, bool spawnInVehicle, bool baseUpgrade, bool maxUpgrade);
/*void SpawnSavedVehicle(Ped playerPed, SavedVehicle savedVehicle, bool spawnInVehicle);
SavedVehicle SaveVehicle(char* name, Vehicle vehicle);
Menu SavedMenu(std::vector<SavedVehicle> savedVehicles, Ped selfPed, bool* spawnInVehicle);*/
void SetPlateType(Vehicle vehicle, PlateTypes plateType);
void SetWindowTint(Vehicle vehicle, WindowTints tint);
void ResprayVehicle(Vehicle vehicle, int primary, int secondary, int pearl, int wheel);
void SetVehiclePrimary(Vehicle vehicle, int color);
void SetVehicleSecondary(Vehicle vehicle, int color);
void SetVehiclePearl(Vehicle vehicle, int color);
void SetVehicleWheelColor(Vehicle vehicle, int color);
void ToggleExtra(Vehicle vehicle, int extraID);
void ClaimVehicle(Vehicle vehicle);
void SetNumberPlate(Vehicle playerVeh, char* text);
void RepairVehicle(Vehicle vehicle);
void SetVehicleModded(Vehicle vehicle);
bool DoesVehicleExist(char* name);
void SetVehicleMod(Vehicle vehicle, int modType, int modIndex);
void ToggleVehicleMod(Vehicle vehicle, int modType);
void SetVehicleToggle(Vehicle vehicle, int modType, BOOL toggle);
void SetVehicleLivery(Vehicle vehicle, int livery);
void SetTireSmoke(Vehicle vehicle, int r, int g, int b);
void ToggleNeonLight(Vehicle vehicle, int index);
void MaxUpgrade(Vehicle vehicle);
std::vector<std::pair<std::string, Menu>> VehicleCustomMenu(Vehicle vehicle);
//void ToggleRadio(bool state);
void VehicleSpeed(Vehicle vehicle);
void FlyingCarLoop(Ped ped, bool state);
void setVehicleVisible(Vehicle vehicle, bool state);
void vehicle_rocket(Player player, bool state);
void vehicle_minigun(Player player, bool state);