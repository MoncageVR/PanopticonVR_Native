#include "CoreObj/Manager/VRGameInstance.h"
#include "CoreObj/Manager/GameInstanceSubSystem/VRGameInstanceSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/PrisonerManagerSubsystem.h"
#include "CoreObj/Manager/GameInstanceSubSystem/MapObjManagerSubsystem.h"
#include "CoreObj/Manager/WorldSubSystem/VREquipmentWorldSubsystem.h"

void UVRGameInstance::Init()
{
	Super::Init();

	bIsGameResultSuccess = false;
	bIsGameResultFail = false;
	bIsFirstLobbyEntryFlag = false;

	UVRGameInstanceSubsystem* tempVRGameInstanceSubSystem = GetSubsystem<UVRGameInstanceSubsystem>();

	P_PrisonerManager = GetSubsystem<UPrisonerManagerSubsystem>();
	P_MapObjManager = GetSubsystem<UMapObjManagerSubsystem>();
	P_VREquipmentManager = GetWorld()->GetSubsystem<UVREquipmentWorldSubsystem>();//GetSubsystem<UVREquipmentWorldSubsystem>();

	TapeUnlockArrs.Empty();
	TapeUnlockArrs.Reserve(3);
	TapeUnlockArrs.Add(false);
	TapeUnlockArrs.Add(false);
	TapeUnlockArrs.Add(false);

	CurrTapeType = 10;
	CurrTapeNum = 10;
}

void UVRGameInstance::ClearAllFlag()
{
	bIsGameResultSuccess = false;
	bIsGameResultFail = false;
}