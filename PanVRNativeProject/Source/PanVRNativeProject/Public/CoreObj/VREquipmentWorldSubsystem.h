// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VREquipmentWorldSubsystem.generated.h"

class IIEquipmentInitInterface;

DECLARE_DELEGATE_OneParam(FGTWGameStartSignature, bool);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEBMoveOrderSignature, FName, InTag, int32, InTargetFloor);



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
#pragma region DELEGATE

	// GTWLever Related GameStart Related Deletage Variable
	FGTWGameStartSignature OnGameStart;

	UPROPERTY(BlueprintAssignable)
	FEBMoveOrderSignature OnEBMoveOrder;

#pragma endregion

#pragma region DELEGATEFUNC

	// GameStart Related Deletage Function
	void OnGameStartBroadCast();

	UFUNCTION()
	void OnMoveByEBBroadCast(FName InTag, int32 InTargetFloor);

#pragma endregion


};
