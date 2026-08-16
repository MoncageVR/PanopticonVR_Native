#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VREquipmentWorldSubsystem.generated.h"

class IIEquipmentInitInterface;

DECLARE_DELEGATE(FOnPunchStartDelegateVar);
DECLARE_DELEGATE(FOnCloseDoorJailDelegateVar);
DECLARE_DELEGATE(FOnLobbyGameStartDelegateVar);
DECLARE_DELEGATE(FOnKeyPadArrClearDelegateVar);
DECLARE_DELEGATE(FOnPumpHorseOneStepDelegateVar);

DECLARE_DELEGATE_OneParam(FOnEBOperationControlDelegateVar, uint8);
DECLARE_DELEGATE_OneParam(FOnJailOperationControlByABDelegateVar, FName);
DECLARE_DELEGATE_OneParam(FOnToggleToKeyPadDelegateVar, uint8);
DECLARE_DELEGATE_OneParam(FOnKeyPadToToggleDelegateVar, uint8);
DECLARE_DELEGATE_OneParam(FOnBinToPumpDelegateVar, float);
DECLARE_DELEGATE_OneParam(FOnToiletToTowerDelegateVar, uint8);

DECLARE_DELEGATE_TwoParams(FOnInputValuePassingByKeyPadDelegateVar, bool, uint32);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStartOrderDelegateVar, bool, bIsGameStartFlag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEBMoveOrderDelegateVar, FName, InTag, int32, InTargetFloor);

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API UVREquipmentWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void PostInitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	void HandleAddEquipmentArrs(UObject* InActor);

	//void ClearEquipmentArrs();

public:
#pragma region Getter
	FORCEINLINE TArray<TScriptInterface<IIEquipmentInitInterface>> GetEquipmentArr() const { return EquipmentArrs; }
#pragma endregion

#pragma region Setter

#pragma endregion

public:
#pragma region DELEGATE No Param Variables
	FOnPunchStartDelegateVar FPunchStartSignature;
	FOnCloseDoorJailDelegateVar FCloseDoorSignature;
	FOnLobbyGameStartDelegateVar FLobbyGameStartSignature;
	FOnKeyPadArrClearDelegateVar FKeyPadArrClearSignature;
	FOnPumpHorseOneStepDelegateVar FPumpHorseOneStepSignature;
#pragma endregion

#pragma region DELEGATE One Param Variables
	FOnEBOperationControlDelegateVar FEBOperateControlSignature;
	FOnJailOperationControlByABDelegateVar FJailOperationControlByABSignature;
	FOnInputValuePassingByKeyPadDelegateVar FInputValuePassingByKeyPadSignature;
	FOnToggleToKeyPadDelegateVar FToggleToKeyPadSignature;
	FOnKeyPadToToggleDelegateVar FKeyPadToToggleSignature;
	FOnBinToPumpDelegateVar FBinToPumpSignature;
	FOnToiletToTowerDelegateVar FToiletToTowerSignature;
#pragma endregion

#pragma region MultiCast One Param Variables
	// GTWLever Related GameStart Related Deletage Variable
	UPROPERTY()
	FOnGameStartOrderDelegateVar FGameStartSignature;
#pragma endregion

#pragma region MultiCast Two Param Variables
	UPROPERTY()
	FOnEBMoveOrderDelegateVar FEBMoveOrderSignature;
#pragma endregion

#pragma region DELEGATEFUNC No Param
	void NotifyPunchStartBroadCast();
	void NotifyCloseDoorBroadCast();
	void NotifyGameStartInLobbyBroadCast();
	void NotifyKeyPadArrClearBroadCast();
	void NotifyPumpHorseOneStepBroadCast();
#pragma endregion

#pragma region DELEGATEFUNC One Param
	void NotifyEBOperationControlBroadCast(uint8 InControlFlag);
	void NotifyJailOperationControlByABBroadCast(FName InOperationTargetObjectName);
	void NotifyInputValuePassingByKeyPadBroadCast(bool bIsClearFlag, uint32 InPassingValue);
	void NotifyKeyPadOperationBroadCast(uint8 InBodyMoveFlag);
	void NotifyToggleSwitchOperationBroadCast(uint8 InSwitchFlag);
	void NotifyPumpOperationBroadCast(float InOperTime);
	void NotifyTowerSubdueBroadCast(uint8 InSubdueFlag);
#pragma endregion

#pragma region DELEGATEFUNC MutliCast One Param
	// GameStart Related Deletage Function
	void NotifyGameStartBroadCast(bool bIsGameStartFlag);
#pragma endregion

#pragma region DELEGATEFUNC MutliCast Two Param
	UFUNCTION()
	void NotifyMoveOrderBroadCast(FName InTag, int32 InTargetFloor);
#pragma endregion

private:
	UPROPERTY()
	TArray<TScriptInterface<IIEquipmentInitInterface>> EquipmentArrs;
};
