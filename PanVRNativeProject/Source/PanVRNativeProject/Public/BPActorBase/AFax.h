// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRActorBase.h"
#include "AFax.generated.h"

/**
 *
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFax : public AVRActorBase
{
	GENERATED_BODY()

public:
	AAFax();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> FaxSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FaxBodyCol;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> FaxButtonCol;

	UFUNCTION()
	void FaxColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void FaxButtonColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	class AAPaper* AttachingPaper = nullptr;
	float AttachedPaperZPos = 0.f;
	FTimerHandle FaxOperationTimer;

	USoundBase* SFXShredSuccess;
	USoundBase* SFXShredFail;

private:
	void PaperShredding();

};
