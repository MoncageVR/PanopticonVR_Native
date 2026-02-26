// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MotionControllerComponent.h"
#include "IGrabInterface.generated.h"

UINTERFACE(MinimalAPI)
class UIGrabInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PANVRNATIVEPROJECT_API IIGrabInterface
{
	GENERATED_BODY()

public:
	FTimerHandle DestroySelfTimer;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) = 0;

	virtual void OnDropped() = 0;
};
