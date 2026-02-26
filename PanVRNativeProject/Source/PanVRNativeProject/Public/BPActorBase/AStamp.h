// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "AStamp.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAStamp : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAStamp();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

protected:
#pragma region Components
	UPROPERTY()
	TObjectPtr<USceneComponent> STMainRoot;

	UPROPERTY()
	TObjectPtr<class USplineComponent> STMoveRail;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLPaperTarget;

	UPROPERTY()
	TObjectPtr<USceneComponent> STHandleRoot;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMStampHandle;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SMStampHandleHead;

	UPROPERTY()
	TObjectPtr<class UBoxComponent> CLStamp;
#pragma endregion

	UFUNCTION()
	void MoveStamp();

	UFUNCTION()
	void CLPaperTargetOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void CLPaperTargetOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void CLStampOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

private:
	FTimerHandle StampMoveTimer;
	class UMotionControllerComponent* TempMCRef = nullptr;

	class AAPaper* AttachingPaper = nullptr;

private:
	// InFlag 1 = OnGrabbed , InFlag 0 = OnDropped
	void AdjustGCPosNRot(uint32 InFlag);

};
