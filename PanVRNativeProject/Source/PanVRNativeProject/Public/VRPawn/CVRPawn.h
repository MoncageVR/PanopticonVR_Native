// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRPawn/VRCharacterBase.h"
#include "CVRPawn.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API ACVRPawn : public AVRCharacterBase
{
	GENERATED_BODY()

public:
	ACVRPawn();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerMovingUpAndDownInStage(uint8 InDir);

	UFUNCTION()
	void HandleMovePlayerToFloor(FName InTag, int32 InTargetFloor);

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ChairPlatform;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ChairBody;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ChairTowerHead;

	UPROPERTY()
	class UInputMappingContext* IMC_Default;

	UPROPERTY()
	class UInputMappingContext* IMC_Hands;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> VRPawnMovementTimeline;

protected:
	UFUNCTION()
	void VRPawnMoveUpTLFunc(float Value);

	UFUNCTION()
	void VRPawnMoveUpTLEndFunc();

private:
	TSubclassOf<class AVRHand> LeftHandBPClass;
	TSubclassOf<class AVRHand> RightHandBPClass;

	UCurveFloat* VRPawnMoveUpCurve;

	int32 CurrFloorNum;
	int32 PressedFloorNum;
	TArray<float> TargetPlayerHeights;

private:
	void SpawnHands();
	void InitFloorData();
};
