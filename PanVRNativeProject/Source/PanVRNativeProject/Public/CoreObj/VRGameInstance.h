// Fill out your copyright notice in the Description page of Project Settings.

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

public:
#pragma region Getter
	FORCEINLINE UPrisonerManagerSubsystem* GetPrisonerManager() const { return P_PrisonerManager; }
	FORCEINLINE UMapObjManagerSubsystem* GetMapObjManager() const { return P_MapObjManager; }
	FORCEINLINE UVREquipmentWorldSubsystem* GetVREquipmentManager() const { return P_VREquipmentManager; }
	FORCEINLINE bool GetIsGameResultSuccess() const { return bIsGameResultSuccess; }
	FORCEINLINE bool GetIsGameResultFail() const { return bIsGameResultFail; }
	FORCEINLINE bool GetIsFirstLobbyEntryFlag() const { return bIsFirstLobbyEntryFlag; }
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
};
