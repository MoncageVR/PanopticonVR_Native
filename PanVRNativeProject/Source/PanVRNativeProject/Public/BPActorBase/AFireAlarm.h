// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AFireAlarm.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFireAlarm : public AVRActorBase
{
	GENERATED_BODY()
	
public:
	AAFireAlarm();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltatTimes) override;

#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> FAFirstArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> FASecondArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> CLFAFireCheck;
#pragma endregion

	UFUNCTION()
	void FireCheckColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void FireCheckColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void SprinklerOperation();

	UFUNCTION()
	void SprinklerCoolDown();

	UFUNCTION()
	void SprinklerCoolDownEnd();

private:
	FTimerHandle SprinklerOperateTimer;
	FTimerHandle SprinklerCooldownTimer;

	USoundBase* SFXFireAlarm;
};
