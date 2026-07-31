// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VREquipmentWorldSubsystem.generated.h"

class IIEquipmentInitInterface;

DECLARE_DELEGATE_OneParam(FOnGameStartDelegateVar, bool);
DECLARE_DELEGATE(FOnPunchStartDelegateVar);
DECLARE_DELEGATE(FOnCloseDoorJailDelegateVar);
DECLARE_DELEGATE(FOnLobbyGameStartDelegateVar);

DECLARE_DELEGATE_OneParam(FOnEBOperationControlDelegateVar, uint8);
DECLARE_DELEGATE_OneParam(FOnJailOperationControlByABDelegateVar, FName);

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
	void HandleAddEquipmentArrs(IIEquipmentInitInterface* InActor);

	//void ClearEquipmentArrs();

public:
#pragma region Getter
	FORCEINLINE TArray<IIEquipmentInitInterface*> GetEquipmentArr() const { return EquipmentArrs; }
#pragma endregion

#pragma region Setter

#pragma endregion

public:
#pragma region DELEGATE No Param Variables
	// GTWLever Related GameStart Related Deletage Variable
	FOnGameStartDelegateVar FGameStartSignature;
	FOnPunchStartDelegateVar FPunchStartSignature;
	FOnCloseDoorJailDelegateVar FCloseDoorSignature;
	FOnLobbyGameStartDelegateVar FLobbyGameStartSignature;
#pragma endregion

#pragma region DELEGATE One Param Variables
	FOnEBOperationControlDelegateVar FEBOperateControlSignature;
	FOnJailOperationControlByABDelegateVar FJailOperationControlByABSignature;
#pragma endregion

#pragma region Two Param Variables
	UPROPERTY()
	FOnEBMoveOrderDelegateVar FEBMoveOrderSignature;
#pragma endregion


#pragma region DELEGATEFUNC No Param
	// GameStart Related Deletage Function
	void NotifyGameStartBroadCast();
	void NotifyPunchStartBroadCast();
	void NotifyCloseDoorBroadCast();
	void NotifyGameStartInLobbyBroadCast();
#pragma endregion

#pragma region DELEGATEFUNC One Param
	void NotifyEBOperationControlBroadCast(uint8 InControlFlag);
	void NotifyJailOperationControlByABBroadCast(FName InOperationTargetObjectName);
#pragma endregion

#pragma region DELEGATEFUNC Two Param
	UFUNCTION()
	void NotifyMoveOrderBroadCast(FName InTag, int32 InTargetFloor);
#pragma endregion

private:
	UPROPERTY()
	TArray<IIEquipmentInitInterface*> EquipmentArrs;
};
