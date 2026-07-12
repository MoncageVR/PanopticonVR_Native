// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/VRGrabActorBase.h"
#include "Core/Interface/IGrabInterface.h"
#include "AFuelTank.generated.h"

/**
 * 
 */
UCLASS()
class PANVRNATIVEPROJECT_API AAFuelTank : public AVRGrabActorBase , public IIGrabInterface
{
	GENERATED_BODY()
	
public:
	AAFuelTank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTimes) override;

	// Actor On Grabbed
	virtual void OnGrabbed(UMotionControllerComponent& InMCRef, const FVector& HandGrabPos, class AVRHand* InGrabbingHand) override;
	virtual void OnDropped() override;

#pragma region Components
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTMainRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTMeshRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> FTAttachRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> FTMainBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> FTCLRod;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> FTCLGrab;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USplineComponent> FTMoveRoute;
#pragma endregion

#pragma region CollisionFunctions
protected:
	// FuelTank Movement For Overlap Check Functions
	UFUNCTION()
	void GrabColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void GrabColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	// FuelRod Attaching Check Functions
	UFUNCTION()
	void FuelRodColOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void FuelRodColOverlapEnd(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
#pragma endregion

protected:
	UFUNCTION()
	void MoveOperateFuelTank();

	/*UFUNCTION()
	void PlaySoundTest();*/

private:
	FTimerHandle FuelTankMoveTimer;

	FTimerHandle FuelTankSoundPlayTimer;

	// Capsule Collision In Handing Check Flag Variable
	bool bIsHanding = false;

	class UMotionControllerComponent* TempMCRef = nullptr;

	uint32 bIsOpen = 0;
	uint32 bIsAttachObjExist = 0;

	class AAFuelRod* EquipFuelRod;

	USoundBase* SFXFuelTankMove;
	USoundBase* SFXFuelRodIn;
	USoundBase* SFXFuelRodOut;
};
