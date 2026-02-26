// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Component/UGrabComp.h"
#include "Core/Interface/IGrabInterface.h"
#include "AGTWLever.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAGTWLever : public AVRGrabActorBase, public IIGrabInterface
{
	GENERATED_BODY()

public:
	AAGTWLever();

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

	virtual void EquipmentRegistrable(AActor* InActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> HandleRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Mesh")
	TObjectPtr<class UStaticMeshComponent> SM_Handle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> LeverMovementStandard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components|Collision")
	TObjectPtr<class UCapsuleComponent> HandleCapsule;

	UFUNCTION()
	void OperateLever(); // Execution Function In Lever Movement Timer

	// Lever Rotation In Pitch 170 ~ 180 Degree Check Function
	// 50 ~ 170(50 ~ -80) : Lever Operation Fail
	// 170 ~ 180(-80 ~ -89) : Lever Operation Success
	UFUNCTION()
	bool CheckGTWLeverOpationAngle();

	// Lever Operation Success After 5 Second Counter Function
	UFUNCTION()
	void CountForGameStart();

public:
	virtual void Tick(float DeltaTime) override;

private:
	// Temp Motion Controller Component
	class UMotionControllerComponent* OperatingMCRef;

	// Default Position Local Variables
	FVector GCLocalPos = FVector(7.0f, 6.0f, 26.0f);
	FVector StandardLocalPos = FVector(0.0f, 6.0f, 22.0f);

	// Default Rotation Local Variables
	FRotator HandleLocalRot = FRotator(50.0f, 0.0f, 0.0f);
	FRotator GCLocalRot = FRotator(0.0f, 90.0f, 0.0f);
	FRotator StandardLocalRot = FRotator(0.0f, 0.0f, -90.0f);

	// Lever Operation Related Timer
	FTimerHandle LeverOperateTimer;

	// Lever Operation After 5 Second Related Timer
	FTimerHandle CountGameStartTimer;
	

protected:
	UFUNCTION()
	void OverlapCapsuleBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OverlapCapsuleEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// Capsule Collision In Handing Check Flag Variable
	bool bIsHanding = false;
};
