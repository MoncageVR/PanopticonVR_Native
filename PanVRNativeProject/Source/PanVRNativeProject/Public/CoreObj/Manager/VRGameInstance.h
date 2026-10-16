#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VRGameInstance.generated.h"

class UVRGameInstanceSubsystem;
class UPrisonerManagerSubsystem;
class UMapObjManagerSubsystem;
class UVREquipmentWorldSubsystem;

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	
	UFUNCTION()
	void ClearAllFlag();

public:
#pragma region Getter
	FORCEINLINE UPrisonerManagerSubsystem* GetPrisonerManager() const { return P_PrisonerManager; }
	FORCEINLINE UMapObjManagerSubsystem* GetMapObjManager() const { return P_MapObjManager; }
	FORCEINLINE UVREquipmentWorldSubsystem* GetVREquipmentManager() const { return P_VREquipmentManager; }
	FORCEINLINE bool GetIsGameResultSuccess() const { return bIsGameResultSuccess; }
	FORCEINLINE bool GetIsGameResultFail() const { return bIsGameResultFail; }
	FORCEINLINE bool GetIsFirstLobbyEntryFlag() const { return bIsFirstLobbyEntryFlag; }
	FORCEINLINE TArray<bool> GetTapeUnlockFlagArr() const { return TapeUnlockArrs; }
	FORCEINLINE uint8 GetCurrTapeType() const { return  CurrTapeType; }
	FORCEINLINE uint32 GetCurrTapeNum() const { return  CurrTapeNum; }
#pragma endregion

#pragma region Setter
	FORCEINLINE void SetIsGameResultSuccess(bool bInSuccessFlag) { bIsGameResultSuccess = bInSuccessFlag; }
	FORCEINLINE void SetIsGameResultFail(bool bInFailFlag) { bIsGameResultFail = bInFailFlag; }
	FORCEINLINE void SetIsFirstLobbyEntryFlag(bool bIsFirstEntry) { bIsFirstLobbyEntryFlag = bIsFirstEntry; }
	FORCEINLINE void SetTapeUnlockNum(int32 InUnlockNumIndex) { TapeUnlockArrs[InUnlockNumIndex] = true; }
	FORCEINLINE void SetCurrTapeType(uint8 InTypeNum) { CurrTapeType = InTypeNum; }
	FORCEINLINE void SetCurrTapeNum(uint32 InNum) { CurrTapeNum = InNum; }
	FORCEINLINE void ClearCurrTapeRelatedInfo()
	{
		CurrTapeType = 0; // DebugValue : 0 , Defautl : 10
		CurrTapeNum = 0; // DebugValue : 0 , Defautl : 10
	}
#pragma endregion

private:
	UPROPERTY()
	class UPrisonerManagerSubsystem* P_PrisonerManager;
	UPROPERTY()
	class UMapObjManagerSubsystem* P_MapObjManager;
	UPROPERTY()
	class UVREquipmentWorldSubsystem* P_VREquipmentManager;

private:
	bool bIsGameResultSuccess;
	bool bIsGameResultFail;
	bool bIsFirstLobbyEntryFlag;

	TArray<bool> TapeUnlockArrs;

	uint8 CurrTapeType;
	uint32 CurrTapeNum;
};
