// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "APumpBin.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAPumpBin : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAPumpBin();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMDeLoreanLever;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLRemover;

	UPROPERTY()
	TObjectPtr<USceneComponent> SCMoveStandard;

	UPROPERTY()
	TObjectPtr<class UTextRenderComponent> TRTrashNum;


private:
};
