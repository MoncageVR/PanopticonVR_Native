// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VREquipmentWorldSubsystem.generated.h"

class IIEquipmentInitInterface;

DECLARE_DELEGATE_OneParam(FOnGameStartDelegateVar, bool);
DECLARE_DELEGATE(FOnPunchStartDelegateVar);
DECLARE_DELEGATE(FOnCloseDoorDelegateVar);
DECLARE_DELEGATE(FOnGloveOperateDelegateVar);

DECLARE_DELEGATE_OneParam(FOnEBOperationControlDelegateVar, uint8);

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

	TArray<IIEquipmentInitInterface*> EquipmentArrs;

	void AddEquipmentArrs(IIEquipmentInitInterface* InActor);


public:
#pragma region DELEGATE No Param Variables
	// GTWLever Related GameStart Related Deletage Variable
	FOnGameStartDelegateVar FGameStartSignature;
	FOnPunchStartDelegateVar FPunchStartSignature;
	FOnCloseDoorDelegateVar FCloseDoorSignature;
	FOnGloveOperateDelegateVar FGloveOperationSignature;
#pragma endregion

#pragma region DELEGATE One Param Variables
	FOnEBOperationControlDelegateVar FEBOperateControlSignature;
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
	void NotifyGloveOperateBroadCast();
#pragma endregion

#pragma region DELEGATEFUNC One Param
	void NotifyEBOperationControlBroadCast(uint8 InControlFlag);
#pragma endregion

#pragma region DELEGATEFUNC Two Param
	UFUNCTION()
	void NotifyMoveOrderBroadCast(FName InTag, int32 InTargetFloor);
#pragma endregion


};
