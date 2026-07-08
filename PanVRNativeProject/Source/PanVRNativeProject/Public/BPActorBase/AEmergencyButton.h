// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AEmergencyButton.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAEmergencyButton : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAEmergencyButton();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> EMButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> SCButtonMoveRail;

	UFUNCTION()
	void EMButtonColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	uint32 bIsHitting = 0;
	FTimerHandle ReturnTimer;

private:
	void OperateAfterReturn();
};
