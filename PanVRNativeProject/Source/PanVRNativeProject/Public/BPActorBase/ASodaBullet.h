// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "ASodaBullet.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AASodaBullet : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AASodaBullet(); // Constructor

	void SetActorPhysics(bool PhysicsFlag);

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;

	virtual void OnDropped() override;

	virtual void Tick(float DeltaTime) override;

	void CurveMove(FVector InDir);

#pragma region Setter
	void SetSBIsAttaching(uint32 TempFlag);
#pragma endregion

#pragma region Getter
	uint32 GetSBIsAttaching() const;
#pragma endregion

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UBoxComponent> CLBox;

	UFUNCTION()
	void DestroySelf();

private:
	// SodaBullet Not Using Check Timer
	FTimerHandle DestroySelfTimer;

	uint32 bIsSBAttaching = 0;

private:
	
};
