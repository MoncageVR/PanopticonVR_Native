// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AABButton.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAABButton : public AVRActorBase 
{
	GENERATED_BODY()

public:
	// Constructor
	AAABButton();
	// Called when the game starts or when the actor is spawned.
	virtual void BeginPlay() override;
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ButtonA;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLButtonA;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ButtonB;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLButtonB;

protected:
	// Begin Overlap For Collision Component Button A
	UFUNCTION()
	void OverlapABoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	// End Overlap For Collision Component Button A
	UFUNCTION()
	void OverlapABoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
	// Begin Overlap For Collision Component Button B
	UFUNCTION()
	void OverlapBBoxBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	// End Overlap For Collision Component Button B
	UFUNCTION()
	void OverlapBBoxEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

private:
	TArray<uint8> AnswerSaveList;
	FTimerHandle NextAnswerCheckTimer;
	FTimerHandle CooldownApplyTimer;
	FString CorrectAnswerFirstCase = TEXT("1010");
	FString CorrectAnswerSecondCase = TEXT("1001");
	uint8 bIsButtonAPressed = 0;
	uint8 bIsButtonBPressed = 0;
	
	UPROPERTY()
	USoundBase* AButtonSFX;
	UPROPERTY()
	USoundBase* BButtonSFX;

private:
	// Actually Static Mesh Button Moving Part Function
	void ButtonMovement(UStaticMeshComponent* InActuallyMoveSMButton, FVector InTargetPos);
	// If there is no next Input, Clear the input Array
	void NextAnswerNonInputClear();
	// Answer Check Main Logic
	void CheckCorrectAnswer();
	// Cooldown Application Function
	void CoolDownApplyFunc(uint8 InCoolTimeFlag);
	// Cooldown removal Function
	void CoolDownUnApplyFunc();
};
