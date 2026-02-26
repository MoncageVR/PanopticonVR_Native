// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "ACarKey.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AACarKey : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AACarKey();

	virtual void BeginPlay() override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;

	virtual void OnDropped() override;

	void SetDontGrabPhysics();

protected:
	UFUNCTION()
	void DestroySelf();

private:
	FTimerHandle DestroySelfTimer;
};
