// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "ALighter.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AALighter : public AVRGrabActorBase , public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AALighter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

protected:
#pragma region Components
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> LHead;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CLFire;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAudioComponent> LSCAudioPlayer;

#pragma endregion

	UFUNCTION()
	void AdjustGrabTransform();

private:
	UMotionControllerComponent* TempMCRef;
	USoundBase* SCSfxLighterCues;
	AActor* DefaultParentActor;
	FTransform DefaultTransform;

private:
	void LighterHeadMovement(uint32 TempDirection);

};
