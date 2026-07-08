// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AGlove.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGlove : public AVRActorBase
{
	GENERATED_BODY()

public:
	AAGlove();

	virtual void BeginPlay() override;
	virtual void EquipmentRegistrable(AActor * InActor) override;

protected:

#pragma region StaticMeshCompArrs
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UStaticMeshComponent>> GloveStaticMeshes;
#pragma endregion

#pragma region CapsuleCompArrs
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<class UCapsuleComponent>> GloveCollisions;
#pragma endregion

#pragma region Timeline
protected:
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> ForwardMoveTheGloveTimeline;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> BackwardMoveTheGloveTimeline;

	UPROPERTY()
	TObjectPtr<UCurveFloat> MoveTheGloveFloatCurve;

	UFUNCTION()
	void ForwardMoveTheGlovePlayEvent(float Value);

	UFUNCTION()
	void ForwardMoveTheGloveFinishedEvent();

	UFUNCTION()
	void BackwardMoveTheGlovePlayEvent(float Value);

	UFUNCTION()
	void BackwardMoveTheGloveFinishedEvent();

#pragma endregion
	
	UFUNCTION()
	void HandleGloveReceiveByEB(FName InTag, int32 InFloor);

	// Jail ¡æ Glove : Receive Function
	UFUNCTION()
	void HandleGloveReceiveByJail();

private:
	FTimerHandle TempMoveTestTimer; // Debug

	TArray<int32> WorldYaws = { 234, };
	TArray<int32> LocalYaws = { 324, };

	TArray<FVector> GloveStartVecs;
	TArray<FVector> GloveTargetVecs;
	
private:
	// Forward Move Call Function In Use Timeline PlayFromStart
	void MoveTheGloveForward();
	// Backward Move Call Function In Use Timeline PlayFromStart
	void MoveTheGloveBackward();
};
